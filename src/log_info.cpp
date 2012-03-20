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

const unsigned LogInfo::STATUS_DEBUG       = 0x00;
const unsigned LogInfo::STATUS_MESSAGE     = 0x01;
const unsigned LogInfo::STATUS_WARNING     = 0x02;
const unsigned LogInfo::STATUS_ERROR       = 0x03;
const unsigned LogInfo::STATUS_FATAL_ERROR = 0x04;

const QString LogInfo::STATUS_STRING[] = { "DEBUG", "MESSAGE", "WARNING",
                                           "ERROR", "FATALERROR" };

//-----------------------------------------------------------------------------
LogInfo::LogInfo(const unsigned status, const QString &domain, 
                 const int code, const QString &msg) : 
    status_(status), domain_(domain), code_(code), msg_(msg), 
    time_(QDateTime::currentDateTime())
{
}

//-----------------------------------------------------------------------------
QString LogInfo::getStatusString() const
{
    return STATUS_STRING[status_];
}
