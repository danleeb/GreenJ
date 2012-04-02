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

#ifndef LOGINFO_INCLUDE_H
#define LOGINFO_INCLUDE_H

#include <QMetaType>
#include <QDateTime>
#include <QString>

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

#endif // LOGINFO_INCLUDE_H
