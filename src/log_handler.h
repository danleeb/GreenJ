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

#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QReadWriteLock>
#include <QStringList>

class LogInfo;

/**
 * Handles the log data. It writes log messages into the
 * logfile and signals it for interested classes
 */
class LogHandler : public QObject
{
    Q_OBJECT

private:
    QFile file_;
    QReadWriteLock lock_;
    QStringList log_list_;

    LogHandler();
    LogHandler(const LogHandler&);
    ~LogHandler();

    QString createString(const LogInfo &info);
    void writeFile(const QString &msg);

signals:
    /**
     * Signal to send message to webkit
     * @param info LogInfo, the log information class     
     */
    void signalLogMessage(const LogInfo &info);

public:
    /**
     * Gets a reference to this class
     * @return LogHandler& the reference to this class
     */
    static LogHandler &getInstance();

    /**
     * Get Loginformation from webkit to write it into a file
     * @param info LogInfo, the log information class
     */
    void logFromJs(const LogInfo &info);

    /**
     * Get Loginformation to write it into a file and try to send it to webkit
     * @param info LogInfo, the log information class
     */
    void logData(const LogInfo &info);

public slots:
    /**
     * Get Loginformation to write it into a file and try to send it to webkit
     * @param info LogInfo, the log information class
     */
    void logDataSlot(const LogInfo &info);

    /**
     * Set the LogLevel
     */
    void setLogLevel(const unsigned &log_level);

    const QStringList &getLogFileList() const;
    QString getLogFileContent(const QString &file_name) const;
    void deleteLogFile(const QString &file_name);
};

#endif // LOG_HANDLER_H
