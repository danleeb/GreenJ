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

class Config;
class JavascriptHandler;
class LogInfo;

namespace phone
{
    class Account;
    class Call;

    namespace api {
        class Interface;
    }

/**
 * Base phone class
 */
class Phone : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param api Pointer to the phone API
     */
    Phone(api::Interface *api);

    /**
     * Destructor
     */
    virtual ~Phone();

    /**
     * Initialize the phone
     */
    bool init();
    
    /**
     * Set the javascript handler object
     */
    void setJavascriptHandler(JavascriptHandler *js_handler);

    /**
     * @return Phone API implementation
     */
    api::Interface *getApi();

    /**
     * Return last error message
     * @return error message
     */
    const QString &getErrorMessage() const;

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
     * @return Map with account information
     */
    QVariantMap getAccountInfo();

    /**
     * Starting a call to the given address
     * @param url Destination address (e.g. "SIP:user@domain")
     * @return New Call object or NULL if unsuccessful
     */
    Call *makeCall(const QString &url);

    /**
     * Hanging up incoming and all active calls
     */
    void hangUpAll();

    /**
     * Return call object via id
     * @param call_id Call ID
     * @return Call object
     */
    Call *getCall(const int call_id);

    /**
     * Get list of active calls
     * @return List of active calls
     */
    QVariantList getActiveCallList() const;

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
     * @param Map with sound and micro signal levels
     */
    QVariantMap getSignalInformation() const;

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
    void slotIncomingCall(int call_id, const QString &url, const QString &name);

    /**
     * This slot gets called when the state of a call has changed
     * @param call_id ID of call
     * @param call_state The new state
     * @param last_status The old state
     */
    void slotCallState(int call_id, int call_state, int last_status);

    /**
     * This slot gets called when the sound level has changed
     * @param level The new sound level
     */
    void slotSoundLevel(int level);

    /**
     * This slot gets called when the microphone level has changed
     * @param level The new micro level
     */
    void slotMicrophoneLevel(int level);

    /**
     * This slot gets called when the account registration state has changed
     * @param state The new account state
     */
    void slotAccountRegState(const int state);

    /**
     * Slot to log messages
     * @param info
     */
    void slotLogData(const LogInfo &info);

    /**
     * Slot to start ring sound
     */
    void slotRingSound();
    
    /**
     * Slot to stop sounds
     */
    void slotStopSound();

signals:
    void signalIncomingCall(const QString &call);

private:
    api::Interface *api_;
    JavascriptHandler *js_handler_;

    QVector<Call*> call_list_;

    QString error_msg_;

    bool addToCallList(Call *call);
};

} // phone::

#endif // PHONE_INCLUDE_H
