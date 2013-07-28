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

#ifndef WEBPAGE_INCLUDE_H
#define WEBPAGE_INCLUDE_H

#include <QWebPage>
#include <QWebHistory>
#include <QString>
#include <QUrl>

/**
 * Overloads Qts WebPage class to manipulate user agent etc.
 */
class WebPage : public QWebPage
{
    /**
     * Set a default user-agent to the browser
     * This is necessary to prevent being identified as smartphone by js-libs
     * (e.g. Google Maps)
     * @param url Currently unused
     * @return User agent string
     */
    // virtual QString userAgentForUrl(const QUrl &url) const 
    // {
    //     Q_UNUSED(url);
    //     return "Chrome/1.0";
    // }

    /**
     * Prevent history-back function being executed
     * @param action The browser action
     * @param checked
     */
    // virtual void triggerAction(WebAction action, bool checked = false)
    // {
    //     if (action == Back) {
    //         return;
    //     }
    //     QWebPage::triggerAction(action, checked);
    // }
};

#endif // WEBPAGE_INCLUDE_H
