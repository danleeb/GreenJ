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

#ifndef PRINTHANDLER_INCLUDE_H
#define PRINTHANDLER_INCLUDE_H

#include <QObject>
#include <QWebView>
#include <QUrl>


class Gui;
class JavascriptHandler;


/**
 * This class handles a print action.
 */
class PrintHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param gui Reference to the main gui
     * @param js_handler Reference to the Javascript handler
     */
    PrintHandler(Gui &gui, JavascriptHandler &js_handler);

    /**
     * Load a page to print
     * @param url The url to the printable webpage
     */
    void loadPrintPage(const QUrl &url);

public slots:
    /**
     * This slot requests an url to the printable webpage and then starts 
     * the print routine.
     */
    void printKeyPressed();

private slots:
    void showPrintPreview();

private:
    Gui &gui_;
    JavascriptHandler &js_handler_;
    QWebView print_page_;
};

#endif // PRINTHANDLER_INCLUDE_H
