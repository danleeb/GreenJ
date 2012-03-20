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

#include "account.h"

//-----------------------------------------------------------------------------
Account::Account() : user_name_(""), password_(""), host_("")
{
}

//-----------------------------------------------------------------------------
Account::~Account()
{
}

//-----------------------------------------------------------------------------
void Account::setUserName(const QString &user_name)
{
  user_name_ = user_name;
}

//-----------------------------------------------------------------------------
void Account::setPassword(const QString &password)
{
    password_ = password;
}

//-----------------------------------------------------------------------------
void Account::setHost(const QString &host)
{
    host_ = host;
}

//-----------------------------------------------------------------------------
const QString &Account::getUserName() const
{
    return user_name_;
}

//-----------------------------------------------------------------------------
const QString &Account::getPassword() const
{
    return password_;
}

//-----------------------------------------------------------------------------
const QString &Account::getHost() const
{
    return host_;
}
