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

#ifndef CALL_H
#define CALL_H

#include <QString>
#include <QDateTime>
#include <QVariantMap>
#include <QDataStream>

class PhoneApi;

/**
 * This class handles a specific call
 * it stores its data like type and status and it
 * contains methodes to comunicate with the phone-api
 */
class Call
{
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

public:
    /**
     * empty constructor only needed for error-logging
     */
    Call(){}

    /**
     * construct 
     * @param phone_api PhoneApi*, a reference to the phone_api
     * @param type int, which type the call is (where it is created)
     * @param status int, which status it has
     */
    Call(PhoneApi *phone_api, const int &type = Call::TYPE_UNKNOWN,
          const int &status = Call::STATUS_UNKNOWN);

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
     * Starting a SIP-Call to the stored address
     * @return The CallId of the started call
     **/
    int makeCall();

    /**
     * Answering the call
     **/
    void answerCall();

    /**
     * Hanging up the call
     **/
    void hangUp();

    /**
     * Combining the callees of two specific call.
     * @param call_dest int, CallID of the other callee.
     * @return true if success else false;
     **/
    bool addCallToConference(const Call &call_dest);

    /**
     * Remove the callee of the conference.
     * @param call_dest int, CallID of the other callee.
     * @return true if success else false;
     **/
    bool removeCallFromConference(const Call &call_dest);

    /**
     * Redirecting an active call to a new destination.
     * @param dest_uri string, SIP-Adress of the new Destination
     * @return int the success-code
     **/
    int redirectCall(const QString &dest_uri);

    /**
     * Get the sip-address
     * @return QString the sip-address
     **/
    const QString &getCallUrl() const;

    /**
     * Get the caller name or number
     * @return QString the name
     **/
    const QString &getCallName() const;

    /**
     * Get the call_id
     * @return int the call_id
     */
    const int &getCallId() const;

    /**
     * Get information about call like sip-adress, state
     * @param call_info QVariantMap, the object with the info to be written
     */
    void getCallInfo(QVariantMap &call_info);

    /**
     * Get call status
     * @return int call_status
     */
    const int &getStatus() const;

    /**
     * Get call type
     * @return int call_type
     */
    const int &getType() const;

    /**
     * Get start time of the call
     * @return QDateTime the date_obj of start_time
     */
    const QDateTime &getStartTime() const;

    /**
     * Get accept time of the call
     * @return QDateTime the date_obj of accept_time
     */
    const QDateTime &getAcceptTime() const;

    /**
     * Get close time of the call
     * @return QDateTime the date_obj of close_time
     */
    const QDateTime &getCloseTime() const;

    /**
     * Get duration of the call in seconds
     * @return int the duration
     */
    const int &getDuration() const;

    /**
     * Set the start time, only needed for error logging
     * @param start_time QDateTime, time_obj to set
     */
    void setStartTime(const QDateTime &start_time);

    /**
     * Set the accept time, only needed for error logging
     * @param accept_time QDateTime, time_obj to set
     */
    void setAcceptTime(const QDateTime &accept_time);

    /**
     * Set the close time, only needed for error logging
     * @param close_time QDateTime, time_obj to set
     */
    void setCloseTime(const QDateTime &close_time);

    /**
     * Set the duration, only needed for error logging
     * @param duration int, seconds to set
     */
    void setDuration(const int &duration);

    /**
     * Get custom user data
     * @return QString the user_data
     */
    QString getUserData() const;

    /**
     * Save custom user data
     * @param data QString the user_data
     */
    void setUserData(const QString &data);

    /**
     * Delete user_data
     */
    void clearUserData();

    /**
     * Checks if call is active
     */
    bool isActive() const;

    /**
     * Set SipUrl from the contact
     * @param url QString, the sip url
     */
    void setUrl(const QString &url);

    /**
     * Set SipName from the contact
     * @param name QString, the name
     */
    void setName(const QString &name);

    /**
     * Set call_id
     * @param call_id int, the call_id
     */
    void setCallId(const int &call_id);

    /**
     * set this call as active
     */
    void setCallActive();

    /**
     * set this call as inactive
     */
    void setCallInactive();

    /**
     * set current call_state
     * @param state int, the call_state
     */
    void setCallState(const int &state);

    /**
     * set current media_state
     * @param state int, the media_state
     */
    void setMediaState(const int &state);

    /**
     * Switch sound on/off
     * @param mute bool, true if callee should be muted
     */
    void muteSound(const bool &mute);

    /**
     * Switch microphone on/off
     * @param mute bool, true if microphone should be muted
     */
    void muteMicrophone(const bool &mute);

    /**
     * Get information about signal levels of sound and microphone
     * @param signal_info QVariantMap, a map to save the mute level
     */
    void getSignalInformation(QVariantMap &signal_info);
};
QDataStream &operator<<(QDataStream &out, const Call &call);
QDataStream &operator>>(QDataStream &in, Call &call);

#endif // CALL_H
