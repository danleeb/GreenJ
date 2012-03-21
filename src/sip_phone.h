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

#ifndef SIPPHONE_INCLUDE_H
#define SIPPHONE_INCLUDE_H

#include <pjsua-lib/pjsua.h>
#include <QVector>
#include "phone_api.h"

class Gui;
class Phone;

/**
 * This class is an implementation of PhoneApi for sip-Protocol
 */
class SipPhone : public PhoneApi
{
    Q_OBJECT

public:
    SipPhone();
    ~SipPhone();

    virtual void init();

    virtual bool checkAccountStatus();
    virtual int registerUser(const Account &acc);
    virtual void getAccountInfo(QVariantMap &account_info);

    virtual int makeCall(const QString &url);
    virtual void answerCall(int call_id = -1);
    virtual void hangUp(const int call_id);
    virtual void hangUpAll();

    virtual bool addCallToConference(const int call_src, const int call_dest);
    virtual bool removeCallFromConference(const int call_src, const int call_dest);

    virtual int redirectCall(const int call_id, const QString &dest_uri);

    virtual void getCallInfo(const int call_id, QVariantMap &call_info);

    virtual void muteSound(const bool mute);
    virtual void muteSoundForCall(const int call_id, const float mute);
    virtual void muteMicrophone(const bool mute);
    virtual void muteMicrophoneForCall(const int call_id, const float mute);
    virtual void getSignalInformation(QVariantMap &signal_info);

    virtual void unregister();
    
    /**
     * Get the sip-address of a given call
     * @param call_id int, the id of the call
     * @return QString the sip-address
     */
    QString getCallUrl(const int call_id);
    
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
     * PJSIP callback for incoming calls
     * @param acc_id The id of the account receiving the incoming call
     * @param call_id The id of the new incoming call
     * @param rdata Incoming message buffer
     */
    static void incomingCallCb(pjsua_acc_id acc_id, pjsua_call_id call_id,
                               pjsip_rx_data *rdata);

    /**
     * PJSIP callback for call state changes
     * @param call_id The id of the call which changed
     * @param e PJSIP event
     */
    static void callStateCb(pjsua_call_id call_id, pjsip_event *e);

    /**
     * PJSIP callback for media state changes
     * @param call_id The id of the call which changed
     */
    static void callMediaStateCb(pjsua_call_id call_id);

    /**
     * PJSIP callback for reg_state changes
     * @param acc The id of the account which changed
     */
    static void regStateCb(pjsua_acc_id acc);
};

#endif // SIPPHONE_INCLUDE_H
