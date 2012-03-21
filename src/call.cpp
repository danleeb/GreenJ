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

#include "phone_api.h"
#include "sound.h"
#include "log_info.h"
#include "log_handler.h"
#include "call.h"

const int Call::TYPE_UNKNOWN    = -1;
const int Call::TYPE_INCOMING   = 0x00;
const int Call::TYPE_OUTGOING   = 0x01;

const int Call::STATUS_UNKNOWN  = -1;
const int Call::STATUS_RINGING  = 0x00;
const int Call::STATUS_ACCEPTED = 0x01;
const int Call::STATUS_CLOSED   = 0x02;
const int Call::STATUS_ERROR    = 0x03;

//-----------------------------------------------------------------------------
Call::Call(PhoneApi *phone_api, const int type, const int status) :
    phone_api_(phone_api), 
    type_(type), status_(status), active_(false), 
    call_id_(-1), call_state_(0), media_state_(0), 
    speaker_level_(1.f), mic_level_(1.f), 
    start_time_(QDateTime::currentDateTime())
{
}

//-----------------------------------------------------------------------------
int Call::makeCall()
{
    Sound::getInstance().startDial();

    int call_id = phone_api_->makeCall(url_);
    active_ = true;

    if (call_id < 0) {
        Sound::getInstance().stop();
    }
    call_id_ = call_id;
 
    return call_id_;
}

//-----------------------------------------------------------------------------
void Call::answerCall()
{
    if (call_id_ != -1) {
        phone_api_->answerCall(call_id_);
    }
}

//-----------------------------------------------------------------------------
void Call::hangUp()
{
    if (call_id_ != -1) {
        phone_api_->hangUp(call_id_);
    }
    setInactive();
}

//-----------------------------------------------------------------------------
bool Call::addCallToConference(const Call &call_dest)
{
    return phone_api_->addCallToConference(call_id_, call_dest.getCallId());
}

//-----------------------------------------------------------------------------
bool Call::removeCallFromConference(const Call &call_dest)
{
    return phone_api_->removeCallFromConference(call_id_, call_dest.getCallId());
}

//-----------------------------------------------------------------------------
int Call::redirectCall(const QString &dest_uri)
{
    return phone_api_->redirectCall(call_id_, dest_uri);
}

//-----------------------------------------------------------------------------
const QString &Call::getCallUrl() const
{
    return url_;
}

//-----------------------------------------------------------------------------
const QString &Call::getCallName() const
{
    return name_;
}

//-----------------------------------------------------------------------------
const int Call::getCallId() const
{
    return call_id_;
}

//-----------------------------------------------------------------------------
void Call::getCallInfo(QVariantMap &call_info)
{
    if (phone_api_) {
        phone_api_->getCallInfo(call_id_, call_info);
    } else {
        call_info.insert("number", url_);
        call_info.insert("duration", duration_);
    }
    call_info.insert("name", name_);
    call_info.insert("type", (int)type_);
    call_info.insert("status", (int)status_);
    call_info.insert("callTime", start_time_.toMSecsSinceEpoch());
    call_info.insert("acceptTime", accept_time_.toMSecsSinceEpoch());
    call_info.insert("closeTime", close_time_.toMSecsSinceEpoch());
    call_info.insert("userData", user_data_);
}

//-----------------------------------------------------------------------------
const int Call::getStatus() const
{
    return status_;
}

//-----------------------------------------------------------------------------
const int Call::getType() const
{
    return type_;
}

//-----------------------------------------------------------------------------
const QDateTime &Call::getStartTime() const
{
    return start_time_;
}

//-----------------------------------------------------------------------------
const QDateTime &Call::getAcceptTime() const
{
    return accept_time_;
}

//-----------------------------------------------------------------------------
const QDateTime &Call::getCloseTime() const
{
    return close_time_;
}

//-----------------------------------------------------------------------------
const int Call::getDuration() const
{
    return duration_;
}

//-----------------------------------------------------------------------------
void Call::setStartTime(const QDateTime &start_time)
{
    start_time_ = start_time;
}

//-----------------------------------------------------------------------------
void Call::setAcceptTime(const QDateTime &accept_time)
{
    accept_time_ = accept_time;
}

//-----------------------------------------------------------------------------
void Call::setCloseTime(const QDateTime &close_time)
{
    close_time_ = close_time;
}

//-----------------------------------------------------------------------------
void Call::setDuration(const int duration)
{
    duration_ = duration;
}

//-----------------------------------------------------------------------------
QString Call::getUserData() const
{
    return user_data_;
}

//-----------------------------------------------------------------------------
void Call::setUserData(const QString &data)
{
    user_data_ = data;
}

//-----------------------------------------------------------------------------
void Call::clearUserData()
{
    user_data_ = "";
}

//-----------------------------------------------------------------------------
bool Call::isActive() const
{
    return active_;
}

//-----------------------------------------------------------------------------
void Call::setUrl(const QString &url)
{
    url_ = url;
}

//-----------------------------------------------------------------------------
void Call::setName(const QString &name)
{
    QStringList list = name.split("\"");

    if (list.size() >= 2) {
        name_ = list[1];
    } else {
        name_ = name;
    }
}

//-----------------------------------------------------------------------------
void Call::setCallId(const int call_id)
{
    call_id_ = call_id;
}

//-----------------------------------------------------------------------------
void Call::setActive()
{
    active_ = true;
    accept_time_ = QDateTime::currentDateTime();
}

//-----------------------------------------------------------------------------
void Call::setInactive()
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "call", 0, "set call inactive"));

    active_ = false;
    close_time_ = QDateTime::currentDateTime();
    QDateTime stop_time = QDateTime::currentDateTime();
    duration_ = start_time_.secsTo(stop_time);
}

//-----------------------------------------------------------------------------
void Call::setState(const int state)
{
    call_state_ = state;
    switch(state) {
        case 2:
        case 3:
            status_ = Call::STATUS_RINGING;
            break;
        case 5:
            status_ = Call::STATUS_ACCEPTED;
            setActive();
            break;
        case 6:
            active_ = false;
            status_ = Call::STATUS_CLOSED;
            break;
    }
}

//-----------------------------------------------------------------------------
void Call::setMediaState(const int state)
{
    media_state_ = state;
}

//-----------------------------------------------------------------------------
void Call::muteSound(const bool mute)
{
    if (mute) {
        speaker_level_ = 0.f;
    } else {
        speaker_level_ = 1.f;
    }
    phone_api_->muteSoundForCall(call_id_, speaker_level_);
}

//-----------------------------------------------------------------------------
void Call::muteMicrophone(const bool mute)
{
    if (mute) {
        mic_level_ = 0.f;
    } else {
        mic_level_ = 1.f;
    }
    phone_api_->muteMicrophoneForCall(call_id_, mic_level_);
}

//-----------------------------------------------------------------------------
void Call::getSignalInformation(QVariantMap &signal_info)
{
    signal_info.insert("sound", speaker_level_);
    signal_info.insert("micro", mic_level_);
}

//-----------------------------------------------------------------------------
// OVERLOADING QDataStream
//-----------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const Call &call)
{
    out << call.getType() << call.getCallId() << call.getCallUrl()
        << call.getStatus() << call.getStartTime() << call.getAcceptTime()
        << call.getCloseTime() << call.getDuration() << call.getUserData();
    return out;
}

//-----------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, Call &call)
{
    int type;
    int call_id;
    QString call_url;
    QDateTime start_time;
    QDateTime accept_time;
    QDateTime close_time;
    int duration;
    int status;
    QString user_data;
    in >> type >> call_id >> call_url >> status >> start_time >> accept_time
       >> close_time >> duration >> user_data;
    call = Call(0, type,status);
    call.setCallId(call_id);
    call.setUrl(call_url);
    call.setUserData(user_data);
    call.setStartTime(start_time);
    call.setAcceptTime(accept_time);
    call.setCloseTime(close_time);
    call.setDuration(duration);

    return in;
}
