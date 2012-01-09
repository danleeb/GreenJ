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

#ifndef PRINT_HANDLER_H
#define PRINT_HANDLER_H

#include <QObject>
#include <QWebView>
#include <QUrl>

class Gui;
class JavascriptHandler;

/**
 * This class handles a print action.
 * It loads a WebPage with the print data.
 */
class PrintHandler : public QObject
{
    Q_OBJECT

    Gui &gui_;
    JavascriptHandler &js_handler_;
    QWebView print_page_;

public:
    /**
     * The constructor
     * @param gui Gui, a reference to the main gui, needed to get the cookie
     * @param js_handler JavascriptHandler, a reference to the js_handler
     */
    PrintHandler(Gui &gui, JavascriptHandler &js_handler);

    /**
     * load a page to print
     * @param url QUrl, the url to the printable webpage
     */
    void loadPrintPage(const QUrl &url);

private slots:
    void showPrintPreview();

public slots:
    /**
     * This slot asks web-page for an url to print and then starts the print routine
     * by calling the loadPrintPage methode.
     */
    void printKeyPressed();
};

#endif // PRINT_HANDLER_H
