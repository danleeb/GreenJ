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

#include "javascript_handler.h"

#include <QWebFrame>
#include <QString>
#include <QTextDocument>

#include "call.h"
#include "phone.h"
#include "print_handler.h"
#include "log_info.h"
#include "log_handler.h"
#include "account.h"
#include "config_file_handler.h"

//----------------------------------------------------------------------
JavascriptHandler::JavascriptHandler(Phone &phone) :
    phone_(phone), web_view_(0), js_class_handler_("")
{
}

//----------------------------------------------------------------------
void JavascriptHandler::init(QWebView *web_view, PrintHandler *print_handler)
{
    web_view_ = web_view;
    print_handler_ = print_handler;
}

//----------------------------------------------------------------------
QVariant JavascriptHandler::callJavascriptFunc(const QString &func)
{
    QVariant ret;
    if (js_class_handler_.isEmpty())
    {
        ret = web_view_->page()->mainFrame()->evaluateJavaScript(func);
    }
    else
    {
        ret = web_view_->page()->mainFrame()->evaluateJavaScript(js_class_handler_+"."+func);
    }

    return ret;
}

//----------------------------------------------------------------------
void JavascriptHandler::accountState(const int &state)
{
    QString state_str(QString::number(state));

    callJavascriptFunc("accountStateChanged("+state_str+")");
}

//----------------------------------------------------------------------
void JavascriptHandler::callState(const int &call_id, const int &code, 
                                  const int &last_status)
{
    QString call_str(QString::number(call_id));
    QString code_str(QString::number(code));
    QString status_str(QString::number(last_status));
    callJavascriptFunc("callStateChanged("+call_str+","+code_str+","+status_str+")");
}

//----------------------------------------------------------------------
void JavascriptHandler::incomingCall(const Call &call)
{
    QString call_str = QString::number(call.getCallId());
    QString sip_url = call.getCallUrl();
    QString name = call.getCallName();

    callJavascriptFunc("incomingCall("+call_str+",'"+sip_url+"','"+name+"')");
}

//----------------------------------------------------------------------
QUrl JavascriptHandler::getPrintPage()
{
    QVariant url = callJavascriptFunc("getPrintUrl();");

    if (!url.convert(QVariant::Url))
    {
        if (!url.isNull())
        {
            LogInfo info(LogInfo::STATUS_ERROR, "print", 0, "Print Page: Wrong Url Format!");
            LogHandler::getInstance().logData(info);
        }
        return QUrl("about:blank");
    }

    return url.toUrl();
}

//----------------------------------------------------------------------
// Public slots
// These functions could be called by webkit-js
//----------------------------------------------------------------------
int JavascriptHandler::registerJsCallbackHandler(const QString &class_name)
{
    js_class_handler_ = class_name;
    return 0;
}

//----------------------------------------------------------------------
bool JavascriptHandler::checkAccountStatus()
{
    return phone_.checkAccountStatus();
}

//----------------------------------------------------------------------
QVariantMap JavascriptHandler::getAccountInformation()
{
    QVariantMap account_info;
    phone_.getAccountInfo(account_info);
    return account_info;
}

//----------------------------------------------------------------------
bool JavascriptHandler::registerToServer(QString host, QString user_name,
                                         QString password)
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "registerToServer");
    LogHandler::getInstance().logData(info);

    Account acc;
    acc.setUserName(user_name);
    acc.setPassword(password);
    acc.setHost(host);

    return phone_.registerUser(acc);
}

//----------------------------------------------------------------------
void JavascriptHandler::unregisterFromServer()
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "unregisterFromServer");
    LogHandler::getInstance().logData(info);

    phone_.unregister();
}

//----------------------------------------------------------------------
int JavascriptHandler::makeCall(const QString &number)
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "call "+number);
    LogHandler::getInstance().logData(info);

    return phone_.makeCall(number);
}

//----------------------------------------------------------------------
void JavascriptHandler::callAccept(const int &call_id)
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "accept call "+QString::number(call_id));
    LogHandler::getInstance().logData(info);

    phone_.answerCall(call_id);
}

//----------------------------------------------------------------------
void JavascriptHandler::hangup(const int &call_id)
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "hangup call "+QString::number(call_id));
    LogHandler::getInstance().logData(info);

    phone_.hangUp(call_id);

    LogInfo info2(LogInfo::STATUS_DEBUG, "js_handler", 0, "hangup finished");
    LogHandler::getInstance().logData(info2);
}

//----------------------------------------------------------------------
void JavascriptHandler::hangupAll()
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "hangup all ");
    LogHandler::getInstance().logData(info);

    phone_.hangUpAll();
}

//----------------------------------------------------------------------
void JavascriptHandler::setLogLevel(const unsigned &log_level)
{
    LogHandler::getInstance().setLogLevel(log_level);
}

//----------------------------------------------------------------------
QString JavascriptHandler::getCallUserData(const int &call_id)
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "Get user data "+QString::number(call_id));
    LogHandler::getInstance().logData(info);

    return phone_.getCallUserData(call_id);
}

//----------------------------------------------------------------------
void JavascriptHandler::setCallUserData(const int &call_id, QString data)
{

    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "Set user data "+QString::number(call_id));
    LogHandler::getInstance().logData(info);

    phone_.setCallUserData(call_id, data);
}

//----------------------------------------------------------------------
QVariantList JavascriptHandler::getErrorLogData()
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "Get error log data");
    LogHandler::getInstance().logData(info);

    QVariantList log_data;
    QFile file("error.log");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    while (!in.atEnd())
    {
        QVariantMap current;
        Call call;
        in >> call;
        call.getCallInfo(current);
        log_data << current;
    }

    return log_data;
}

//----------------------------------------------------------------------
void JavascriptHandler::deleteErrorLogFile()
{
    LogInfo info(LogInfo::STATUS_DEBUG, "js_handler", 0, "Delete error log file");
    LogHandler::getInstance().logData(info);

    QFile::remove("error.log");
}

//----------------------------------------------------------------------
bool JavascriptHandler::addToConference(const int &src_id, const int &dst_id)
{
    return phone_.addCallToConference(src_id, dst_id);
}

//----------------------------------------------------------------------
bool JavascriptHandler::removeFromConference(const int &src_id, const int &dst_id)
{
    return phone_.removeCallFromConference(src_id, dst_id);
}

//----------------------------------------------------------------------
int JavascriptHandler::redirectCall(const int &call_id, const QString dst_url)
{
    return phone_.redirectCall(call_id, dst_url);
}

//----------------------------------------------------------------------
QVariantList JavascriptHandler::getActiveCallList()
{
    QVariantList call_list;
    phone_.getActiveCallList(call_list);

    return call_list;
}

//----------------------------------------------------------------------
void JavascriptHandler::muteSound(const bool &mute, const int &call_id)
{
  phone_.muteSound(mute, call_id);
}

//----------------------------------------------------------------------
void JavascriptHandler::muteMicrophone(const bool &mute, const int &call_id)
{
  phone_.muteMicrophone(mute, call_id);
}

//----------------------------------------------------------------------
QVariantMap JavascriptHandler::getSignalInformation()
{
    QVariantMap signal_info;
    phone_.getSignalInformation(signal_info);
    return signal_info;
}

//----------------------------------------------------------------------
QVariant JavascriptHandler::getOption(const QString &name)
{
    return ConfigFileHandler::getInstance().getOption(name);
}

//----------------------------------------------------------------------
void JavascriptHandler::setOption(const QString &name, const QVariant &option)
{
    ConfigFileHandler::getInstance().setOption(name, option);
}

//----------------------------------------------------------------------
void JavascriptHandler::printPage(const QString &url_str)
{
    QUrl url(url_str);
    print_handler_->loadPrintPage(url);
}

//----------------------------------------------------------------------
bool JavascriptHandler::sendLogMessage(QVariantMap log)
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

    LogInfo info(status.toUInt(), domain.toString(), code.toInt(), msg.toString());
    info.time_.fromString(time.toString(), "dd.MM.yyyy hh:mm:ss");

    LogHandler::getInstance().logFromJs(info);
    return true;
}

//----------------------------------------------------------------------
void JavascriptHandler::incomingCallSlot(const Call &call)
{
    incomingCall(call);
}

//----------------------------------------------------------------------
void JavascriptHandler::logMessageSlot(const LogInfo &info)
{
    QString json;
    json.append("{'time':'"+info.time_.toString("dd.MM.yyyy hh:mm:ss")
                + "','status':"+QString::number(info.status_)+",'domain':'"+info.domain_
                +"','code':"+QString::number(info.code_)+",'message':'"+info.msg_+"'}");

    callJavascriptFunc("logMessage("+json+")");
}

//----------------------------------------------------------------------
void JavascriptHandler::soundLevelSlot(int level)
{
    callJavascriptFunc("soundLevel("+QString::number(level)+")");
}

//----------------------------------------------------------------------
void JavascriptHandler::microphoneLevelSlot(int level)
{
    callJavascriptFunc("microphoneLevel("+QString::number(level)+")");
}

//----------------------------------------------------------------------
QStringList JavascriptHandler::getLogFileList()
{
    return LogHandler::getInstance().getLogFileList();
}

//----------------------------------------------------------------------
QString JavascriptHandler::getLogFileContent(const QString &file_name)
{
    return LogHandler::getInstance().getLogFileContent(file_name);
}

//----------------------------------------------------------------------
void JavascriptHandler::deleteLogFile(const QString &file_name)
{
    LogHandler::getInstance().deleteLogFile(file_name);
}
