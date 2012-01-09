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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QXmlStreamReader>

/**
 * This class handles account data, needed
 * to log in to voip-server
 */
class Account
{
    QString user_name_;
    QString password_;
    QString host_;

public:
    Account();
    ~Account();

    /**
     * set the user_name
     * @param user_name QString, the user_name
     */
    void setUserName(const QString &user_name);

    /**
     * set the password
     * @param password QString, the password
     */
    void setPassword(const QString &password);

    /**
     * set the host
     * @param host_name QString, the host
     */
    void setHost(const QString &host);

    /**
     * get the user_name
     * @return QString, the user_name
     */
    const QString &getUserName() const;

    /**
     * get the password
     * @return QString, the password
     */
    const QString &getPassword() const;

    /**
     * get the host
     * @return QString, the host
     */
    const QString &getHost() const;
};

#endif // ACCOUNT_H
