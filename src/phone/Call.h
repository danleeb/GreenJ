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

#ifndef CALL_INCLUDE_H
#define CALL_INCLUDE_H

#include <QString>
#include <QDateTime>
#include <QVariantMap>
#include <QDataStream>

namespace phone
{

class Phone;

/**
 * This class handles a specific call.
 * Stores data like type and status and has methods to comunicate with the 
 * phone-api.
 */
class Call
{
public:
    /**
     * Call Type
     * Defines where the call is from
     */
    enum Type {
        TYPE_UNKNOWN = -1,
        TYPE_INCOMING,
        TYPE_OUTGOING
    };

    /**
     * Call Status
     * Defines the life cycle of the call
     */
    enum Status {
        STATUS_UNKNOWN = -1,
        STATUS_RINGING,
        STATUS_ACCEPTED,
        STATUS_CLOSED,
        STATUS_ERROR
    };

    /**
     * Empty constructor only needed for error-logging
     */
    Call() {}

    /**
     * Constructor
     * @param phone_ Parent Phone
     * @param type Call type
     * @param status Call status
     */
    Call(Phone *phone_, const Type type = TYPE_UNKNOWN,
         const Status status = STATUS_UNKNOWN);

    /**
     * Starts a SIP call to the stored address
     * @param url Destination address for the outgoing call
     * @return CallId of the new call
     */
    int makeCall(const QString &url);
    
    /**
     * Starts a SIP call to the stored address
     * @param url Destination address for the outgoing call
     * @param header_map Map of SIP header names and values.
     * @return CallId of the new call
     */
    int makeCall(const QString &url, const QVariantMap &header_map);

    /**
     * Answers the call
     */
    void answerCall(const int code = 200) const;

    /**
     * Hanging up the call
     */
    void hangUp();

    /**
     * Combining the callees of two calls.
     * @param call_dest CallID of the other callee.
     * @return true if successful
     */
    bool addToConference(const Call &call_dest) const;

    /**
     * Remove the callee from the conference.
     * @param call_dest CallID of the other callee.
     * @return true if successful
     */
    bool removeFromConference(const Call &call_dest) const;

    /**
     * Redirecting an active call to a new destination.
     * @param dest_uri SIP address of the new destination
     * @return success-code
     */
    int redirect(const QString &dest_uri) const;

    /**
     * Get information about call like SIP address, state, etc
     * @return Call information
     */
    QVariantMap getInfo() const;

    /**
     * Get the call ID
     * @return Call ID
     */
    const int getId() const;

    /**
     * Get the SIP address
     * @return SIP address
     */
    const QString &getUrl() const;

    /**
     * Get the caller name or number
     * @return the name
     */
    const QString &getName() const;

    /**
     * Get call status
     * @return call status
     */
    const int getStatus() const;

    /**
     * Get call type
     * @return calltype
     */
    const int getType() const;

    /**
     * Get start time of the call
     * @return date object
     */
    const QDateTime &getStartTime() const;

    /**
     * Get accept time of the call
     * @return date object
     */
    const QDateTime &getAcceptTime() const;

    /**
     * Get close time of the call
     * @return date object
     */
    const QDateTime &getCloseTime() const;

    /**
     * Get duration of the call in seconds
     * @return duration in seconds
     */
    const int getDuration() const;

    /**
     * Get custom user data
     * @return user data as a string
     */
    const QString &getUserData() const;

    /**
     * Save custom user data
     * @param data custom user data as a string
     */
    void setUserData(const QString &data);

    /**
     * Checks if call is active
     * @return true, if call is currently active
     */
    bool isActive() const;

    /**
     * Set SIP url of the contact
     * @param url sip url
     */
    void setUrl(const QString &url);

    /**
     * Set SIP name of the contact
     * @param name sip name
     */
    void setName(const QString &name);

    /**
     * Set call ID
     * @param call_id
     */
    void setId(const int call_id);

    /**
     * Set call as active
     */
    void setActive();

    /**
     * Set call as inactive
     */
    void setInactive();

    /**
     * Set current call state
     * @param state
     */
    void setState(const int state);

    /**
     * Set current media state
     * @param state media state
     */
    void setMediaState(const int state);

    /**
     * Set sound level
     * @param soundLevel 0.0f (mute) to 1.0f (full)
     */
    void setSoundSignal(const float soundLevel);

    /**
     * Set microphone level
     * @param microLevel 0.0f (mute) to 1.0f (full)
     */
    void setMicroSignal(const float microLevel);

    /**
     * Get information about signal levels of sound and microphone
     * @param Map with 'sound' and 'micro' signal levels (floats)
     */
    QVariantMap getSignalLevels() const;

    /**
     * Setters, only needed for error logging
     */
    void setStartTime(const QDateTime &start_time);
    void setAcceptTime(const QDateTime &accept_time);
    void setCloseTime(const QDateTime &close_time);
    void setDuration(const int duration);

    /**
     * Set SIP call headers
     * @param Map with call headers
     */
    void setHeaders(const QVariantMap &header_map);

    /**
     * Get SIP call headers
     * @param Map with call headers
     */
    QVariantMap getHeaders() const;

private:
    QDateTime start_time_;
    QDateTime accept_time_;
    QDateTime close_time_;
    int duration_;

    Phone *phone_;

    int type_;
    int status_;
    bool active_;
    int id_;
    int call_state_;
    int media_state_;

    QString url_;
    QString name_;
    QString user_data_;

    QVariantMap headers_;
};

} // phone::

QDataStream &operator<<(QDataStream &out, const phone::Call &call);
QDataStream &operator>>(QDataStream &in, phone::Call &call);

#endif // CALL_INCLUDE_H
