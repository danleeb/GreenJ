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

#ifndef SIP_PHONE_H
#define SIP_PHONE_H

#include "phone_api.h"

#include <pjsua-lib/pjsua.h>

#include <QVector>

#include "sound.h"

class Gui;
class Phone;

/**
 * This class is an implementation of PhoneApi for sip-Protocol
 */
class SipPhone : public PhoneApi
{
    Q_OBJECT

    private:
    /**
     * Pointer to the instance for using static methods
     */
    static SipPhone *self_;

    float speaker_level_;
    float mic_level_;

    /**
     * AccountID we got from our registration
     */
    pjsua_acc_id acc_id_;

    /**
     * Stop ringing
     * Setting the incoming_call_info_
     */
    void finishIncoming();

    /**
     * PJSIP-Callback
     */
    static void incomingCallCb(pjsua_acc_id acc_id, pjsua_call_id call_id,
                               pjsip_rx_data *rdata);

    /**
     * PJSIP-Callback
     */
    static void callStateCb(pjsua_call_id call_id, pjsip_event *e);

    /**
     * PJSIP-Callback
     */
    static void callMediaStateCb(pjsua_call_id call_id);

    /**
     * PJSIP-Callback, called when reg_state changes
     * @param acc pjsua_acc_id, the id of account which changes
     */
    static void regStateCb(pjsua_acc_id acc);


public:
    SipPhone();
    ~SipPhone(void);

    /**
     * Initializing the SIP-API
     */
    void init();

    /**
     * checks if acc_id is valid or not
     * @return bool true if acc_id is valid
     */
    bool checkAccountStatus();

    /**
     * Registers the user at the given Asterisk server
     * @param acc Account, the object with user data
     * @return bool true if registering was successful
     *         false if registering failed
     */
    int registerUser(const Account &acc);

    /**
     * Get some information about account
     * @param &account_info QVariantMap, which holds account information
     */
    void getAccountInfo(QVariantMap &account_info);

    /**
     * Starting a SIP-Call to the given address
     * @param url string, The SIP-Adress. E.g. "SIP:user@domain"
     * @return int The CallId of the started call
     */
    int makeCall(const QString &url);

    /**
     * Answering an incoming call
     * @param call_id int, CallId of the incoming call
     * If call_id is not set, the method takes the IncomingCall struct
     * for answering the call
     */
    void answerCall(int call_id=-1);

    /**
     * Hanging up a specific call
     * @param call_id int, The callID to hang up.
     */
    void hangUp(const int &call_id);

    /**
     * Hanging up incoming and all active calls
     */
    void hangUpAll();

    /**
     * Combining the callees of two specific calls.
     * They get linked together and hear and can speak to each other
     * as long as we don't hang up.
     * @param call_src int, CallID of the first callee.
     * @param call_dest int, CallID of the second callee.
     * @return bool true if success else false;
     */
    bool addCallToConference(const int &call_src, const int &call_dest);

    /**
     * Remove one call of a conference
     * @param call_src int, CallID of the first callee.
     * @param call_dest int, CallID of the second callee.
     * @return bool true if success else false;
     */
    bool removeCallFromConference(const int &call_src, const int &call_dest);

    /**
     * Redirecting an active call to a new destination.
     * @param call_id int, The CallID of the call to be redirected.
     * @param destUri_ string, SIP-Adress of the new Destination
     * @return int the success-code
     */
    int redirectCall(const int &call_id, const QString &dest_uri);

    /**
     * Get the sip-address of a given call
     * @param call_id int, the id of the call
     * @return QString the sip-address
     */
    QString getCallUrl(const int &call_id);

    /**
     * Get information about call like sip-adress, state
     * @param call_id int, the id of the call
     * @param call_info QVariantMap, the object with the info to be written
     */
    void getCallInfo(const int &call_id, QVariantMap &call_info);

    /**
     * Switch sound on/off
     * @param mute bool, true if callee should be muted
     */
    void muteSound(const bool &mute);

    /**
     * Adjust level of speaker
     * @param call_id int, CallId of the call
     * @param mute float, mute level
     */
    void muteSoundForCall(const int &call_id, const float &mute);

    /**
     * Switch microphone on/off
     * @param mute bool, true if microphone should be muted
     */
    void muteMicrophone(const bool &mute);

    /**
     * Adjust level of microphone
     * @param call_id int, CallId of the call
     * @param mute float, mute level
     */
    void muteMicrophoneForCall(const int &call_id, const float &mute);

    /**
     * Get information about signal levels of sound and microphone
     * @param signal_info QVariantMap, a map to save the mute level
     */
    void getSignalInformation(QVariantMap &signal_info);

    /**
     * Hanging up all active calls,
     * Unregistering the user
     * and destroying the pjsip instance
     */
    void unregister();
};

#endif // SIP_PHONE_H
