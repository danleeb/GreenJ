/****************************************************************************
**
** Copyright (C) 2012 Lorem Ipsum Mediengesellschaft m.b.H.
**
** GNU General Public License
** This file may be used under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation and
** appearing in the file LICENSE.GPL included in the packaging of this file.
**
****************************************************************************/

#include <QTextDocument>
#include "../../LogInfo.h"
#include "../Phone.h"
#include "Sip.h"

namespace phone
{
    namespace api
    {

Sip *Sip::self_;

//-----------------------------------------------------------------------------
Sip::Sip()
{
    self_ = this;
}

//-----------------------------------------------------------------------------
Sip::~Sip()
{
    pjsua_destroy();
}

//-----------------------------------------------------------------------------
bool Sip::init(unsigned int port, const QString &stun)
{
    // Create pjsua first
    pj_status_t status = pjsua_create();
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_FATAL, "pjsip", status, "Creating pjsua application failed"));
        return false;
    }

    // Init pjsua
    if (!_initPjsua(stun)) {
        return false;
    }

    // Add UDP transport
    if (!_addTransport(PJSIP_TRANSPORT_UDP, port)) {
        return false;
    }
    
    // Initialization is done, now start pjsua
    status = pjsua_start();
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_FATAL, "pjsip", status, "Couldn't start pjsua"));
        return false;
    }

    speaker_level_ = 1.f;
    mic_level_ = 1.f;
    pjsua_conf_adjust_rx_level(0, speaker_level_);
    pjsua_conf_adjust_tx_level(0, mic_level_);
    
    return true;
}

//-----------------------------------------------------------------------------
bool Sip::_initPjsua(const QString &stun)
{
    pjsua_config cfg;
    pjsua_logging_config log_cfg;
    pjsua_config_default(&cfg);

    // TODO: additional configurations
    // * max_calls
    // * nameserver_count, nameserver (instead of default pj_gethostbyname)
    // * outbound_proxy_cnt, outbound_proxy

    if (stun.size()) {
        if (stun.size() > 99) {
            signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", 0, "Couldn't initialize pjsip: Stun server string too long"));
            return false;
        }
        char ch_stun[100];
        strcpy(ch_stun, stun.toLocal8Bit().data());
        cfg.stun_srv[cfg.stun_srv_cnt++] = pj_str(ch_stun);
    }
    cfg.enable_unsolicited_mwi = PJ_FALSE;
    cfg.cb.on_incoming_call = &incomingCallCb;
    cfg.cb.on_call_state = &callStateCb;
    cfg.cb.on_call_media_state = &callMediaStateCb;
    cfg.cb.on_reg_state = &registerStateCb;

    pjsua_logging_config_default(&log_cfg);
    log_cfg.console_level = 4;

    pj_status_t status = pjsua_init(&cfg, &log_cfg, NULL);
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_FATAL, "pjsip", status, "pjsua initialization failed"));
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Sip::_addTransport(pjsip_transport_type_e type, unsigned int port)
{
    pjsua_transport_config cfg;
    pjsua_acc_id aid;
    pjsua_transport_id transport_id = -1;
    pjsua_transport_config tcp_cfg;

    pjsua_transport_config_default(&cfg);
    cfg.port = port;

    // TODO: tls settings

    pj_status_t status = pjsua_transport_create(type, &cfg, &transport_id);
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_FATAL, "pjsip", status, "Transport creation failed"));
        return false;
    }

    // Add local account
    pjsua_acc_add_local(transport_id, PJ_TRUE, &aid);
    pjsua_acc_set_online_status(aid, PJ_TRUE);

    if (cfg.port == 0) {
        pjsua_transport_info ti;
        pj_sockaddr_in *a;

        pjsua_transport_get_info(transport_id, &ti);
        a = (pj_sockaddr_in*)&ti.local_addr;

        tcp_cfg.port = pj_ntohs(a->sin_port);
    }
    return true;
}

//-----------------------------------------------------------------------------
int Sip::registerUser(const QString &user, const QString &password, const QString &domain)
{
    if (pjsua_acc_is_valid(account_id_)) {
        signalLog(LogInfo(LogInfo::STATUS_WARNING, "pjsip", 0, "Account already exists"));
        return -1;
    }

    QString id = "sip:" + user + "@" + domain;
    QString uri = "sip:" + domain;

    if (id.size() > 149
        || uri.size() > 99
        || user.size() > 99
        || password.size() > 99
        || domain.size() > 99)
    {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", 0, "Error adding account: Invalid data"));
        return -1;
    }

    char cid[150], curi[100], cuser[100], cpassword[100], cdomain[100];
    strcpy(cid, id.toLocal8Bit().constData());
    strcpy(curi, uri.toLocal8Bit().constData());
    strcpy(cuser, user.toLocal8Bit().constData());
    strcpy(cpassword, password.toLocal8Bit().constData());
    strcpy(cdomain, domain.toLocal8Bit().constData());

    // Register to SIP server by creating SIP account.
    pjsua_acc_config cfg;
    pjsua_acc_config_default(&cfg);

    cfg.id = pj_str(cid);
    cfg.reg_uri = pj_str(curi);
    cfg.cred_count = 1;
    //cfg.cred_info[0].realm = pj_str((char*)"*");
    cfg.cred_info[0].realm = pj_str(cdomain);
    cfg.cred_info[0].scheme = pj_str("digest");
    cfg.cred_info[0].username = pj_str(cuser);
    cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
    cfg.cred_info[0].data = pj_str(cpassword);

    pj_status_t status = pjsua_acc_add(&cfg, PJ_TRUE, &account_id_);
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", status, "Error adding account"));
        return -1;
    }
    signalLog(LogInfo(LogInfo::STATUS_MESSAGE, "pjsip", 0, 
                      "Registered user with account-id " + QString::number(account_id_)));

    return account_id_;
}

//-----------------------------------------------------------------------------
bool Sip::checkAccountStatus()
{
    return pjsua_acc_is_valid(account_id_);
}

//-----------------------------------------------------------------------------
void Sip::unregister()
{
    if (pjsua_acc_is_valid(account_id_)) {
        hangUpAll();
        pjsua_acc_del(account_id_);
        signalLog(LogInfo(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Account unregistered"));
    }
}

//-----------------------------------------------------------------------------
void Sip::getAccountInfo(QVariantMap &account_info)
{
    if (!pjsua_acc_is_valid(account_id_)) {
        signalLog(LogInfo(LogInfo::STATUS_WARNING, "pjsip", 0, "Account is not active"));
        return;
    }
    pjsua_acc_info ai;
    pjsua_acc_get_info(account_id_, &ai);

    account_info.insert("address", Qt::escape(ai.acc_uri.ptr));
    account_info.insert("status", Qt::escape(ai.status_text.ptr));
    account_info.insert("online_status", Qt::escape(ai.online_status_text.ptr));
}

//-----------------------------------------------------------------------------
void Sip::incomingCallCb(pjsua_acc_id acc_id, pjsua_call_id call_id,
                         pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    if (pjsua_call_get_count() <= 1) {
        self_->signalRingSound();
    }
    self_->signalLog(LogInfo(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Incoming call from " +
                             QString(ci.remote_contact.ptr)));

    self_->signalIncomingCall(call_id, QString(ci.remote_contact.ptr), QString(ci.remote_info.ptr));
}

//-----------------------------------------------------------------------------
void Sip::callStateCb(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &ci);
    
    if (ci.state == PJSIP_INV_STATE_CONFIRMED 
        || ci.state == PJSIP_INV_STATE_DISCONNECTED) 
    {
        self_->signalStopSound();
    }
    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        self_->hangUp(call_id);
    }

    self_->signalLog(LogInfo(LogInfo::STATUS_DEBUG, "pjsip", 0, 
                             "State of call " + QString::number(call_id)
                             + " changed to " + QString::number(ci.state)));

    self_->signalCallState(call_id, ci.state, ci.last_status);
}

//-----------------------------------------------------------------------------
void Sip::callMediaStateCb(pjsua_call_id call_id)
{
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);
    
    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
        // When media is active, connect call to sound device
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
    self_->signalLog(LogInfo(LogInfo::STATUS_DEBUG, "pjsip", 0,
                             "Media state of call " + QString::number(call_id)
                             + " changed to " + QString::number(ci.state)));
}

//-----------------------------------------------------------------------------
void Sip::registerStateCb(pjsua_acc_id acc_id)
{
    PJ_UNUSED_ARG(acc_id);
    pjsua_acc_info acc_info;

    pjsua_acc_get_info(self_->account_id_, &acc_info);

    QString msg("\t");
    msg.append(acc_info.status_text.ptr);
    if (acc_info.status < 300) {
        self_->signalLog(LogInfo(LogInfo::STATUS_MESSAGE, "pjsip-account", acc_info.status, msg));
    } else {
        self_->signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip-account", acc_info.status, msg));
    }
    self_->signalAccountRegState(acc_info.status);
}

//-----------------------------------------------------------------------------
int Sip::makeCall(const QString &url)
{
    if (url.size() > 149) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", 0, "Error making call: url too long"));
        return -1;
    }

    signalLog(LogInfo(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Make call"));

    char ch_url[150];
    strcpy(ch_url, url.toLocal8Bit().constData());
    pj_str_t uri = pj_str(ch_url);
    pjsua_call_id call_id;

    pj_status_t status = pjsua_call_make_call(account_id_, &uri, 0, NULL, NULL, &call_id);
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", status, "Error making call"));
        return -1;
    }
    return (int)call_id;
}

//-----------------------------------------------------------------------------
void Sip::answerCall(int call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    if (ci.state == PJSIP_INV_STATE_INCOMING) {
        pjsua_call_answer((pjsua_call_id)call_id, 200, NULL, NULL);
        signalLog(LogInfo(LogInfo::STATUS_DEBUG, "pjsip", ci.state, 
                          "Call " + QString::number(call_id) + " answered"));
    } else {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", ci.state, 
                          "Call " + QString::number(call_id) + " is not an incoming call"));
    }

    signalStopSound();
}

//-----------------------------------------------------------------------------
void Sip::hangUp(const int call_id)
{
    signalLog(LogInfo(LogInfo::STATUS_DEBUG, "psjip", 0, "Hangup call " + QString::number(call_id)));

    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    pjsua_call_hangup(call_id, 0, 0, 0);

    signalStopSound();
}

//-----------------------------------------------------------------------------
void Sip::hangUpAll()
{
    pjsua_call_hangup_all();
    
    signalStopSound();
}

//-----------------------------------------------------------------------------
bool Sip::addCallToConference(const int call_src, const int call_dest)
{
    if (call_src == -1 || call_dest == -1) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", 0, "Error: Conference calls are not valid"));
        return false;
    }

    pjsua_call_info src_ci, dest_ci;

    pjsua_call_get_info(call_src, &src_ci);
    pjsua_call_get_info(call_dest, &dest_ci);

    pj_status_t status =  pjsua_conf_connect(src_ci.conf_slot, dest_ci.conf_slot);
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", status, "Error connecting conference"));
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Sip::removeCallFromConference(const int call_src, const int call_dest)
{
    if (call_src == -1 || call_dest == -1) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", 0, "Error: Conference calls are not valid"));
        return false;
    }

    pjsua_call_info src_ci, dest_ci;

    pjsua_call_get_info(call_src, &src_ci);
    pjsua_call_get_info(call_dest, &dest_ci);

    pj_status_t status =  pjsua_conf_disconnect(src_ci.conf_slot, dest_ci.conf_slot);
    if (status != PJ_SUCCESS) {
        signalLog(LogInfo(LogInfo::STATUS_ERROR, "pjsip", status, "Error disconnecting conference"));
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
int Sip::redirectCall(const int call_id, const QString &dest_uri)
{
    pjsua_msg_data msg_data;
    pjsua_msg_data_init(&msg_data);
    pj_str_t str = pj_str((char*)dest_uri.toLocal8Bit().data());

    return pjsua_call_xfer(call_id, &str, NULL);
}

//-----------------------------------------------------------------------------
QString Sip::getCallUrl(const int call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);
    return QString(ci.remote_contact.ptr);
}

//-----------------------------------------------------------------------------
void Sip::getCallInfo(const int call_id, QVariantMap &call_info)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);

    call_info.insert("address", Qt::escape(ci.remote_contact.ptr));
    call_info.insert("number", Qt::escape(ci.remote_info.ptr));
    call_info.insert("stateText", Qt::escape(ci.state_text.ptr));
    call_info.insert("state", (int)ci.state);
    call_info.insert("lastStatus", Qt::escape(ci.last_status_text.ptr));
    call_info.insert("duration", (int)ci.connect_duration.sec);
}

//-----------------------------------------------------------------------------
void Sip::muteSound(const bool mute)
{
    speaker_level_ = mute ? 0.f : 1.f;
    QString str = mute ? "true" : "false";
    signalLog(LogInfo(LogInfo::STATUS_DEBUG, "phone", 0, "mute sound: " + str));
    pjsua_conf_adjust_tx_level(0, speaker_level_);
    signalSoundLevel(int(speaker_level_ * 255));
}

//-----------------------------------------------------------------------------
void Sip::muteSound(const int call_id, const float mute)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);
    pjsua_conf_adjust_rx_level(ci.conf_slot, mute);
}

//-----------------------------------------------------------------------------
void Sip::muteMicrophone(const bool mute)
{
    mic_level_ = mute ? 0.f : 1.f;
    QString str = mute ? "true" : "false";
    signalLog(LogInfo(LogInfo::STATUS_DEBUG, "phone", 0, "mute microphone: " + str));
    pjsua_conf_adjust_rx_level(0, mic_level_);
    signalMicrophoneLevel(int(mic_level_ * 255));
}

//-----------------------------------------------------------------------------
void Sip::muteMicrophone(const int call_id, const float mute)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id, &ci);
    pjsua_conf_adjust_tx_level(ci.conf_slot, mute);
}

//-----------------------------------------------------------------------------
void Sip::getSignalInformation(QVariantMap &signal_info)
{
    signal_info.insert("sound", speaker_level_);
    signal_info.insert("micro", mic_level_);
}

}} // phone::api::
