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
     * \name Log Status Codes
     * Status codes are priorities of writing log messages.
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
     * Translations of status codes
     */
    static const QString STATUS_STRING[];

    /**
     * Status code
     */
    unsigned status_;

    /**
     * The domain of the log message
     */
    QString domain_;

    /**
     * Log message code
     */
    int code_;

    /**
     * Log message
     */
    QString msg_;

    /**
     * Log time
     */
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
    LogInfo(const unsigned int status, const QString &domain, const int code,
            const QString &msg);

    /**
     * Convert the status code into a string
     * @return The text of current status_
     */
    QString getStatusString() const;
};

Q_DECLARE_METATYPE(LogInfo)

#endif // LOGINFO_INCLUDE_H
