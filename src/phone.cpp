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
#include "call.h"
#include "phone_api.h"
#include "log_info.h"
#include "log_handler.h"
#include "javascript_handler.h"
#include "account.h"
#include "phone.h"

//-----------------------------------------------------------------------------
Phone::Phone(PhoneApi *api) : phone_api_(api)
{
    phone_api_->init();

    connect(phone_api_, SIGNAL(signalAccountRegState(const int)),
            this,       SLOT(accountRegState(const int)));
    connect(phone_api_, SIGNAL(signalIncomingCall(Call*)),
            this,       SLOT(slotIncomingCall(Call*)));
    connect(phone_api_, SIGNAL(signalCallState(int,int,int)),
            this,       SLOT(slotCallState(int,int,int)));
    connect(phone_api_, SIGNAL(signalSoundLevel(int)),
            this,       SLOT(slotSoundLevel(int)));

    connect(phone_api_, SIGNAL(signalMicrophoneLevel(int)),
            this,       SLOT(slotMicrophoneLevel(int)));
    connect(phone_api_,                 SIGNAL(signalLogData(const LogInfo&)),
            &LogHandler::getInstance(), SLOT(slotLogData(const LogInfo&)));
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
    delete phone_api_;
}

//-----------------------------------------------------------------------------
void Phone::setJsHandler(JavascriptHandler *js_handler)
{
    js_handler_ = js_handler;
}

//-----------------------------------------------------------------------------
bool Phone::checkAccountStatus()
{
    return phone_api_->checkAccountStatus();
}

//-----------------------------------------------------------------------------
bool Phone::registerUser(const Account &acc)
{
    if (phone_api_->registerUser(acc) == -1) {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
void Phone::getAccountInfo(QVariantMap &account_info)
{
    phone_api_->getAccountInfo(account_info);
}

//-----------------------------------------------------------------------------
bool Phone::addToCallList(Call *call)
{
    for (int i=0; i < call_list_.size(); i++) {
        if (call_list_[i] == call) {
            return true;
        }
        if (call_list_[i]->getCallId() == call->getCallId()) {
            return false;
        }
    }

    call_list_.push_back(call);
    return true;
}

//-----------------------------------------------------------------------------
Call *Phone::getCallFromList(const int call_id)
{
    for (int i=0; i< call_list_.size(); i++) {
        if (call_list_[i]->getCallId() == call_id) {
            return call_list_[i];
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------
int Phone::makeCall(const QString &url)
{
    Call *call = new Call(phone_api_, Call::TYPE_OUTGOING);

    call->setUrl(url);

    int call_id = call->makeCall();
    if (call_id != -1) {
        if (!addToCallList(call)) {
            delete call;
        }
    } else {
        delete call;
    }
    return call_id;
}

//-----------------------------------------------------------------------------
void Phone::answerCall(const int call_id)
{
    Call *call = getCallFromList(call_id);

    if (call) {
        call->answerCall();
    } else {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Call to answer doesn't exist!"));
    }
}

//-----------------------------------------------------------------------------
void Phone::hangUp(const int call_id)
{
    Call *call = getCallFromList(call_id);

    if (call) {
        call->hangUp();
    }
}


//-----------------------------------------------------------------------------
void Phone::hangUpAll()
{
    phone_api_->hangUpAll();
    for (int i=0; i < call_list_.size(); i++) {
        call_list_[i]->setInactive();
    }
}

//-----------------------------------------------------------------------------
QString Phone::getCallUserData(const int call_id)
{
    Call *call = getCallFromList(call_id);
    if (call) {
        return call->getUserData();
    }
    return "";
}

//-----------------------------------------------------------------------------
void Phone::setCallUserData(const int call_id, const QString &data)
{
    Call *call = getCallFromList(call_id);
    if (call) {
        call->setUserData(data);
    }
}

//-----------------------------------------------------------------------------
void Phone::clearCallUserData(const int call_id)
{
    Call *call = getCallFromList(call_id);
    if (call) {
        call->clearUserData();
    }
}

//-----------------------------------------------------------------------------
bool Phone::addCallToConference(const int call_src, const int call_dest)
{
    Call *call = getCallFromList(call_src);
    Call *dest_call = getCallFromList(call_dest);
    if (!call || !dest_call) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: one of the selected calls does NOT exist!"));
        return false;
    }
    if (!call->isActive() || !dest_call->isActive()) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: one of the selected calls just ended!"));
        return false;
    }
    if (!call->addCallToConference(*dest_call)) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: failed to connect to source!"));
        return false;
    }
    if (!dest_call->addCallToConference(*call)) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: failed to connect to destination!"));
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool Phone::removeCallFromConference(const int call_src, const int call_dest)
{
    Call *call = getCallFromList(call_src);
    Call *dest_call = getCallFromList(call_dest);
    if (!call || !dest_call) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: one of the selected calls does NOT exist!"));
        return false;
    }
    if (!call->isActive() || !dest_call->isActive()) {;
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: one of the selected calls just ended!"));
        return false;
    }
    if (call->removeCallFromConference(*dest_call)) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: failed to remove from source!"));
        return false;
    }
    if(dest_call->removeCallFromConference(*call)) {
        LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "phone", 0, "Error: failed to remove from destination!"));
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
int Phone::redirectCall(const int call_id, const QString &dest_uri)
{
    Call *call = getCallFromList(call_id);
    if (call) {
        return call->redirectCall(dest_uri);
    }
    return -1;
}

//-----------------------------------------------------------------------------
QString Phone::getCallUrl(const int call_id)
{
    Call *call = getCallFromList(call_id);
    if (call) {
        return call->getCallUrl();
    }
    return "";
}

//-----------------------------------------------------------------------------
void Phone::getCallInfo(const int call_id, QVariantMap &call_info)
{
    Call *call = getCallFromList(call_id);
    if (call) {
        call->getCallInfo(call_info);
    }
}

//-----------------------------------------------------------------------------
void Phone::getActiveCallList(QVariantList &call_list)
{
    for (int i=0; i < call_list_.size(); ++i) {
        int id = call_list_[i]->getCallId();
        if (call_list_[i]->isActive()) {
            QVariantMap current;
            current.insert("id",id);
            getCallInfo(id, current);
            call_list << current;
        }
    }
}

//-----------------------------------------------------------------------------
void Phone::muteSound(const bool mute, const int call_id)
{
    if (call_id == -1) {
        phone_api_->muteSound(mute);
    } else {
        Call *call = getCallFromList(call_id);
        if (call) {
            call->muteSound(mute);
        }
    }
}

//-----------------------------------------------------------------------------
void Phone::muteMicrophone(const bool mute, const int call_id)
{
    if (call_id == -1) {
        phone_api_->muteMicrophone(mute);
    } else {
        Call *call = getCallFromList(call_id);
        if (call) {
            call->muteMicrophone(mute);
        }
    }
}

//-----------------------------------------------------------------------------
void Phone::getSignalInformation(QVariantMap &signal_info)
{
    phone_api_->getSignalInformation(signal_info);
}

//-----------------------------------------------------------------------------
void Phone::unregister()
{
    phone_api_->unregister();
}

//-----------------------------------------------------------------------------
void Phone::slotIncomingCall(Call *call)
{
    if (!addToCallList(call)) {
        delete call;
        return;
    }
    js_handler_->incomingCall(*call);

    signalIncomingCall(call->getCallUrl());
}

//-----------------------------------------------------------------------------
void Phone::slotCallState(int call_id, int call_state, int last_status)
{
    Call *call = getCallFromList(call_id);
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
void Phone::accountRegState(const int state)
{
    js_handler_->accountState(state);
}
