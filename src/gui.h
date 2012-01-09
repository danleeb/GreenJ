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

#ifndef GUI_H
#define GUI_H

#include <QtGui/QMainWindow>
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
 * This class is the base of the application.
 * It handles the main-window and it initializes the the classes
 * for communciation
 */
class Gui : public QMainWindow
{
    Q_OBJECT

private:
    Phone phone_;

    JavascriptHandler js_handler_;
    PrintHandler print_handler_;


    QAction *minimize_action_;
    QAction *maximize_action_;
    QAction *restore_action_;
    QAction *fullscreen_action_;
    QAction *quit_action_;

    QSystemTrayIcon *tray_icon_;
    QMenu *tray_icon_menu_;

    QShortcut *toggle_fullscreen_;
    QShortcut *print_;


    int current_state_;

    /**
     * Creates Actions for System Tray
     */
    void createActions();
    /**
     * Creates System Tray
     */
    void createTrayIcon();

public:
    /**
     * constructor of the gui
     * @param parent QWidget, the parent of the window, default 0
     * @param flags Qt::WFlags, flags for creating the window, default 0
     */
    Gui(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Gui();

    Ui::MainWindow ui_;

private slots:

    void linkClicked(const QUrl &url);

    /**
     * Toggles Full Screen
     */
    void toggleFullScreen();

    void printKeyPressed();

    /**
     * Register js_handler to Webkit. Allows Javascript to access qt-gui
     */
    void slotCreateJSWinObject();

    /**
     * If window has no focus, this function should tell user about incoming calls
     * (blinking, short message in system tray)
     *
     * @param url QString, number or url of caller
     */
    void alertIncomingCall(const QString &url);

    /**
     * load page into main window
     */
    void updateWebPage();
};

#endif // GUI_H
