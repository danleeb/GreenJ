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

#ifndef JAVASCRIPT_HANDLER_H
#define JAVASCRIPT_HANDLER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QWebView>
#include <QUrl>

class Phone;
class PrintHandler;
class LogInfo;
class Call;

/**
 * This class should be the bridge between Qt and website-javascript
 */
class JavascriptHandler : public QObject
{
    Q_OBJECT

    private:
    Phone &phone_;
    PrintHandler *print_handler_;
    QWebView *web_view_;
    QString js_class_handler_;

    /**
     * this function do the communication with website-javascript
     * @param func QString, the name of the function to be called
     */
    QVariant callJavascriptFunc(const QString &func);

public:
    /**
     * Constructor
     * @param phone Phone&, to access the phone-function 
     */
    JavascriptHandler(Phone &phone);
    ~JavascriptHandler(){}

    /**
     * Initmethode, because web_view can't be set in Constructor
     * @param web_view QWebView*, needed to call the Js-funcion
     */
    void init(QWebView *web_view, PrintHandler *print_handler);

    /**
     * Gets a status code by the Phone::callbackCallState
     * @param call_id int, call ID
     */
    void accountState(const int &state);

    /**
     * Gets a status code by the Phone::callbackCallState
     * @param call_id int, call ID
     * @param code int, status code
     */
    void callState(const int &call_id, const int &code, const int &last_status);

    /**
     * function should handle an incoming call
     * @param number QString, Phonenumber or name from incoming call
     */
    void incomingCall(const Call &call);

    /**
     * Ask Js for the url to the print page
     * @return QUrl the url to the print page
     */
    QUrl getPrintPage();

public slots:

    /**
     * allows to register an individual callback-handler
     * @param class_name QString, the name of the handler to register
     */
    int registerJsCallbackHandler(const QString &class_name);

    /**
     * get the account status
     * @return bool true if account is just registered
     */
    bool checkAccountStatus();

    /**
     * get information about account
     * @return QVariantList object with information
     */
    QVariantMap getAccountInformation();

    /**
     * register client to server
     * @param host QString, address of server
     * @param user_name QString, login name
     * @param password QString, password to login
     * @return bool on success true
     */
    bool registerToServer(QString host, QString user_name, QString password);

    /**
     * unregister client from server
     */
    void unregisterFromServer();

    /**
     * starts a call
     * @param number QString, Phonenumber or name to call
     * @return The ID of the call
     */
    int makeCall(const QString &number);

    /**
     * accept the call with given id
     * @param call_id int, id of the call to accept
     */
    void callAccept(const int &call_id);

    /**
     * finish call with given id
     * @param call_id int, id of the call to end
     */
    void hangup(const int &call_id);

    /**
     * finish all running calls
     */
    void hangupAll();

    /**
     * Set the LogLevel
     */
    void setLogLevel(const unsigned &log_level);

    /**
     * get stored data to specific call
     * @param call_id int, the id of the call
     * @return QString the data
     */
    QString getCallUserData(const int &call_id);

    /**
     * store data to a call
     * @param call_id int, the if of the call
     * @param data QString, the data to store
     */
    void setCallUserData(const int &call_id, QString data);

    /**
     * get a list of all open calls
     */
    QVariantList getErrorLogData();

    /**
     * delete error-log file
     */
    void deleteErrorLogFile();

    /**
     * adds a call to a conference
     *
     * @param src_id int, id of call to add to conference
     * @param dst_id int, id of conference
     * @return bool true on success
     */
    bool addToConference(const int &src_id, const int &dst_id);

    /**
     * remove a call from a conference
     *
     * @param src_id int, id of call to remove from conference
     * @param dst_id int, id of conference
     * @return bool true on success
     */
    bool removeFromConference(const int &src_id, const int &dst_id);

    /**
     * Redirect an active call to a new destination
     * @param call_id int, id of the call to be redirected
     * @param dst_url QString, the Number or address of the new destination
     * @return int the success-code
     */
    int redirectCall(const int &call_id, const QString dst_url);

    /**
     * Get all active calls
     * @return QVariant
     */
    QVariantList getActiveCallList();

    /**
     * Switch sound on/off
     * @param mute bool, true if call should be muted
     */
    void muteSound(const bool &mute, const int &call_id = -1);

    /**
     * Switch microphone on/off
     * @param mute bool, true if call should be muted
     */
    void muteMicrophone(const bool &mute, const int &call_id = -1);

    /**
     * Get information about signal levels
     * @return QVariantMap, current signal level for sound and microphone
     */
    QVariantMap getSignalInformation();

    /**
     * get data of an option
     * @param name QString, the name of the option
     * @return QString the option data
     */
    QVariant getOption(const QString &name);

    /**
     * set new data to an option
     * @param name QString, the name of the option
     * @param option QString, the new data of the option
     */
    void setOption(const QString &name, const QVariant &option);

    /**
     * tell qt to print a page given by url
     * @param url_str QString the url of the page to print
     */
    void printPage(const QString &url_str);

    /**
     * Get log message from js and send it to the log_handler
     * @param QVariantMap log, the log-object
     */
    bool sendLogMessage(QVariantMap log);

    /**
     * Slot to catch sended signals
     * @params call_id int, id of incoming call
     * @params call_url QString, number of incoming call
     */
    void incomingCallSlot(const Call &call);

    /**
     * Should print errors, warning and debug messages
     * @param info LogInfo, log information and message
     */
    void logMessageSlot(const LogInfo &info);

    /**
     * Sound level has changed
     * @param int level new sound level
     */
    void soundLevelSlot(int level);

    /**
     * Microphone level has changed
     * @param int level new microphone level
     */
    void microphoneLevelSlot(int level);

    QStringList getLogFileList();
    QString getLogFileContent(const QString &file_name);
    void deleteLogFile(const QString &file_name);
};

#endif // JAVASCRIPT_HANDLER_H
