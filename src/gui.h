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

#ifndef GUI_INCLUDE_H
#define GUI_INCLUDE_H

#include <QtGui>
#include <QString>
#include <QAction>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QShortcut>
#include "ui_gui.h"
#include "phone.h"
#include "javascript_handler.h"
#include "print_handler.h"

/**
 * This class represents the main window of the application.
 * It handles the gui of the main window and it initializes the classes
 * for communciation.
 */
class Gui : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent The parent of the window, defaults to null
     * @param flags Flags for creating the window, default 0
     */
    Gui(QWidget *parent = 0, Qt::WFlags flags = 0);

    /**
     * Destructor
     */
    virtual ~Gui();

    Ui::MainWindow &getWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void slotLinkClicked(const QUrl &url);
    void slotToggleFullScreen();
    void slotPrintKeyPressed();
    void slotPrintPage(const QUrl &url);

    /**
     * Register js_handler to Webkit. Allows Javascript to access qt-gui
     */
    void slotCreateJavascriptWindowObject();

    /**
     * If window has no focus, the user will be notified when an incoming 
     * call has been detected. (blinking short message in system tray)
     * @param url Number or url of caller
     */
    void slotAlertIncomingCall(const QString &url);

    /**
     * Load webpage in internal browser
     */
    void slotUpdateWebPage();
    
    /**
     * Prints errors, warning and debug messages
     * @param info Log information and message
     */
    void slotLogMessage(const LogInfo &info);

private:
    Ui::MainWindow ui_;

    Phone phone_;

    JavascriptHandler *js_handler_;
    PrintHandler print_handler_;

    QSystemTrayIcon *system_tray_icon_;

    void createSystemTray();
    void createShortcuts();

    void readSettings();
    void writeSettings();
};

#endif // GUI_INCLUDE_H
