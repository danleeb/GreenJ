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

#include "log_info.h"

const QString LogInfo::STATUS_[] = { "DEBUG", "MESSAGE", "WARNING",
                                     "ERROR", "FATALERROR" };

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