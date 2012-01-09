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

#ifndef LOG_INFO_H
#define LOG_INFO_H

#include <QMetaType>
#include <QDateTime>
#include <QString>

/**
 * This class stores information to log like status, message and log_time
 */
class LogInfo
{
public:
    /**
     * \name Log Status Codes
     * Statuscodes are some kind of priority of writing log messages.
     * These codes here are constants to simplify use of the status
     * and to recognize the severity of the log message more easily.
     */
    //@{
    /**
     * STATUS code for debug
     */
    static const unsigned STATUS_DEBUG;
    /**
     * STATUS code for message
     */
    static const unsigned STATUS_MESSAGE;
    /**
     * STATUS code for warning
     */
    static const unsigned STATUS_WARNING;
    /**
     * STATUS code for error
     */
    static const unsigned STATUS_ERROR;
    /**
     * STATUS code for fatal_error
     */
    static const unsigned STATUS_FATAL_ERROR;
    //@}

    /**
     * translation of status code 
     */
    static const QString STATUS_STRING[];

    /**
     * the log_message status code
     */
    unsigned status_;

    /**
     * the domain, where log_message is from
     */
    QString domain_;

    /**
     * the log code like 200 when all worked well
     */
    int code_;

    /**
     * the log message
     */
    QString msg_;

    /**
     * the log time
     */
    QDateTime time_;

    /**
     * basic constructor, not really used but Q_DECLARE_METATYPE needs it
     */
    LogInfo(){}

    /**
     * Constructor of the class
     * @param status unsigned. the status of the log message (debug ... fatal_error)
     * @param domain QString, used to identify where the log_message came from
     * @param code int, some log messages use an error code
     * @param msg QString, the log text
     */
    LogInfo(const unsigned &status, const QString &domain, const int &code,
            const QString &msg);

    /**
     * Convert the status_ var of the class into a readable string
     * @return QString the name of current status_
     */
    QString getStatusString() const;
};

Q_DECLARE_METATYPE(LogInfo)

#endif // LOG_INFO_H
