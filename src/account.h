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

#ifndef ACCOUNT_INCLUDE_H
#define ACCOUNT_INCLUDE_H

#include <QString>

/**
 * This class stores account data used for logging into the server.
 */
class Account
{
public:
    /**
     * Constructor
     */
    Account();

    /**
     * Destructor
     */
    virtual ~Account();

    // Setters/Getters
    void setUserName(const QString &user_name);
    void setPassword(const QString &password);
    void setHost(const QString &host);

    const QString &getUserName() const;
    const QString &getPassword() const;
    const QString &getHost() const;

private:
    QString user_name_;
    QString password_;
    QString host_;
};

#endif // ACCOUNT_INCLUDE_H
