/****************************************************************************
**
** Copyright (C) 2011 Lorem Ipsum Mediengesellschaft m.b.H.
**
** GNU General Public License
** This file may be used under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation and
** appearing in the file LICENSE.GPL included in the packaging of this file.
**
****************************************************************************/

#include "sip_phone.h"

#include <QApplication>
#include <QTextDocument>

#include "phone.h"
#include "gui.h"
#include "call.h"
#include "log_handler.h"
#include "account.h"
#include "config_file_handler.h"

SipPhone *SipPhone::self_;

//----------------------------------------------------------------------
SipPhone::SipPhone()
{
    self_ = this;
}

//----------------------------------------------------------------------
void SipPhone::init()
{
    pj_status_t status;
    /* Create pjsua first! */
    status = pjsua_create();

    if (status != PJ_SUCCESS)
    {
        LogInfo info(LogInfo::STATUS_FATAL_ERROR, "pjsip", status, "Error in pjsua_create()");
        signalLogData(info);
        return;
    }

    /* Init pjsua */
    {
        pjsua_config cfg;
        pjsua_logging_config log_cfg;
        ConfigFileHandler &config = ConfigFileHandler::getInstance();
        QString stun = config.getStunServer();
        pjsua_config_default(&cfg);

        if (stun.size())
        {
            char ch_stun[100];
            if (stun.size() > 99)
            {
                LogInfo info(LogInfo::STATUS_ERROR, "pjsip", 0, "Error init pjsip, stun-server too long");
                signalLogData(info);
                return;
            }

            strcpy(ch_stun, stun.toLocal8Bit().data());
            ch_stun[stun.size()] = 0;

            cfg.stun_srv[cfg.stun_srv_cnt++] = pj_str(ch_stun);
        }
        cfg.enable_unsolicited_mwi = PJ_FALSE;
        cfg.cb.on_incoming_call = &incomingCallCb;
        cfg.cb.on_call_state = &callStateCb;
        cfg.cb.on_call_media_state = &callMediaStateCb;
        cfg.cb.on_reg_state = &regStateCb;

        pjsua_logging_config_default(&log_cfg);
        log_cfg.console_level = 4;

        status = pjsua_init(&cfg, &log_cfg, NULL);
        printf("init successfull\n");
        if (status != PJ_SUCCESS)
        {
            LogInfo info(LogInfo::STATUS_FATAL_ERROR, "pjsip", status, "Error in pjsua_init()");
            signalLogData(info);
            return;
        }
    }

    /* Add UDP transport. */
    {
        pjsua_transport_config cfg;
        pjsua_acc_id aid;
        pjsua_transport_id transport_id = -1;
        pjsua_transport_config tcp_cfg;

        pjsua_transport_config_default(&cfg);
        cfg.port = 5060;

        status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &transport_id);

        if (status != PJ_SUCCESS)
        {
            LogInfo info(LogInfo::STATUS_FATAL_ERROR, "pjsip", status, "Error creating transport");
            signalLogData(info);
            return;
        }

        /* Add local account */
        pjsua_acc_add_local(transport_id, PJ_TRUE, &aid);
        pjsua_acc_set_online_status(aid, PJ_TRUE);

        if (cfg.port == 0)
        {
            pjsua_transport_info ti;
            pj_sockaddr_in *a;

            pjsua_transport_get_info(transport_id, &ti);
            a = (pj_sockaddr_in*)&ti.local_addr;

            tcp_cfg.port = pj_ntohs(a->sin_port);
        }

    }
    /* Initialization is done, now start pjsua */
    status = pjsua_start();

    if (status != PJ_SUCCESS)
    {
        LogInfo info(LogInfo::STATUS_FATAL_ERROR, "pjsip", status, "Error starting PJSUA");
        signalLogData(info);
        return;
    }
    pjsua_conf_adjust_rx_level(0, 1.f);
    pjsua_conf_adjust_tx_level(0, 1.f);
    speaker_level_ = 1.f;
    mic_level_ = 1.f;
}

//----------------------------------------------------------------------
bool SipPhone::checkAccountStatus()
{
    return pjsua_acc_is_valid(acc_id_);
}

//----------------------------------------------------------------------
int SipPhone::registerUser(const Account &acc)
{
    QString user = acc.getUserName();
    QString password = acc.getPassword();
    QString domain = acc.getHost();
    if (pjsua_acc_is_valid(acc_id_))
    {
        LogInfo info(LogInfo::STATUS_WARNING, "pjsip", 0, "Account already exists");
        signalLogData(info);
        return -1;
    }

    /* Register to SIP server by creating SIP account. */
    pjsua_acc_config cfg;

    pjsua_acc_config_default(&cfg);

    QString id = "sip:";
    id.append(user);
    id.append("@");
    id.append(domain);

    QString reg_uri = "sip:";
    reg_uri.append(domain);

    char ch_id[150];
    char ch_reg_uri[100];
    char ch_user[100];
    char ch_password[100];

    if (id.size() > 149 ||
        reg_uri.size() > 99 ||
        user.size() > 99 ||
        password.size() > 99)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", 0, "Error adding account, data too long");
        signalLogData(info);
        return -1;
    }

    strcpy(ch_id, id.toLocal8Bit().data());
    ch_id[id.size()] = 0;

    strcpy(ch_reg_uri, reg_uri.toLocal8Bit().data());
    ch_reg_uri[reg_uri.size()] = 0;

    strcpy(ch_user, user.toLocal8Bit().data());
    ch_user[user.size()] = 0;
    strcpy(ch_password, password.toLocal8Bit().data());
    ch_password[password.size()] = 0;

    cfg.id = pj_str(ch_id);
    cfg.reg_uri = pj_str(ch_reg_uri);
    cfg.cred_count = 1;
    cfg.cred_info[0].realm = pj_str((char*)"*");
    cfg.cred_info[0].scheme = pj_str((char*)"digest");
    cfg.cred_info[0].username = pj_str(ch_user);
    cfg.cred_info[0].data_type = 0;
    cfg.cred_info[0].data = pj_str(ch_password);

    pj_status_t status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id_);

    if (status != PJ_SUCCESS)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", status, "Error adding account");
        signalLogData(info);
        return -1;
    }
    LogInfo info(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Registered user with account-id "
                 + QString::number(acc_id_));
    signalLogData(info);

    return acc_id_;
}

//----------------------------------------------------------------------
void SipPhone::getAccountInfo(QVariantMap &account_info)
{
    if (!pjsua_acc_is_valid(acc_id_))
    {
        LogInfo info(LogInfo::STATUS_WARNING, "pjsip", 0, "There is no active account");
        signalLogData(info);
        return;
    }
    pjsua_acc_info ai;
    pjsua_acc_get_info(acc_id_, &ai);

    account_info.insert("address", Qt::escape(ai.acc_uri.ptr));
    account_info.insert("status", Qt::escape(ai.status_text.ptr));
    account_info.insert("online_status", Qt::escape(ai.online_status_text.ptr));
}

//----------------------------------------------------------------------
void SipPhone::incomingCallCb(pjsua_acc_id acc_id, pjsua_call_id call_id,
                              pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    if (pjsua_call_get_count() <= 1)
        Sound::getInstance().startRing();

    Call *call = new Call(self_, Call::TYPE_INCOMING);
    call->setCallId(call_id);
    call->setUrl(ci.remote_contact.ptr);
    call->setName(ci.remote_info.ptr);
    LogInfo info(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Incoming Call");
    self_->signalLogData(info);

    self_->signalIncomingCall(call);
}


//----------------------------------------------------------------------
void SipPhone::callStateCb(pjsua_call_id call_id, pjsip_event *e)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &ci);
    
    if (ci.state == PJSIP_INV_STATE_CONFIRMED || ci.state == PJSIP_INV_STATE_DISCONNECTED) 
    {
        Sound::getInstance().stopRing();
    }

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED)
    {
        self_->hangUp(call_id);
    }

    LogInfo info(LogInfo::STATUS_DEBUG, "pjsip", 0, "Call-state from call "+QString::number(call_id)+" changed to "+QString::number(ci.state));
    self_->signalLogData(info);

    self_->signalCallState(call_id, ci.state, ci.last_status);
}

//----------------------------------------------------------------------
void SipPhone::callMediaStateCb(pjsua_call_id call_id)
{
    pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);
    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) 
    {
        // When media is active, connect call to sound device.
        pjsua_conf_connect(ci.conf_slot, 0);
        pjsua_conf_connect(0, ci.conf_slot);
    }
    LogInfo info(LogInfo::STATUS_DEBUG, "pjsip", 0, "Call-media-state changed to "+QString::number(ci.state));
    self_->signalLogData(info);
}

//----------------------------------------------------------------------
void SipPhone::regStateCb(pjsua_acc_id acc)
{
    PJ_UNUSED_ARG(acc);
    pjsua_acc_info acc_info;

    pjsua_acc_get_info(self_->acc_id_, &acc_info);

    QString msg("\t");
    msg.append(acc_info.status_text.ptr);
    if (acc_info.status < 300)
    {
        LogInfo info(LogInfo::STATUS_MESSAGE, "account", acc_info.status, msg);
        self_->signalLogData(info);
    }
    else
    {
        LogInfo info(LogInfo::STATUS_ERROR, "account", acc_info.status, msg);
        self_->signalLogData(info);
    }
    self_->signalAccountRegState(acc_info.status);
}

//----------------------------------------------------------------------
int SipPhone::makeCall(const QString &url)
{
    if (url.size() > 149)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", 0, "Error making call, phoneurl too long");
        signalLogData(info);
        return -1;
    }

    char ch_url[150];

    strcpy(ch_url, url.toLocal8Bit().data());
    ch_url[url.size()] = 0;

    pj_str_t uri = pj_str(ch_url);
    pjsua_call_id call_id;

    LogInfo info(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Make call");
    signalLogData(info);

    pj_status_t status = pjsua_call_make_call(acc_id_, &uri, 0, NULL, NULL, &call_id);

    if (status != PJ_SUCCESS)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", status, "Error making call");
        signalLogData(info);
        return -1;
    }
    return (int)call_id;
}

//----------------------------------------------------------------------
void SipPhone::answerCall(int call_id)
{
    pjsua_call_info call_info;
    pjsua_call_get_info(call_id, &call_info);

    if (call_info.state == PJSIP_INV_STATE_INCOMING)
    {
        pjsua_call_answer((pjsua_call_id)call_id, 200, NULL, NULL);
        LogInfo info(LogInfo::STATUS_DEBUG, "pjsip", call_info.state, "Call answered");
        signalLogData(info);
    }
    else
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", call_info.state, "Call not incoming");
        signalLogData(info);
    }
    finishIncoming();
}

//----------------------------------------------------------------------
void SipPhone::hangUp(const int &call_id)
{
    LogInfo info(LogInfo::STATUS_DEBUG, "psjip", 0, "hangup");
    LogHandler::getInstance().logData(info);

    pjsua_call_info ci;
    pjsua_call_get_info(call_id,&ci);

    pjsua_call_hangup(call_id,0,0,0);

    finishIncoming();
}

//----------------------------------------------------------------------
void SipPhone::hangUpAll()
{
    pjsua_call_hangup_all();
    finishIncoming();
}

//----------------------------------------------------------------------
void SipPhone::finishIncoming()
{
    LogInfo info(LogInfo::STATUS_DEBUG, "psjip", 0, "stop the sound");
    LogHandler::getInstance().logData(info);

    Sound::getInstance().stopRing();
}


//----------------------------------------------------------------------
bool SipPhone::addCallToConference(const int &call_src, const int &call_dest)
{
    int src = call_src;
    int dest = call_dest;

    if (src == -1 || dest == -1)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", 0, "Error: one of the selected calls does NOT exist!");
        signalLogData(info);
        return false;
    }

    pjsua_call_info src_ci, dest_ci;

    pjsua_call_get_info(src,&src_ci);
    pjsua_call_get_info(dest,&dest_ci);

    pj_status_t status =  pjsua_conf_connect(src_ci.conf_slot,dest_ci.conf_slot);
    if (status != PJ_SUCCESS)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", status, "Error connecting conference!");
        signalLogData(info);
        return false;
    }

    return true;
}

//----------------------------------------------------------------------
bool SipPhone::removeCallFromConference(const int &call_src, const int &call_dest)
{
    int src = call_src;
    int dest = call_dest;

    if (src == -1 || dest == -1)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", 0, "Error: one of the selected calls does NOT exist!");
        signalLogData(info);
        return false;
    }

    pjsua_call_info src_ci, dest_ci;

    pjsua_call_get_info(src,&src_ci);
    pjsua_call_get_info(dest,&dest_ci);

    pj_status_t status =  pjsua_conf_disconnect(src_ci.conf_slot,dest_ci.conf_slot);
    if (status != PJ_SUCCESS)
    {
        LogInfo info(LogInfo::STATUS_ERROR, "pjsip", status, "Error connecting conference (1/2)!");
        signalLogData(info);
        return false;
    }
    return true;
}

//----------------------------------------------------------------------
int SipPhone::redirectCall(const int &call_id, const QString &dest_uri)
{
    pjsua_msg_data msg_data;
    pjsua_msg_data_init(&msg_data);
    pj_str_t str = pj_str((char*)dest_uri.toLocal8Bit().data());

    return pjsua_call_xfer(call_id,&str,NULL);
}

//----------------------------------------------------------------------
QString SipPhone::getCallUrl(const int &call_id)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id,&ci);
    return QString(ci.remote_contact.ptr);
}

//----------------------------------------------------------------------
void SipPhone::getCallInfo(const int &call_id, QVariantMap &call_info)
{
    pjsua_call_info ci;
    pjsua_call_get_info(call_id,&ci);

    call_info.insert("address", Qt::escape(ci.remote_contact.ptr));
    call_info.insert("number", Qt::escape(ci.remote_info.ptr));
    call_info.insert("stateText", Qt::escape(ci.state_text.ptr));
    call_info.insert("state", (int)ci.state);
    call_info.insert("lastStatus", Qt::escape(ci.last_status_text.ptr));
    call_info.insert("duration", (int)ci.connect_duration.sec);
}

//----------------------------------------------------------------------
void SipPhone::muteSound(const bool &mute)
{
    float level = 0.f;
    if (mute)
    {
        LogInfo info(LogInfo::STATUS_MESSAGE, "phone", 0, "muteSound: true");
        signalLogData(info);
        level = 0.f;
    }
    else
    {
        LogInfo info(LogInfo::STATUS_MESSAGE, "phone", 0, "muteSound: false");
        signalLogData(info);
        level = 1.f;
    }
    speaker_level_ = level;
    pjsua_conf_adjust_tx_level(0, level);
    self_->signalSoundLevel(int(level * 255));
}

//----------------------------------------------------------------------
void SipPhone::muteSoundForCall(const int &call_id, const float &mute)
{
    pjsua_call_info ci;
    
    pjsua_call_get_info(call_id,&ci);
    
    pjsua_conf_adjust_rx_level(ci.conf_slot, mute);
}


//----------------------------------------------------------------------
void SipPhone::muteMicrophone(const bool &mute)
{
    float level = 0.f;
    if (mute)
    {
        LogInfo info(LogInfo::STATUS_MESSAGE, "phone", 0, "muteMicrophone: true");
        signalLogData(info);
        level = 0.f;
    }
    else
    {
        LogInfo info(LogInfo::STATUS_MESSAGE, "phone", 0, "muteMicrophone: false");
        signalLogData(info);
        level = 1.f;
    }
    mic_level_ = level;
    pjsua_conf_adjust_rx_level(0, level);
    self_->signalMicrophoneLevel(int(level * 255));
}

//----------------------------------------------------------------------
void SipPhone::muteMicrophoneForCall(const int &call_id, const float &mute)
{
    pjsua_call_info ci;
    
    pjsua_call_get_info(call_id,&ci);
    
    pjsua_conf_adjust_tx_level(ci.conf_slot, mute);
}

//----------------------------------------------------------------------
void SipPhone::getSignalInformation(QVariantMap &signal_info)
{
    signal_info.insert("sound", speaker_level_);
    signal_info.insert("micro", mic_level_);
}

//----------------------------------------------------------------------
void SipPhone::unregister()
{
    LogInfo info(LogInfo::STATUS_MESSAGE, "pjsip", 0, "Unregister account");
    signalLogData(info);
    if (pjsua_acc_is_valid(acc_id_))
    {
        hangUpAll();
        pjsua_acc_del(acc_id_);
    }
}

//----------------------------------------------------------------------
SipPhone::~SipPhone(void)
{
    pjsua_destroy();
}
