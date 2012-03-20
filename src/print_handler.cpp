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

#include <QPrinter>
#include <QPrintPreviewDialog>
#include "gui.h"
#include "javascript_handler.h"
#include "print_handler.h"


//-----------------------------------------------------------------------------
PrintHandler::PrintHandler(Gui &gui, JavascriptHandler &js_handler)
    : gui_(gui), js_handler_(js_handler)
{
    connect(&print_page_, SIGNAL(loadFinished(bool)), 
            this,         SLOT(showPrintPreview()));
}

//-----------------------------------------------------------------------------
void PrintHandler::loadPrintPage(const QUrl &url)
{
    QNetworkCookieJar *cookie = gui_.getWindow().webview->page()->networkAccessManager()->cookieJar();

    print_page_.page()->networkAccessManager()->setCookieJar(cookie);
    print_page_.load(url);
}

//-----------------------------------------------------------------------------
void PrintHandler::showPrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog *preview = new QPrintPreviewDialog(&printer, &gui_);

    preview->setWindowTitle(tr("Print"));
    connect(preview,      SIGNAL(paintRequested(QPrinter*)), 
            &print_page_, SLOT(print(QPrinter*)));
    preview->exec();
}

//-----------------------------------------------------------------------------
void PrintHandler::printKeyPressed()
{
    QUrl url = js_handler_.getPrintPage();
    if (url != QUrl("about:blank")) {
        loadPrintPage(url);
    }
}
