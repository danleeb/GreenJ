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

#ifndef PHONE_INCLUDE_H
#define PHONE_INCLUDE_H

#include <QObject>
#include <QVector>
#include <QVariantMap>


class PhoneApi;
class Account;
class Call;
class JavascriptHandler;


/**
 * Base phone class
 */
class Phone : public QObject
{
    Q_OBJECT

public:
    /**
     * Constuctor
     * @param api Pointer to the phone API
     */
    Phone(PhoneApi *api);
    virtual ~Phone();

    /**
     * Initializing the phone
     */
    void init(JavascriptHandler *js_handler);

    /**
     * Checks if account is valid
     * @return true, if account is valid
     */
    bool checkAccountStatus();

    /**
     * Registers the account
     * @param acc Account
     * @return true, if registration was successful
     */
    bool registerUser(const Account &acc);

    /**
     * Get information about the account
     * @param account_info Map to store account information
     */
    void getAccountInfo(QVariantMap &account_info);

    /**
     * Starting a call to the given address
     * @param url Destination address (e.g. "SIP:user@domain")
     * @return The ID of the new call
     */
    int makeCall(const QString &url);

    /**
     * Answering an incoming call
     * @param call_id ID of the incoming call
     */
    void answerCall(const int call_id = -1);

    /**
     * Hanging up a specific call
     * @param call_id int, The callID to hang up.
     */
    void hangUp(const int call_id);

    /**
     * Hanging up incoming and all active calls
     */
    void hangUpAll();

    /**
     * Get custom user data by call id
     * @param call_id ID of the call
     * @return Stored user data as a string
     */
    QString getCallUserData(const int call_id);

    /**
     * Set customer user data for call by call id
     * @param call_id ID of the call
     * @param Data that should be stored
     */
    void setCallUserData(const int call_id, const QString &data);

    /**
     * Deletes the custom user data of the call
     * @param call_id ID of the call
     */
    void clearCallUserData(const int call_id);

    /**
     * Connecting the callee of one specific call with another.
     * @param call_src CallID of the first callee.
     * @param call_dest CallID of the second callee.
     * @return true, if successful
     */
    bool addCallToConference(const int call_src, const int call_dest);

    /**
     * Remove one call from the conference
     * @param call_src CallID of the first callee.
     * @param call_dest CallID of the second callee.
     * @return true, if successful
     */
    bool removeCallFromConference(const int call_src, const int call_dest);

    /**
     * Redirecting an active call to a new destination.
     * @param call_id CallID of the call to be redirected.
     * @param dest_uri Address of the new destination
     * @return success code
     */
    int redirectCall(const int call_id, const QString &dest_uri);

    /**
     * Get the address of a given call
     * @param call_id ID of the call
     * @return address
     */
    QString getCallUrl(const int call_id);

    /**
     * Get information about a call
     * @param call_id ID of the call
     * @param call_info Map, to store information in
     */
    void getCallInfo(const int call_id, QVariantMap &call_info);

    /**
     * Get list of active calls
     * @param call_list List to store active calls in
     */
    void getActiveCallList(QVariantList &call_list);

    /**
     * Switch sound on/off
     * @param mute true, if call should be muted
     * @param call_id Optional id of a specific call
     */
    void muteSound(const bool mute, const int call_id = -1);

    /**
     * Switch microphone on/off
     * @param mute true, if microphone should be muted
     * @param call_id Optional id of a specific call
     */
    void muteMicrophone(const bool mute, const int call_id = -1);

    /**
     * Get information about signal levels of sound and microphone
     * @param signal_info Map to save sound and micro signal levels in
     */
    void getSignalInformation(QVariantMap &signal_info);

    /**
     * Hangs up all active calls, unregisters the account
     * and shuts down the phone
     */
    void unregister();

public slots:
    /**
     * This slot gets called on incoming calls
     * @param call Call*, the call handler class
     */
    void incomingCallSlot(Call *call);

    /**
     * This slot gets called when the state of a call has changed
     * @param call_id ID of call
     * @param call_state The new state
     * @param last_status The old state
     */
    void callStateSlot(int call_id, int call_state, int last_status);

    /**
     * This slot gets called when the sound level has changed
     * @param level The new sound level
     */
    void soundLevelSlot(int level);

    /**
     * This slot gets called when the microphone level has changed
     * @param level The new micro level
     */
    void microphoneLevelSlot(int level);

    /**
     * This slot gets called when the account registration state has changed
     * @param state The new account state
     */
    void accountRegState(const int state);

signals:
    void signalIncomingCall(const QString &call);

private:
    PhoneApi *phone_api_;
    JavascriptHandler *js_handler_;

    QVector<Call*> call_list_;

    bool addToCallList(Call *call);
    Call *getCallFromList(const int call_id);
};

#endif // PHONE_INCLUDE_H
