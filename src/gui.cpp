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

#include <QApplication>
#include <QWebFrame>
#include <QWebInspector>
#include "config_file_handler.h"
#include "gui_window_handler.h"
#include "log_info.h"
#include "log_handler.h"
#include "sip_phone.h"
#include "web_page.h"
#include "gui.h"

//-----------------------------------------------------------------------------
Gui::Gui(QWidget *parent, Qt::WFlags flags) : 
    QMainWindow(parent, flags), 
    phone_(new SipPhone), 
    js_handler_(phone_),
    print_handler_(*this, js_handler_)
{
    qRegisterMetaType<LogInfo>("LogInfo");
    ui_.setupUi(this);
    
    ConfigFileHandler &config = ConfigFileHandler::getInstance();

    connect(&LogHandler::getInstance(), SIGNAL(signalLogMessage(const LogInfo&)),
            &js_handler_,               SLOT(logMessageSlot(const LogInfo&)));

    connect(&phone_,                    SIGNAL(signalIncomingCall(const QString&)),
            this,                       SLOT(alertIncomingCall(const QString&)));

    // Set window appearence
    GuiWindowHandler window_handler(*this);
    window_handler.loadFromConfig();

    // Setting up phone
    phone_.init(&js_handler_);

    WebPage *page = new WebPage();
    ui_.webview->setPage(page);

    js_handler_.init(ui_.webview, &print_handler_);

#ifndef DEBUG
    // deactivate right click context menu
    ui_.webview->setContextMenuPolicy(Qt::NoContextMenu);
#else
    // Enable webkit Debugger
    ui_.webview->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
#endif

    //ui_.webview->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui_.webview->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), 
            this,                             SLOT(slotCreateJSWinObject()));
    
    connect(ui_.webview, SIGNAL(linkClicked(const QUrl&)), 
            this,        SLOT(linkClicked(const QUrl&)));

    connect(&config, SIGNAL(signalWebPageChanged()),
            this,    SLOT(updateWebPage()));

    const QUrl &server_url = config.getWebpageUrl();
    if (!server_url.isEmpty()) {
        ui_.webview->setUrl(server_url);
    }

    createSystemTray();

    // Shortcuts
    toggle_fullscreen_ = new QShortcut(Qt::Key_F11, this, SLOT(toggleFullScreen()));
    print_ = new QShortcut(Qt::CTRL + Qt::Key_P, this, SLOT(printKeyPressed()));
}

//-----------------------------------------------------------------------------
Gui::~Gui()
{
    phone_.unregister();

    // Save window appearence
    GuiWindowHandler window_handler(*this);
    window_handler.saveToConfig();
}

//-----------------------------------------------------------------------------
Ui::MainWindow &Gui::getWindow()
{
    return ui_;
}

void Gui::linkClicked(const QUrl &url)
{
    ui_.webview->load(url);
}

//-----------------------------------------------------------------------------
void Gui::createSystemTray()
{
    minimize_action_ = new QAction(tr("Mi&nimize"), this);
    connect(minimize_action_, SIGNAL(triggered()), this, SLOT(hide()));
  
    maximize_action_ = new QAction(tr("Ma&ximize"), this);
    connect(maximize_action_, SIGNAL(triggered()), this, SLOT(showMaximized()));
  
    restore_action_ = new QAction(tr("&Restore"), this);
    connect(restore_action_, SIGNAL(triggered()), this, SLOT(showNormal()));
  
    fullscreen_action_ = new QAction(tr("&FullScreen"), this);
    connect(fullscreen_action_, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

    quit_action_ = new QAction(tr("&Quit"), this);
    connect(quit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));

    tray_icon_menu_ = new QMenu(this);
    tray_icon_menu_->addAction(minimize_action_);
    tray_icon_menu_->addAction(maximize_action_);
    tray_icon_menu_->addAction(restore_action_);
    tray_icon_menu_->addAction(fullscreen_action_);
    tray_icon_menu_->addSeparator();
    tray_icon_menu_->addAction(quit_action_);

    tray_icon_ = new QSystemTrayIcon(this);
    tray_icon_->setContextMenu(tray_icon_menu_);
    tray_icon_->setIcon(QIcon(":images/icon.xpm"));
    tray_icon_->show();
}

//-----------------------------------------------------------------------------
void Gui::toggleFullScreen()
{
    bool is_fullscreen = windowState() & Qt::WindowFullScreen;
    if (is_fullscreen) {
        showNormal();
        setWindowState((Qt::WindowState)current_state_);
    } else {
        current_state_ = windowState();
        showFullScreen();
    }
}

//-----------------------------------------------------------------------------
void Gui::printKeyPressed()
{
    print_handler_.printKeyPressed();
}

//-----------------------------------------------------------------------------
void Gui::slotCreateJSWinObject()
{
    ui_.webview->page()->mainFrame()
        ->addToJavaScriptWindowObject("qt_handler", &js_handler_);
}

//-----------------------------------------------------------------------------
void Gui::alertIncomingCall(const QString &url)
{
    QApplication::alert(this);
    if (!QApplication::focusWidget()) {
        tray_icon_->showMessage("Anruf", url+" versucht Sie zu kontaktieren");
    }
}

//-----------------------------------------------------------------------------
void Gui::updateWebPage()
{
    const QUrl &server_url = ConfigFileHandler::getInstance().getWebpageUrl();
    if (!server_url.isEmpty()) {
        ui_.webview->setUrl(server_url);
    }
}
