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
    id_(-1), call_state_(0), media_state_(0), 
    start_time_(QDateTime::currentDateTime())
{
}

//-----------------------------------------------------------------------------
int Call::makeCall(const QString &url)
{
    url_ = url;
    id_ = phone_->getApi()->makeCall(url_);
    active_ = true;

    if (id_ >= 0) {
        Sound::getInstance().startDial();
    }
    return id_;
}
    
//-----------------------------------------------------------------------------
int Call::makeCall(const QString &url, const QVariantMap &header_map)
{
    url_ = url;
    id_ = phone_->getApi()->makeCall(url_, header_map);
    active_ = true;
    
    if (id_ >= 0) {
        Sound::getInstance().startDial();
    }
    return id_;
}

//-----------------------------------------------------------------------------
void Call::answerCall() const
{
    if (id_ != -1) {
        phone_->getApi()->answerCall(id_);
    }
}

//-----------------------------------------------------------------------------
void Call::hangUp()
{
    if (id_ != -1) {
        phone_->getApi()->hangUp(id_);
    }
    setInactive();
}

//-----------------------------------------------------------------------------
bool Call::addToConference(const Call &call_dest) const
{
    return phone_->getApi()->addCallToConference(id_, call_dest.getId());
}

//-----------------------------------------------------------------------------
bool Call::removeFromConference(const Call &call_dest) const
{
    return phone_->getApi()->removeCallFromConference(id_, call_dest.getId());
}

//-----------------------------------------------------------------------------
int Call::redirect(const QString &dest_uri) const
{
    return phone_->getApi()->redirectCall(id_, dest_uri);
}

//-----------------------------------------------------------------------------
QVariantMap Call::getInfo() const
{
    QVariantMap info;
    if (phone_) {
        phone_->getApi()->getCallInfo(id_, info);
    } else {
        info.insert("number", url_);
        info.insert("duration", duration_);
    }
    info.insert("name", name_);
    info.insert("type", (int)type_);
    info.insert("status", (int)status_);
    info.insert("callTime", (qint64)start_time_.toTime_t() * 1000 + (qint64)start_time_.time().msec());
    info.insert("acceptTime", (qint64)accept_time_.toTime_t() * 1000 + (qint64)accept_time_.time().msec());
    info.insert("closeTime", (qint64)close_time_.toTime_t() * 1000 + (qint64)close_time_.time().msec());
    info.insert("userData", user_data_);
    return info;
}

//-----------------------------------------------------------------------------
const int Call::getId() const
{
    return id_;
}

//-----------------------------------------------------------------------------
const QString &Call::getUrl() const
{
    return url_;
}

//-----------------------------------------------------------------------------
const QString &Call::getName() const
{
    return name_;
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
void Call::setId(const int call_id)
{
    id_ = call_id;
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
    LogHandler::getInstance().log(LogInfo(LogInfo::STATUS_DEBUG, "call", 0, "set call inactive"));

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
void Call::setSoundSignal(const float soundLevel)
{
    phone_->getApi()->setSoundSignal(soundLevel, id_);
}

//-----------------------------------------------------------------------------
void Call::setMicroSignal(const float microLevel)
{
    phone_->getApi()->setMicroSignal(microLevel, id_);
}

//-----------------------------------------------------------------------------
QVariantMap Call::getSignalLevels() const
{
    QVariantMap info;
    phone_->getApi()->getSignalLevels(info, id_);
    return info;
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
QVariantMap Call::getHeaders() const
{
    return headers_;
}

//-----------------------------------------------------------------------------
void Call::setHeaders(const QVariantMap &header_map)
{
    headers_ = header_map;
}

} // phone::

//-----------------------------------------------------------------------------
// OVERLOADING QDataStream
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QDataStream &operator<<(QDataStream &out, const phone::Call &call)
{
    out << call.getType() << call.getId() << call.getUrl()
        << call.getStatus() << call.getStartTime() << call.getAcceptTime()
        << call.getCloseTime() << call.getDuration() << call.getUserData();
    return out;
}

//-----------------------------------------------------------------------------
QDataStream &operator>>(QDataStream &in, phone::Call &call)
{
    int type, call_id, duration, status;
    QString call_url, user_data;
    QDateTime start_time, accept_time, close_time;

    in >> type >> call_id >> call_url >> status >> start_time >> accept_time
       >> close_time >> duration >> user_data;
    
    call = phone::Call(0, (phone::Call::Type)type, (phone::Call::Status)status);
    call.setId(call_id);
    call.setUrl(call_url);
    call.setUserData(user_data);
    call.setStartTime(start_time);
    call.setAcceptTime(accept_time);
    call.setCloseTime(close_time);
    call.setDuration(duration);

    return in;
}
