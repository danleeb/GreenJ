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

#ifndef LOGHANDLER_INCLUDE_H
#define LOGHANDLER_INCLUDE_H

#include <QObject>
#include <QFile>
#include <QReadWriteLock>
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QDateTime>

class LogInfo;

/**
 * Singleton that handles the logging. 
 * Writes log messages into the logfile and sends them to webkit.
 */
class LogHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * Gets a reference to this class
     * @return LogHandler& the reference to this class
     */
    static LogHandler &getInstance();

    /**
     * Set the log level
     * @param level New log level
     */
    void setLevel(const uint level);

    /**
     * Get a list of log files
     * @return List of filenames
     */
    const QStringList &getFileList() const;
    
    /**
     * Get the content of a log file
     * @param file_name File name of log file
     * @return Log file content
     */
    QString getFileContent(const QString &file_name) const;
    
    /**
     * Delete a log file
     * @param file_name File name of log file that should be deleted
     */
    void deleteFile(const QString &file_name);

    /**
     * Get log information to write it into a file and try to send it to webkit
     * @param info The log information data
     * @param signal True, if the log message should be signaled
     */
    void log(const LogInfo &info, bool signal = true);

signals:
    /**
     * Signal to send message to webkit
     * @param info The log information data     
     */
    void signalLogMessage(const LogInfo &info);

private:
    QFile file_;
    QReadWriteLock lock_;
    QStringList file_list_;
    uint level_;

    LogHandler();
    LogHandler(const LogHandler&);
    virtual ~LogHandler();

    void writeFile(const QString &msg);
};

/**
 * This class stores log information (status, message, time, etc)
 */
class LogInfo
{
public:
    /**
     * Log status codes
     * Status codes are priorities of writing log messages.
     */
    enum Status {
        STATUS_DEBUG = 0,
        STATUS_MESSAGE,
        STATUS_WARNING,
        STATUS_ERROR,
        STATUS_FATAL
    };

    /**
     * Translations of status codes
     */
    static const QString STATUS_[];

    Status status_;
    QString domain_;
    int code_;
    QString msg_;
    QDateTime time_;

    /**
     * Basic constructor, not really used but Q_DECLARE_METATYPE needs it
     */
    LogInfo() {}

    /**
     * Constructor
     * @param status The status of the log message (STATUS_*)
     * @param domain Used to identify where the log message came from
     * @param code Some log messages use an error code
     * @param msg Log message text
     */
    LogInfo(const Status status, const QString &domain, const int code,
            const QString &msg);

    /**
     * Convert the status code into a string
     * @return The text of current status_
     */
    const QString &getStatusString() const;
    
    /**
     * Return a formatted string for this log info entry
     * @return Log message string
     */
    QString toString() const;
};

Q_DECLARE_METATYPE(LogInfo)

#endif // LOGHANDLER_INCLUDE_H
