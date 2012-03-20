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
    virtual ~Gui();

    Ui::MainWindow &getWindow();

private slots:
    void linkClicked(const QUrl &url);
    void toggleFullScreen();
    void printKeyPressed();

    /**
     * Register js_handler to Webkit. Allows Javascript to access qt-gui
     */
    void slotCreateJSWinObject();

    /**
     * If window has no focus, the user will be notified when an incoming 
     * call has been detected. (blinking short message in system tray)
     * @param url Number or url of caller
     */
    void alertIncomingCall(const QString &url);

    /**
     * Load webpage in internal browser
     */
    void updateWebPage();

private:
    Ui::MainWindow ui_;

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

    void createSystemTray();
};

#endif // GUI_INCLUDE_H
