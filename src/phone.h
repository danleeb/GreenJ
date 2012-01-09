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

#ifndef PHONE_H
#define PHONE_H

#include <QObject>
#include <QVector>

#include "phone_api.h"
#include "log_info.h"

class Account;
class Gui;
class Call;
class JavascriptHandler;

/**
 * This is a wrapper class provide the communication between an
 * unknown voip-class and the application
 */
class Phone : public QObject
{
    Q_OBJECT

    PhoneApi *phone_api_;
    JavascriptHandler *js_handler_;

    bool addToCallList(Call *call);
    Call *getCallFromList(const int &call_id);

    /**
     * AccountID we got from our registration
     */

    QVector<Call*> call_list_;

public:
    /**
     * Constuctor of the class
     * @param api PhoneApi*, a reference to the voip-api
     */
    Phone(PhoneApi *api);
    ~Phone(void);

    /**
     * Initializing the SIP-API
     */
    void init(JavascriptHandler *js_handler);

    /**
     * checks if acc_id is valid or not
     * @return bool true if acc_id is valid
     */
    bool checkAccountStatus();

    /**
     * Registers the user at the given Asterisk server
     * @param acc Account, hte data needed to login
     * @return true if registering was successful
     * @return false if registering failed
     */
    bool registerUser(const Account &acc);

    /**
     * Get some information about account
     * @param &account_info QVariantMap, which holds account information
     */
    void getAccountInfo(QVariantMap &account_info);

    /**
     * Starting a SIP-Call to the given adress
     * @param url string, The SIP-Adress. E.g. "SIP:user@domain"
     * @return The CallId of the started call
     */
    int makeCall(const QString &url);

    /**
     * Answering an incoming call
     * @param call_id int, CallId of the incoming call
     */
    void answerCall(const int &call_id=-1);

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
     * Get user data by callid
     * @param call_id int, the id of the call
     * @return QString the stored data
     */
    QString getCallUserData(const int &call_id);

    /**
     * Set user data for call by callid
     * @param call_id int, the id of the call
     * @param data QString, the data to store
     */
    void setCallUserData(const int &call_id, const QString &data);

    /**
     * deletes user data of the call
     * @param call_id int, the id of the call
     */
    void clearCallUserData(const int &call_id);

    /**
     * Combining the callee of one specific call to an other.
     * It gets linked to it and hears and can speak to it
     * as long as we don't hang up or disconnect it.
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
     * @param dest_uri string, SIP-Adress of the new Destination
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
     * Get listof active calls
     * @param call_list QVariantMap, the object with the calls to be written
     */
    void getActiveCallList(QVariantList &call_list);

    /**
     * Switch sound on/off
     * @param mute bool, true if callee should be muted
     * @param call_id int, optional id of an specific call
     */
    void muteSound(const bool &mute, const int &call_id = -1);

    /**
     * Switch microphone on/off
     * @param mute bool, true if microphone should be muted
     * @param call_id int, optional id of an specific call
     */
    void muteMicrophone(const bool &mute, const int &call_id = -1);

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

public slots:
    /**
     * This slot get called when call arrived
     * @param call Call*, the call handler class
     */
    void incomingCallSlot(Call *call);

    /**
     * This slot get called when state of call changed
     * @param call_id int, the id of call
     * @param call_state int, the new state
     * @param last_status int, the old state
     */
    void callStateSlot(int call_id, int call_state, int last_status);

    /**
     * This slot get called when soundlevel changed
     * @param level int, the new sound level
     */
    void soundLevelSlot(int level);

    /**
     * This slot get called when microlevel changed
     * @param level int, the new micro level
     */
    void microphoneLevelSlot(int level);

    /**
     * This slot get called when account registration state get changed
     * @param state int, the new state of account
     */
    void accountRegState(const int &state);

signals:
    void signalIncomingCall(const QString &call);
};

#endif // PHONE_H
