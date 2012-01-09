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

#ifndef PHONE_API_H
#define PHONE_API_H

#include <QObject>
#include <QString>
#include <QVariantMap>

#include "log_info.h"

class Account;
class Call;

/**
 * This class is implemented as interface.
 * It defines methodes a voip-api has to implement
 */
class PhoneApi : public QObject
{
    Q_OBJECT

public:
    virtual ~PhoneApi(){}

    /**
     * Initializing the the api
     */
    virtual void init() = 0;

    /**
     * checks if acc_id is valid or not
     * @return bool true if acc_id is valid
     */
    virtual bool checkAccountStatus() = 0;

    /**
     * Registers the user at the given Asterisk server
     * @param acc Account, the data needed to login
     * @return bool true if registering was successful false if registering failed
     */
    virtual int registerUser(const Account &acc) = 0;

    /**
     * Get some information about account
     * @param &account_info QVariantMap, which holds account information
     */
    virtual void getAccountInfo(QVariantMap &account_info) = 0;

    /**
     * Starting a SIP-Call to the given adress
     * @param url string, The SIP-Adress. E.g. "SIP:user@domain"
     * @return int The CallId of the started call
     */
    virtual int makeCall(const QString &url) = 0;

    /**
     * Answering an incoming call
     * @param call_id int, CallId of the incoming call
     */
    virtual void answerCall(int call_id=-1) = 0;

    /**
     * Hanging up a specific call
     * @param call_id int, The callID to hang up.
     */
    virtual void hangUp(const int &call_id) = 0;

    /**
     * Hanging up incoming and all active calls
     */
    virtual void hangUpAll() = 0;

    /**
     * Combining the callee of one specific call to an other.
     * It gets linked to it and hears and can speak to it
     * as long as we don't hang up or disconnect it.
     * @param call_src int, CallID of the first callee.
     * @param call_dest int, CallID of the second callee.
     * @return bool true if success else false;
     */
    virtual bool addCallToConference(const int &call_src, const int &call_dest) = 0;

    /**
     * Remove one call of a conference
     * @param call_src int, CallID of the first callee.
     * @param call_dest int, CallID of the second callee.
     * @return bool true if success else false;
     */
    virtual bool removeCallFromConference(const int &call_src, const int &call_dest) = 0;

    /**
     * Redirecting an active call to a new destination.
     * @param call_id int, The CallID of the call to be redirected.
     * @param destUri_ string, SIP-Adress of the new Destination
     * @return int the success-code
     */
    virtual int redirectCall(const int &call_id, const QString &dest_uri) = 0;

    /**
     * Get information about call like adress, state
     * @param call_id int, the id of the call
     * @param call_info QVariantMap, the object with the info to be written
     */
    virtual void getCallInfo(const int &call_id, QVariantMap &call_info) = 0;

    /**
     * Switch sound on/off
     * @param mute bool, true if callee should be muted
     */
    virtual void muteSound(const bool &mute) = 0;

    /**
     * Adjust level of speaker
     * @param call_id int, CallId of the call
     * @param mute float, mute level
     */
    virtual void muteSoundForCall(const int &call_id, const float &mute) = 0;

    /**
     * Switch microphone on/off
     * @param mute bool, true if microphone should be muted
     */
    virtual void muteMicrophone(const bool &mute) = 0;

    /**
     * Adjust level of microphone
     * @param call_id int, CallId of the call
     * @param mute float, mute level
     */
    virtual void muteMicrophoneForCall(const int &call_id, const float &mute) = 0;

    /**
     * Get information about signal levels of sound and microphone
     * @param signal_info QVariantMap, a map to save the mute level
     */
    virtual void getSignalInformation(QVariantMap &signal_info) = 0;

    /**
     * Hanging up all active calls,
     * Unregistering the user
     */
    virtual void unregister() = 0;

signals:

    /**
     * Send signal when account status changed
     * @param state int, the new state of the account
     */
    void signalAccountRegState(const int &state);
    /**
     * Send a signal when someone tries to call
     * @param call_id int, id of incoming call
     * @param callUrl QString, the name of caller
     */
    void signalIncomingCall(Call *call);

    /**
     * Send signal on changing call state
     * @param call_id int, id of call that changes callstate
     * @param state int, new call state
     */
    void signalCallState(int call_id, int state, int last_status);

    /**
     * Send signal to handle log_data
     * @param LogInfo the log_data
     */
    void signalLogData(const LogInfo&);

    /**
     * Send a signal when the sound signal level changed
     * @param int level new sound signal level (0...off, 255...full)
     */
    void signalSoundLevel(int level);

    /**
     * Send a signal when the microphone signal level changed
     * @param int level new microphone signal level (0...off, 255...full)
     */
    void signalMicrophoneLevel(int level);

};

#endif // PHONE_API_H
