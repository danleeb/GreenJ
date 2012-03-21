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
#include <QString>
#include <QReadWriteLock>
#include <QStringList>

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
     * Get log information from webkit to write it into a file
     * @param info The log information data
     */
    void logFromJs(const LogInfo &info);

    /**
     * Set the LogLevel
     */
    void setLogLevel(const unsigned int log_level);

    /**
     * Get a list of log files
     * @return List of filenames
     */
    const QStringList &getLogFileList() const;
    
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
    void deleteLogFile(const QString &file_name);

public slots:
    /**
     * Get log information to write it into a file and try to send it to webkit
     * @param info The log information data
     */
    void slotLogData(const LogInfo &info);

signals:
    /**
     * Signal to send message to webkit
     * @param info The log information data     
     */
    void signalLogMessage(const LogInfo &info);

private:
    QFile file_;
    QReadWriteLock lock_;
    QStringList log_list_;

    LogHandler();
    LogHandler(const LogHandler&);
    virtual ~LogHandler();

    void writeFile(const QString &msg);
};

#endif // LOGHANDLER_INCLUDE_H
