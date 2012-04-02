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

#include <QFile>
#include <QDataStream>
#include "../Config.h"
#include "../LogInfo.h"
#include "../LogHandler.h"
#include "../JavascriptHandler.h"
#include "../Sound.h"
#include "api/Interface.h"
#include "Call.h"
#include "Account.h"
#include "Phone.h"

namespace phone
{

//-----------------------------------------------------------------------------
Phone::Phone(api::Interface *api) : api_(api)
{
    connect(api_, SIGNAL(signalAccountRegState(const int)),
            this, SLOT(slotAccountRegState(const int)));
    connect(api_, SIGNAL(signalIncomingCall(int, QString, QString)),
            this, SLOT(slotIncomingCall(int, QString, QString)));
    connect(api_, SIGNAL(signalCallState(int,int,int)),
            this, SLOT(slotCallState(int,int,int)));
    connect(api_, SIGNAL(signalSoundLevel(int)),
            this, SLOT(slotSoundLevel(int)));
    connect(api_, SIGNAL(signalMicrophoneLevel(int)),
            this, SLOT(slotMicrophoneLevel(int)));
    connect(api_, SIGNAL(signalLog(const LogInfo&)),
            this, SLOT(slotLogData(const LogInfo&)));
    connect(api_, SIGNAL(signalRingSound()),
            this, SLOT(slotRingSound()));
    connect(api_, SIGNAL(signalStopSound()),
            this, SLOT(slotStopSound()));
}

//-----------------------------------------------------------------------------
Phone::~Phone()
{
    QFile file("error.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QDataStream out(&file);
    for (int i = 0; i < call_list_.size(); i++) {
        Call *call = call_list_[i];
        if (call->isActive()) {
            out << *call;
        }
        delete call;
    }
    call_list_.clear();

    delete api_;
}

//-----------------------------------------------------------------------------
bool Phone::init()
{
    Config &config = Config::getInstance();
    return api_->init(5060, config.getStunServer());
}

//-----------------------------------------------------------------------------
void Phone::setJavascriptHandler(JavascriptHandler *js_handler)
{
    js_handler_ = js_handler;
}

//-----------------------------------------------------------------------------
api::Interface *Phone::getApi()
{
    return api_;
}

//-----------------------------------------------------------------------------
const QString &Phone::getErrorMessage() const
{
    return error_msg_;
}

//-----------------------------------------------------------------------------
bool Phone::checkAccountStatus()
{
    return api_->checkAccountStatus();
}

//-----------------------------------------------------------------------------
bool Phone::registerUser(const Account &acc)
{
    if (api_->registerUser(acc.getUsername(), acc.getPassword(), acc.getHost()) == -1) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
QVariantMap Phone::getAccountInfo()
{
    QVariantMap info;
    api_->getAccountInfo(info);
    return info;
}

//-----------------------------------------------------------------------------
bool Phone::addToCallList(Call *call)
{
    for (int i = 0; i < call_list_.size(); i++) {
        if (call_list_[i] == call) {
            return true;
        }
        if (call_list_[i]->getId() == call->getId()) {
            return false;
        }
    }

    call_list_.push_back(call);
    return true;
}

//-----------------------------------------------------------------------------
Call *Phone::makeCall(const QString &url)
{
    Call *call = new Call(this, Call::TYPE_OUTGOING);
    if (call->makeCall(url) < 0 || !addToCallList(call)) {
        delete call;
        return NULL;
    }
    return call;
}

//-----------------------------------------------------------------------------
void Phone::hangUpAll()
{
    api_->hangUpAll();
    for (int i = 0; i < call_list_.size(); i++) {
        call_list_[i]->setInactive();
    }
}

//-----------------------------------------------------------------------------
Call *Phone::getCall(const int call_id)
{
    for (int i = 0; i < call_list_.size(); i++) {
        if (call_list_[i]->getId() == call_id) {
            return call_list_[i];
        }
    }
    return NULL;
}

//-----------------------------------------------------------------------------
QVariantList Phone::getActiveCallList() const
{
    QVariantList list;
    for (int i = 0; i < call_list_.size(); ++i) {
        Call *call = call_list_[i];
        int id = call->getId();
        if (call->isActive()) {
            QVariantMap current;
            current.insert("id", id);
            current = call->getInfo();
            list << current;
        }
    }
    return list;
}

//-----------------------------------------------------------------------------
void Phone::muteSound(const bool mute, const int call_id)
{
    if (call_id == -1) {
        api_->muteSound(mute);
    } else {
        Call *call = getCall(call_id);
        if (call) {
            call->muteSound(mute);
        }
    }
}

//-----------------------------------------------------------------------------
void Phone::muteMicrophone(const bool mute, const int call_id)
{
    if (call_id == -1) {
        api_->muteMicrophone(mute);
    } else {
        Call *call = getCall(call_id);
        if (call) {
            call->muteMicrophone(mute);
        }
    }
}

//-----------------------------------------------------------------------------
QVariantMap Phone::getSignalInformation() const
{
    QVariantMap info;
    api_->getSignalInformation(info);
    return info;
}

//-----------------------------------------------------------------------------
void Phone::unregister()
{
    api_->unregister();
}

//-----------------------------------------------------------------------------
void Phone::slotIncomingCall(int call_id, const QString &url, const QString &name)
{
    Call *call = new Call(this, Call::TYPE_INCOMING);
    call->setId(call_id);
    call->setUrl(url);
    call->setName(name);

    if (!addToCallList(call)) {
        delete call;
        return;
    }
    js_handler_->incomingCall(*call);

    signalIncomingCall(call->getUrl());
}

//-----------------------------------------------------------------------------
void Phone::slotCallState(int call_id, int call_state, int last_status)
{
    Call *call = getCall(call_id);
    if (call) {
        call->setState(call_state);
    }

    js_handler_->callState(call_id, call_state, last_status);
}

//-----------------------------------------------------------------------------
void Phone::slotSoundLevel(int level)
{
    js_handler_->soundLevel(level);
}

//-----------------------------------------------------------------------------
void Phone::slotMicrophoneLevel(int level)
{
    js_handler_->microphoneLevel(level);
}

//-----------------------------------------------------------------------------
void Phone::slotAccountRegState(const int state)
{
    js_handler_->accountState(state);
}

//-----------------------------------------------------------------------------
void Phone::slotLogData(const LogInfo &info)
{
    if (info.status_ >= LogInfo::STATUS_ERROR) {
        error_msg_ = info.msg_;
    }
    LogHandler::getInstance().logData(info);
}

//-----------------------------------------------------------------------------
void Phone::slotRingSound()
{
    Sound::getInstance().startRing();
}

//-----------------------------------------------------------------------------
void Phone::slotStopSound()
{
    Sound::getInstance().stop();
}

} // phone::
