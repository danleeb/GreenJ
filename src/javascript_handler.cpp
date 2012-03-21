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

#include <QWebFrame>
#include <QString>
#include "call.h"
#include "phone.h"
#include "log_info.h"
#include "log_handler.h"
#include "account.h"
#include "config.h"
#include "javascript_handler.h"

//-----------------------------------------------------------------------------
JavascriptHandler::JavascriptHandler(Phone &phone, QWebView *web_view) :
    phone_(phone), web_view_(web_view), js_callback_handler_("")
{
}

//-----------------------------------------------------------------------------
void JavascriptHandler::accountState(const int state) const
{
    evaluateJavaScript("accountStateChanged(" + QString::number(state) + ")");
}

//-----------------------------------------------------------------------------
void JavascriptHandler::callState(const int call_id, const int code, 
                                  const int last_status) const
{
    evaluateJavaScript("callStateChanged(" + QString::number(call_id) + "," 
                                           + QString::number(code) + "," 
                                           + QString::number(last_status) + ")");
}

//-----------------------------------------------------------------------------
void JavascriptHandler::incomingCall(const Call &call) const
{
    evaluateJavaScript("incomingCall(" + QString::number(call.getCallId()) + ",'" 
                                       + call.getCallUrl() + "','" 
                                       + call.getCallName() + "')");
}

//-----------------------------------------------------------------------------
QUrl JavascriptHandler::getPrintPage() const
{
    QVariant url = evaluateJavaScript("getPrintUrl();");

    if (!url.convert(QVariant::Url)) {
        if (!url.isNull()) {
            LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_ERROR, "print", 0, "Print Page: Wrong Url Format!"));
        }
        return QUrl("about:blank");
    }

    return url.toUrl();
}

//-----------------------------------------------------------------------------
void JavascriptHandler::soundLevel(int level) const
{
    evaluateJavaScript("soundLevel(" + QString::number(level) + ")");
}

//-----------------------------------------------------------------------------
void JavascriptHandler::microphoneLevel(int level) const
{
    evaluateJavaScript("microphoneLevel(" + QString::number(level) + ")");
}

//-----------------------------------------------------------------------------
void JavascriptHandler::logMessage(const LogInfo &info) const
{
    QString json = "{'time':'" + info.time_.toString("dd.MM.yyyy hh:mm:ss")
                 + "','status':" + QString::number(info.status_) 
                 + ",'domain':'" + info.domain_
                 + "','code':" + QString::number(info.code_) 
                 + ",'message':'" + info.msg_ + "'}";

    evaluateJavaScript("logMessage(" + json + ")");
}

//-----------------------------------------------------------------------------
QVariant JavascriptHandler::evaluateJavaScript(const QString &code) const
{
    if (js_callback_handler_.isEmpty()) {
        return web_view_->page()->mainFrame()->evaluateJavaScript(code);
    }
    return web_view_->page()->mainFrame()->evaluateJavaScript(js_callback_handler_ + "." + code);
}

//-----------------------------------------------------------------------------
// Public slots
// These methods will be exposed as JavaScript methods.

//-----------------------------------------------------------------------------
int JavascriptHandler::registerJsCallbackHandler(const QString &handler_name)
{
    js_callback_handler_ = handler_name;
    return 0;
}

//-----------------------------------------------------------------------------
bool JavascriptHandler::checkAccountStatus() const
{
    return phone_.checkAccountStatus();
}

//-----------------------------------------------------------------------------
QVariantMap JavascriptHandler::getAccountInformation() const
{
    QVariantMap account_info;
    phone_.getAccountInfo(account_info);
    return account_info;
}

//-----------------------------------------------------------------------------
bool JavascriptHandler::registerToServer(const QString &host, const QString &user_name,
                                         const QString &password) const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "registerToServer"));

    Account acc;
    acc.setUserName(user_name);
    acc.setPassword(password);
    acc.setHost(host);

    return phone_.registerUser(acc);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::unregisterFromServer() const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "unregisterFromServer"));

    phone_.unregister();
}

//-----------------------------------------------------------------------------
int JavascriptHandler::makeCall(const QString &number) const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "call "+number));

    return phone_.makeCall(number);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::callAccept(const int call_id) const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "accept call " + QString::number(call_id)));

    phone_.answerCall(call_id);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::hangup(const int call_id) const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "hangup call " + QString::number(call_id)));

    phone_.hangUp(call_id);

    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "hangup finished"));
}

//-----------------------------------------------------------------------------
void JavascriptHandler::hangupAll() const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "hangup all "));

    phone_.hangUpAll();
}

//-----------------------------------------------------------------------------
void JavascriptHandler::setLogLevel(const unsigned int log_level) const
{
    LogHandler::getInstance().setLogLevel(log_level);
}

//-----------------------------------------------------------------------------
QString JavascriptHandler::getCallUserData(const int call_id) const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "Get user data " + QString::number(call_id)));

    return phone_.getCallUserData(call_id);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::setCallUserData(const int call_id, const QString &data) const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "Set user data " + QString::number(call_id)));

    phone_.setCallUserData(call_id, data);
}

//-----------------------------------------------------------------------------
QVariantList JavascriptHandler::getErrorLogData() const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "Get error log data"));

    QVariantList log_data;
    QFile file("error.log");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    while (!in.atEnd()) {
        QVariantMap current;
        Call call;
        in >> call;
        call.getCallInfo(current);
        log_data << current;
    }

    return log_data;
}

//-----------------------------------------------------------------------------
void JavascriptHandler::deleteErrorLogFile() const
{
    LogHandler::getInstance().slotLogData(LogInfo(LogInfo::STATUS_DEBUG, "js_handler", 0, "Delete error log file"));

    QFile::remove("error.log");
}

//-----------------------------------------------------------------------------
bool JavascriptHandler::addToConference(const int src_id, const int dst_id) const
{
    return phone_.addCallToConference(src_id, dst_id);
}

//-----------------------------------------------------------------------------
bool JavascriptHandler::removeFromConference(const int src_id, const int dst_id) const
{
    return phone_.removeCallFromConference(src_id, dst_id);
}

//-----------------------------------------------------------------------------
int JavascriptHandler::redirectCall(const int call_id, const QString &dst_uri) const
{
    return phone_.redirectCall(call_id, dst_uri);
}

//-----------------------------------------------------------------------------
QVariantList JavascriptHandler::getActiveCallList() const
{
    QVariantList call_list;
    phone_.getActiveCallList(call_list);

    return call_list;
}

//-----------------------------------------------------------------------------
void JavascriptHandler::muteSound(const bool mute, const int call_id) const
{
    phone_.muteSound(mute, call_id);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::muteMicrophone(const bool mute, const int call_id) const
{
    phone_.muteMicrophone(mute, call_id);
}

//-----------------------------------------------------------------------------
QVariantMap JavascriptHandler::getSignalInformation() const
{
    QVariantMap signal_info;
    phone_.getSignalInformation(signal_info);
    return signal_info;
}

//-----------------------------------------------------------------------------
QVariant JavascriptHandler::getOption(const QString &name) const
{
    return Config::getInstance().getOption(name);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::setOption(const QString &name, const QVariant &option)
{
    Config::getInstance().setOption(name, option);
    if (name == "url") {
        signalWebPageChanged();
    }
}

//-----------------------------------------------------------------------------
void JavascriptHandler::printPage(const QString &url_str)
{
    QUrl url(url_str);
    signalPrintPage(url);
}

//-----------------------------------------------------------------------------
bool JavascriptHandler::sendLogMessage(const QVariantMap &log) const
{
    QVariant time = log["time"];
    QVariant status = log["status"];
    QVariant domain = log["domain"];
    QVariant code = log["code"];
    QVariant msg = log["message"];

    if (!time.convert(QVariant::String) ||
        !status.convert(QVariant::UInt) ||
        !domain.convert(QVariant::String) ||
        !code.convert(QVariant::Int) ||
        !msg.convert(QVariant::String))
    {
        return false;
    }

    LogInfo info((LogInfo::Status)status.toUInt(), domain.toString(), code.toInt(), msg.toString());
    info.time_.fromString(time.toString(), "dd.MM.yyyy hh:mm:ss");

    LogHandler::getInstance().logFromJs(info);
    return true;
}

//-----------------------------------------------------------------------------
QStringList JavascriptHandler::getLogFileList() const
{
    return LogHandler::getInstance().getLogFileList();
}

//-----------------------------------------------------------------------------
QString JavascriptHandler::getLogFileContent(const QString &file_name) const
{
    return LogHandler::getInstance().getLogFileContent(file_name);
}

//-----------------------------------------------------------------------------
void JavascriptHandler::deleteLogFile(const QString &file_name) const
{
    LogHandler::getInstance().deleteLogFile(file_name);
}
