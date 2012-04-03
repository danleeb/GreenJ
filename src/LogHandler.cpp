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

#include <QDateTime>
#include <QTextStream>
#include <QDir>
#include "LogInfo.h"
#include "Config.h"
#include "LogHandler.h"

//-----------------------------------------------------------------------------
LogHandler::LogHandler() : 
    file_("log-" + QDateTime::currentDateTime().toString("MM-yyyy") + ".log")
{
    level_ = Config::getInstance().getApplicationLogLevel();

    QDir current(".");
    QStringList filters;
    filters << "*.log";
    file_list_ << current.entryList(filters);

    QString msg("========== ");
    msg.append(QDateTime::currentDateTime().toString("ddd dd.MM.yyyy hh:mm:ss"));
    msg.append(" - Session started ==========\n");
    writeFile(msg);
}

//-----------------------------------------------------------------------------
LogHandler::~LogHandler()
{
    QString msg("========== ");
    msg.append(QDateTime::currentDateTime().toString("ddd dd.MM.yyyy hh:mm:ss"));
    msg.append(" - Session closed ==========\n");
    writeFile(msg);
}

//-----------------------------------------------------------------------------
LogHandler &LogHandler::getInstance()
{
    static LogHandler instance;
    return instance;
}

//-----------------------------------------------------------------------------
void LogHandler::setLevel(const uint level)
{
    level_ = level;
    Config::getInstance().setLogLevel(level);
}

//-----------------------------------------------------------------------------
const QStringList &LogHandler::getFileList() const
{
    return file_list_;
}

//-----------------------------------------------------------------------------
QString LogHandler::getFileContent(const QString &file_name) const
{
    if (file_list_.contains(file_name)) {
        QFile log_file(file_name);
        if (!log_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return "";
        }
        QTextStream in(&log_file);
        QString result;
        while (!in.atEnd()) {
            QString line = in.readLine();
            result.append(line + "\n");
        }
        log_file.close();
        return result;
    }
    return "";
}

//-----------------------------------------------------------------------------
void LogHandler::deleteFile(const QString &file_name)
{
    int index = file_list_.indexOf(file_name);
    if (index != -1) {
        file_list_.removeAt(index);
        QFile::remove(file_name);
    }
}

//-----------------------------------------------------------------------------
void LogHandler::log(const LogInfo &info, bool signal)
{
    if (info.status_ >= level_) {
        writeFile(info.toString());
        if (signal) {
            signalLogMessage(info);
        }
    }
}

//-----------------------------------------------------------------------------
void LogHandler::logFromJs(const LogInfo &info)
{
    log(info, false);
}

//-----------------------------------------------------------------------------
void LogHandler::writeFile(const QString &msg)
{
    lock_.lockForWrite();

    if (file_.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file_);
        out << msg;

        file_.close();
    }

    lock_.unlock();
}

//-----------------------------------------------------------------------------
// LogInfo

const QString LogInfo::STATUS_[] = { "DEBUG", "MESSAGE", "WARNING",
                                     "ERROR", "FATAL" };

//-----------------------------------------------------------------------------
LogInfo::LogInfo(const Status status, const QString &domain, 
                 const int code, const QString &msg) : 
    status_(status), domain_(domain), code_(code), msg_(msg), 
    time_(QDateTime::currentDateTime())
{
}

//-----------------------------------------------------------------------------
const QString &LogInfo::getStatusString() const
{
    return STATUS_[status_];
}

//-----------------------------------------------------------------------------
QString LogInfo::toString() const
{
    return time_.toString("hh:mm:ss") + " [" 
         + getStatusString() + "] " 
         + domain_ + ": [" 
         + QString::number(code_) + "] " 
         + msg_ + "\n";
}
