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

#ifndef JAVASCRIPTHANDLER_INCLUDE_H
#define JAVASCRIPTHANDLER_INCLUDE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QUrl>
#include <QWebView>

class Phone;
class PrintHandler;
class LogInfo;
class Call;
class QWebView;

/**
 * This class is the bridge between Qt and website-javascript
 */
class JavascriptHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param phone To access the phone methods
     */
    JavascriptHandler(Phone &phone);

    /**
     * Init method, because web_view can't be set in constructor
     * @param web_view WebView needed to call the JS functions
     * @param print_handler
     */
    void init(QWebView *web_view, PrintHandler *print_handler);

    /**
     * Send current account state
     * @param state
     */
    void accountState(const int state);

    /**
     * Send call state
     * @param call_id
     * @param code status code
     * @param last_status last status code
     */
    void callState(const int call_id, const int code, const int last_status);

    /**
     * Notify about an incoming call
     * @param call
     */
    void incomingCall(const Call &call);

    /**
     * Request the url of a page to print
     * @return The url of the webpage that should be printed
     */
    QUrl getPrintPage();

public slots:
    /**
     * Register an individual callback-handler by name
     * @param class_name The name of the handler to register
     */
    int registerJsCallbackHandler(const QString &class_name);

    /**
     * Get the account status
     * @return true, if account is registred
     */
    bool checkAccountStatus();

    /**
     * Get account information
     * @return Object with information about the registred account
     */
    QVariantMap getAccountInformation();

    /**
     * Register client on a given host
     * @param host Address of server
     * @param user_name Login name
     * @param password Login password
     * @return true, if successful
     */
    bool registerToServer(QString host, QString user_name, QString password);

    /**
     * Unregister client from server
     */
    void unregisterFromServer();

    /**
     * Starts an outgoing call
     * @param number Phone number or name to call
     * @return ID of the new call
     */
    int makeCall(const QString &number);

    /**
     * Accept the call with given id
     * @param call_id ID of the call to accept
     */
    void callAccept(const int call_id);

    /**
     * Finish call with given id
     * @param call_id ID of the call to end
     */
    void hangup(const int call_id);

    /**
     * Finish all running calls
     */
    void hangupAll();

    /**
     * Set the LogLevel
     */
    void setLogLevel(const unsigned int log_level);

    /**
     * Get stored custom user data to specific call
     * @param call_id ID of the call
     * @return custom user data as a string
     */
    QString getCallUserData(const int call_id);

    /**
     * Store custom user data to a call
     * @param call_id ID of the call
     * @param data Customer user data as a string
     */
    void setCallUserData(const int call_id, QString data);

    /**
     * Get a list of error logs
     * @return List of error log data
     */
    QVariantList getErrorLogData();

    /**
     * Delete error log file
     */
    void deleteErrorLogFile();

    /**
     * Adds a call to a conference
     * @param src_id ID of call to add to conference
     * @param dst_id ID of conference
     * @return true, if successful
     */
    bool addToConference(const int src_id, const int dst_id);

    /**
     * Remove a call from a conference
     * @param src_id ID of call to remove from conference
     * @param dst_id ID of conference
     * @return true, if successful
     */
    bool removeFromConference(const int src_id, const int dst_id);

    /**
     * Redirect an active call to a new destination
     * @param call_id ID of the call to be redirected
     * @param dst_url Number or address of the new destination
     * @return success code
     */
    int redirectCall(const int call_id, const QString dst_url);

    /**
     * Get list of all active calls
     * @return List of active calls
     */
    QVariantList getActiveCallList();

    /**
     * Switch sound on/off
     * @param mute true, if sound of call should be muted
     * @param call_id ID of call, or -1
     */
    void muteSound(const bool mute, const int call_id = -1);

    /**
     * Switch microphone on/off
     * @param mute true, if microphone of call should be muted
     * @param call_id ID of call, or -1
     */
    void muteMicrophone(const bool mute, const int call_id = -1);

    /**
     * Get information about signal levels
     * @return Map with sound and micro signal levels
     */
    QVariantMap getSignalInformation();

    /**
     * Get data of an option
     * @param name The name of the option
     * @return The option data
     */
    QVariant getOption(const QString &name);

    /**
     * Set new data of an option
     * @param name The name of the option
     * @param option The new data of the option
     */
    void setOption(const QString &name, const QVariant &option);

    /**
     * Tell Qt to print a page given by url
     * @param url_str The url of the page to print
     */
    void printPage(const QString &url_str);

    /**
     * Get log message from js and send it to the log_handler
     * @param log The log object
     */
    bool sendLogMessage(QVariantMap log);

    /**
     * Slot to catch sended signals
     * @params call_id ID of incoming call
     * @params call_url Number of incoming call
     */
    void incomingCallSlot(const Call &call);

    /**
     * Prints errors, warning and debug messages
     * @param info Log information and message
     */
    void logMessageSlot(const LogInfo &info);

    /**
     * Sound level has changed
     * @param level New sound level
     */
    void soundLevelSlot(int level);

    /**
     * Microphone level has changed
     * @param level New microphone level
     */
    void microphoneLevelSlot(int level);

    /**
     * Get a list of log files
     * @return List of filenames
     */
    QStringList getLogFileList();

    /**
     * Get the content of a log file
     * @param file_name File name of log file
     * @return Log file content
     */
    QString getLogFileContent(const QString &file_name);
    
    /**
     * Delete a log file
     * @param file_name File name of log file that should be deleted
     */
    void deleteLogFile(const QString &file_name);

private:
    Phone &phone_;
    PrintHandler *print_handler_;
    QWebView *web_view_;
    QString js_class_handler_;

    /**
     * This method handles the communication with javascript in the
     * internal browser.
     * @param func The name of the function to be called
     * @return The return value of the executed function
     */
    QVariant callJavascriptFunc(const QString &func);
};

#endif // JAVASCRIPTHANDLER_INCLUDE_H
