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

#include "../Sound.h"
#include "../LogInfo.h"
#include "../LogHandler.h"
#include "api/Interface.h"
#include "Phone.h"
#include "Call.h"

namespace phone
{

//-----------------------------------------------------------------------------
Call::Call(Phone *phone, const Type type, const Status status) :
    phone_(phone), 
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

    int id = phone_->getApi()->makeCall(url_);
    active_ = true;

    if (id < 0) {
        Sound::getInstance().stop();
    }
    call_id_ = id;
 
    return call_id_;
}

//-----------------------------------------------------------------------------
void Call::answerCall() const
{
    if (call_id_ != -1) {
        phone_->getApi()->answerCall(call_id_);
    }
}

//-----------------------------------------------------------------------------
void Call::hangUp()
{
    if (call_id_ != -1) {
        phone_->getApi()->hangUp(call_id_);
    }
    setInactive();
}

//-----------------------------------------------------------------------------
bool Call::addCallToConference(const Call &call_dest) const
{
    return phone_->getApi()->addCallToConference(call_id_, call_dest.getCallId());
}

//-----------------------------------------------------------------------------
bool Call::removeCallFromConference(const Call &call_dest) const
{
    return phone_->getApi()->removeCallFromConference(call_id_, call_dest.getCallId());
}

//-----------------------------------------------------------------------------
int Call::redirectCall(const QString &dest_uri) const
{
    return phone_->getApi()->redirectCall(call_id_, dest_uri);
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
void Call::getCallInfo(QVariantMap &call_info) const
{
    if (phone_) {
        phone_->getApi()->getCallInfo(call_id_, call_info);
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
const QString &Call::getUserData() const
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
    LogHandler::getInstance().logData(LogInfo(LogInfo::STATUS_DEBUG, "call", 0, "set call inactive"));

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
    speaker_level_ = mute ? 0.f : 1.f;
    phone_->getApi()->muteSound(call_id_, speaker_level_);
}

//-----------------------------------------------------------------------------
void Call::muteMicrophone(const bool mute)
{
    mic_level_ = mute ? 0.f : 1.f;
    phone_->getApi()->muteMicrophone(call_id_, mic_level_);
}

//-----------------------------------------------------------------------------
void Call::getSignalInformation(QVariantMap &signal_info) const
{
    signal_info.insert("sound", speaker_level_);
    signal_info.insert("micro", mic_level_);
}

} // phone::

//-----------------------------------------------------------------------------
// OVERLOADING QDataStream
//-----------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const phone::Call &call)
{
    out << call.getType() << call.getCallId() << call.getCallUrl()
        << call.getStatus() << call.getStartTime() << call.getAcceptTime()
        << call.getCloseTime() << call.getDuration() << call.getUserData();
    return out;
}

//-----------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, phone::Call &call)
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
    call = phone::Call(0, (phone::Call::Type)type, (phone::Call::Status)status);
    call.setCallId(call_id);
    call.setUrl(call_url);
    call.setUserData(user_data);
    call.setStartTime(start_time);
    call.setAcceptTime(accept_time);
    call.setCloseTime(close_time);
    call.setDuration(duration);

    return in;
}
