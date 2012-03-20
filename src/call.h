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

class PhoneApi;

/**
 * This class handles a specific call.
 * Stores data like type and status and has methods to comunicate with the 
 * phone-api.
 */
class Call
{
public:
    /**
     * Empty constructor only needed for error-logging
     */
    Call() {}

    /**
     * Constructor
     * @param phone_api A reference to the phone_api
     * @param type Which type the call is (where it is created)
     * @param status Call status
     */
    Call(PhoneApi *phone_api, const int type = Call::TYPE_UNKNOWN,
         const int status = Call::STATUS_UNKNOWN);

    /**
     * \name Call Type
     * TYPE defines where call is from
     * \{
     */
    /**
     * TYPE is unknown
     */
    static const int TYPE_UNKNOWN;
    /**
     * TYPE is incoming
     */
    static const int TYPE_INCOMING;
    /**
     * TYPE is outgoing
     */
    static const int TYPE_OUTGOING;
    /**
     * \}
     */

    /**
     * \name Call Status
     * STATUS defines the life cycle of the call
     * \{
     */
    /**
     * STATUS is unknown
     */
    static const int STATUS_UNKNOWN;
    /**
     * STATUS is ringing
     */
    static const int STATUS_RINGING;
    /**
     * STATUS is accepted
     */
    static const int STATUS_ACCEPTED;
    /**
     * STATUS is closed
     */
    static const int STATUS_CLOSED;
    /**
     * STATUS is error
     */
    static const int STATUS_ERROR;
    /**
     * \}
     */

    /**
     * Starts a SIP call to the stored address
     * @return CallId of the new call
     */
    int makeCall();

    /**
     * Answers the call
     */
    void answerCall();

    /**
     * Hanging up the call
     */
    void hangUp();

    /**
     * Combining the callees of two calls.
     * @param call_dest CallID of the other callee.
     * @return true if successful
     */
    bool addCallToConference(const Call &call_dest);

    /**
     * Remove the callee from the conference.
     * @param call_dest CallID of the other callee.
     * @return true if successful
     */
    bool removeCallFromConference(const Call &call_dest);

    /**
     * Redirecting an active call to a new destination.
     * @param dest_uri SIP address of the new destination
     * @return success-code
     */
    int redirectCall(const QString &dest_uri);

    /**
     * Get the SIP address
     * @return SIP address
     */
    const QString &getCallUrl() const;

    /**
     * Get the caller name or number
     * @return the name
     */
    const QString &getCallName() const;

    /**
     * Get the CallID
     * @return CallID
     */
    const int getCallId() const;

    /**
     * Get information about call like SIP address, state, etc
     * @param call_info Object to store call information
     */
    void getCallInfo(QVariantMap &call_info);

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
     * Set the start time, only needed for error logging
     * @param start_time time to set
     */
    void setStartTime(const QDateTime &start_time);

    /**
     * Set the accept time, only needed for error logging
     * @param accept_time time to set
     */
    void setAcceptTime(const QDateTime &accept_time);

    /**
     * Set the close time, only needed for error logging
     * @param close_time time to set
     */
    void setCloseTime(const QDateTime &close_time);

    /**
     * Set the duration, only needed for error logging
     * @param duration seconds
     */
    void setDuration(const int duration);

    /**
     * Get custom user data
     * @return user data as a string
     */
    QString getUserData() const;

    /**
     * Save custom user data
     * @param data custom user data as a string
     */
    void setUserData(const QString &data);

    /**
     * Delete custom user data
     */
    void clearUserData();

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
     * Set CallID
     * @param call_id
     */
    void setCallId(const int call_id);

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
     * Switch sound on/off
     * @param mute true, if callee should be muted
     */
    void muteSound(const bool mute);

    /**
     * Switch microphone on/off
     * @param mute true, if microphone should be muted
     */
    void muteMicrophone(const bool mute);

    /**
     * Get information about signal levels of sound and microphone
     * @param signal_info Map to store current sound and micro signal levels
     */
    void getSignalInformation(QVariantMap &signal_info);

private:
    QDateTime start_time_;
    QDateTime accept_time_;
    QDateTime close_time_;
    int duration_;

    PhoneApi *phone_api_;

    int type_;
    int status_;
    bool active_;
    int call_id_;
    int call_state_;
    int media_state_;
    float speaker_level_;
    float mic_level_;

    QString url_;
    QString name_;
    QString user_data_;
};

QDataStream &operator<<(QDataStream &out, const Call &call);
QDataStream &operator>>(QDataStream &in, Call &call);

#endif // CALL_INCLUDE_H
