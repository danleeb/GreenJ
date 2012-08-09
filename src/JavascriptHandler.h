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

class LogInfo;

namespace phone {
    class Phone;
    class Call;
}

/**
 * This class is the bridge between Qt and website-javascript
 */
class JavascriptHandler : public QObject
{
    Q_OBJECT

public:
    static const QString OBJECT_NAME;

    /**
     * Constructor
     * @param web_view WebView needed to call the JS functions
     * @param phone To access the phone methods
     */
    JavascriptHandler(QWebView *web_view, phone::Phone &phone);

    /**
     * Send current account state
     * @param state
     */
    void accountStateChanged(const int state) const;

    /**
     * Send call state
     * @param call_id
     * @param code status code
     * @param last_status last status code
     */
    void callState(const int call_id, const int code, const int last_status) const;

    /**
     * Notify about an incoming call
     * @param call
     */
    void incomingCall(const phone::Call &call) const;

    /**
     * Request the url of a page to print
     * @return The url of the webpage that should be printed
     */
    QUrl getPrintUrl() const;

    /**
     * Sound level has changed
     * @param level New sound level
     */
    void soundLevel(int level) const;

    /**
     * Microphone level has changed
     * @param level New microphone level
     */
    void microphoneLevel(int level) const;

signals:
    /**
     * Signal when weppage-url changed
     */
    void signalWebPageChanged();

    /**
     * Signal that a page should be printed
     * @param url Page url
     */
    void signalPrintPage(const QUrl &url);

public slots:
    /**
     * Register an individual callback-handler by name.
     * This handler is a JavaScript object and must implement
     * all necessary callback methods.
     * @param handler_name The name of the handler to register
     */
    int registerJsCallbackHandler(const QString &handler_name);

    /**
     * Get the account status
     * @return true, if account is registred
     */
    bool checkAccountStatus() const;

    /**
     * Get account information
     * @return Object with information about the registred account
     */
    QVariantMap getAccountInformation() const;

    /**
     * Register client on a given host
     * @param host Address of server
     * @param user_name Login name
     * @param password Login password
     * @return true, if successful
     */
    bool registerToServer(const QString &host, const QString &user_name,
                          const QString &password) const;

    /**
     * Unregister client from server
     */
    void unregisterFromServer() const;

    /**
     * Starts an outgoing call
     * @param number Phone number or name to call
     * @return ID of the new call (-1 if unsuccessful)
     */
    int makeCall(const QString &number) const;

    /**
     * Accept the call with given id
     * @param call_id ID of the call to accept
     */
    void callAccept(const int call_id) const;

    /**
     * Finish call with given id
     * @param call_id ID of the call to end
     */
    void hangup(const int call_id) const;

    /**
     * Finish all running calls
     */
    void hangupAll() const;

    /**
     * Set the LogLevel
     */
    void setLogLevel(const unsigned int log_level) const;

    /**
     * Get stored custom user data to specific call
     * @param call_id ID of the call
     * @return custom user data as a string
     */
    QString getCallUserData(const int call_id) const;

    /**
     * Store custom user data to a call
     * @param call_id ID of the call
     * @param data Customer user data as a string
     */
    void setCallUserData(const int call_id, const QString &data) const;

    /**
     * Get a list of error logs
     * @return List of error log data
     */
    QVariantList getErrorLogData() const;

    /**
     * Delete error log file
     */
    void deleteErrorLogFile() const;

    /**
     * Adds a call to a conference
     * @param src_id ID of call to add to conference
     * @param dst_id ID of conference
     * @return true, if successful
     */
    bool addToConference(const int src_id, const int dst_id) const;

    /**
     * Remove a call from a conference
     * @param src_id ID of call to remove from conference
     * @param dst_id ID of conference
     * @return true, if successful
     */
    bool removeFromConference(const int src_id, const int dst_id) const;

    /**
     * Redirect an active call to a new destination
     * @param call_id ID of the call to be redirected
     * @param dst_uri Number or address of the new destination
     * @return success code
     */
    int redirectCall(const int call_id, const QString &dst_uri) const;

    /**
     * Get list of all active calls
     * @return List of active calls
     */
    QVariantList getActiveCallList() const;

    /**
     * Switch sound on/off
     * @param mute true, if sound of call should be muted
     * @param call_id ID of call, or -1
     */
    void muteSound(const bool mute, const int call_id = -1) const;

    /**
     * Switch microphone on/off
     * @param mute true, if microphone of call should be muted
     * @param call_id ID of call, or -1
     */
    void muteMicrophone(const bool mute, const int call_id = -1) const;

    /**
     * Set sound level
     * @param level new sound level (0..255)
     * @param call_id ID of call, or -1
     */
    void setSoundLevel(const int level, const int call_id) const;

    /**
     * Set microphone level
     * @param level new microphone level (0..255)
     * @param call_id ID of call, or -1
     */
    void setMicrophoneLevel(const int level, const int call_id) const;

    /**
     * Get information about signal levels
     * @return Map with sound and micro signal levels
     */
    QVariantMap getSignalInformation() const;
    
    /**
     * Set priority of codec to new_priority
     * @param codec Name of Codec
     * @param new_priority Range 0...PJMEDIA_CODEC_PRIO_HIGHEST
     */
    void setCodecPriority(const QString &codec, int new_priority) const;

    /**
     * Return all codecs and priorities
     * @return Map of codecs with priorities
     */
    QVariantMap getCodecPriorities() const;

    /**
     * Select or change sound device
     * @param input Device ID of the capture device.
     * @param output Device ID of the playback device.
     */
    void setSoundDevice(const int input, const int output) const;

    /**
     * Return a list of all available sound devices
     * @return list of QVariantMaps with device information
     */
    QVariantList getSoundDevices() const;

    /**
     * Get data of an option
     * @param name The name of the option
     * @return The option data
     */
    QVariant getOption(const QString &name) const;

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
    bool sendLogMessage(const QVariantMap &log) const;

    /**
     * Get a list of log files
     * @return List of filenames
     */
    QStringList getLogFileList() const;

    /**
     * Get the content of a log file
     * @param file_name File name of log file
     * @return Log file content
     */
    QString getLogFileContent(const QString &file_name) const;
    
    /**
     * Delete a log file
     * @param file_name File name of log file that should be deleted
     */
    void deleteLogFile(const QString &file_name) const;

private slots:
    /**
     * Prints errors, warning and debug messages
     * @param info Log information and message
     */
    void slotLogMessage(const LogInfo &info) const;

private:
    QWebView *web_view_;
    phone::Phone &phone_;

    QString js_callback_handler_;

    /**
     * This method handles the communication with javascript by
     * evaluating code in the internal browser.
     * @param code JavaScript code
     * @return The return value from JavaScript
     */
    QVariant evaluateJavaScript(const QString &code) const;
};

#endif // JAVASCRIPTHANDLER_INCLUDE_H
