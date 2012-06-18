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

#ifndef PHONEAPI_INCLUDE_H
#define PHONEAPI_INCLUDE_H

#include <QObject>
#include <QString>
#include <QVariantMap>

class LogInfo;

namespace phone
{
    class Settings;

    namespace api
    {

/**
 * Interface for phone implementations.
 */
class Interface : public QObject
{
    Q_OBJECT

public:
    /**
     * Initializing the phone
     */
    virtual bool init(const Settings &settings) = 0;

    /**
     * Registers the account
     * @param user
     * @param password
     * @param domain
     * @return Account id or -1 if unsuccessful
     */
    virtual int registerUser(const QString &user, const QString &password, const QString &domain) = 0;

    /**
     * Checks if account is valid
     * @return true, if account is valid
     */
    virtual bool checkAccountStatus() = 0;

    /**
     * Hangs up all active calls, unregisters the account
     * and shuts down the phone
     */
    virtual void unregister() = 0;

    /**
     * Get information about the account
     * @param account_info Map to store account information
     */
    virtual void getAccountInfo(QVariantMap &account_info) = 0;

    /**
     * Starting a call to the given address
     * @param url Destination address (e.g. "SIP:user@domain")
     * @return The ID of the new call
     */
    virtual int makeCall(const QString &url) = 0;

    /**
     * Answering an incoming call
     * @param call_id ID of the incoming call
     */
    virtual void answerCall(int call_id = -1) = 0;

    /**
     * Hanging up a specific call
     * @param call_id int, The callID to hang up.
     */
    virtual void hangUp(const int call_id) = 0;

    /**
     * Hanging up incoming and all active calls
     */
    virtual void hangUpAll() = 0;

    /**
     * Connecting the callee of one specific call with another.
     * @param call_src CallID of the first callee.
     * @param call_dest CallID of the second callee.
     * @return true, if successful
     */
    virtual bool addCallToConference(const int call_src, const int call_dest) = 0;

    /**
     * Remove one call from the conference
     * @param call_src CallID of the first callee.
     * @param call_dest CallID of the second callee.
     * @return true, if successful
     */
    virtual bool removeCallFromConference(const int call_src, const int call_dest) = 0;

    /**
     * Redirecting an active call to a new destination.
     * @param call_id CallID of the call to be redirected.
     * @param dest_uri Address of the new destination
     * @return success code
     */
    virtual int redirectCall(const int call_id, const QString &dest_uri) = 0;

    /**
     * Get information about a call
     * @param call_id ID of the call
     * @param call_info Map, to store information in
     */
    virtual void getCallInfo(const int call_id, QVariantMap &call_info) = 0;

    /**
     * Set sound level
     * @param soundLevel 0.0f (mute) to 1.0f (full)
     * @param call_id ID of a specific call, or -1 for general soundLevel
     */
    virtual void setSoundSignal(const float soundLevel, const int call_id = -1) = 0;
    
    /**
     * Switch microphone on/off
     * @param microLevel 0.0f (mute) to 1.0f (full)
     * @param call_id ID of a specific call, or -1 for general microLevel
     */
    virtual void setMicroSignal(const float microLevel, const int call_id = -1) = 0;
    
    /**
     * Get information about signal levels of sound and microphone (0...255)
     * @param signal_info Map to save sound and micro signal levels in
     * @param call_id ID of a specific call, or -1 for general signal levels
     */
    virtual void getSignalLevels(QVariantMap &levels, const int call_id = -1) = 0;

    /**
     * Set priority of codec to new_priority
     * @param codec Name of Codec
     * @param new_priority Range 0...PJMEDIA_CODEC_PRIO_HIGHEST
     */
    virtual void setCodecPriority(const QString &codec, int new_priority) = 0;

    /**
     * Return all codecs and priorities
     * @param codecs Map of codecs with priorities
     */
    virtual void getCodecPriorities(QVariantMap &codecs) = 0;

signals:
    /**
     * Send signal when account status changed
     * @param state The new state of the account
     */
    void signalAccountState(const int state);

    /**
     * Send signal on incoming calls
     * @param call Incoming call object
     */
    void signalIncomingCall(int call_id, const QString &url, const QString &name);

    /**
     * Send signal on changing call state
     * @param call_id ID of the call
     * @param state New call state
     * @param state Old call state
     */
    void signalCallState(int call_id, int state, int last_status);

    /**
     * Send signal to handle log data
     * @param LogInfo The log data
     */
    void signalLog(const LogInfo&);

    /**
     * Send a signal when the sound signal level changed
     * @param level New sound signal level (0...off, 255...full)
     */
    void signalSoundLevel(int level);

    /**
     * Send a signal when the microphone signal level changed
     * @param level New microphone signal level (0...off, 255...full)
     */
    void signalMicroLevel(int level);
    
    /**
     * Send signal to stop sounds
     */
    void signalRingSound();

    /**
     * Send signal to stop sounds
     */
    void signalStopSound();
};

}} // phone::api::

#endif // PHONEAPI_INCLUDE_H
