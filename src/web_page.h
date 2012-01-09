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

#ifndef WEB_PAGE_H
#define WEB_PAGE_H

#include <QWebPage>
#include <QWebHistory>
#include <QString>
#include <QUrl>

#include "log_info.h"
#include "log_handler.h"

/**
 * This class gives webkit a simple name.
 * It is needed because some web-pages like google-maps could have problems
 * with not available touchscreen
 */
class WebPage : public QWebPage
{
    /**
     * Set a default user-agent to the browser
     * This is necessary to prevent being identified as smartphone by js-libs
     * @param url QUrl, currently unused
     */
    // virtual QString userAgentForUrl(const QUrl &url) const 
    // {
    //     Q_UNUSED(url);
    //     return "Chrome/1.0";
    // }

    /**
     * Prevent history-back function being executed
     * @param action webaction, the action of browser
     * @param checked bool
     */
    // virtual void triggerAction(WebAction action, bool checked = false)
    // {
    //     if (action == Back) {
    //         return;
    //     }
    //     QWebPage::triggerAction(action, checked);
    // }
};

#endif // WEB_PAGE_H
