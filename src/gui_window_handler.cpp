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

#include <QString>
#include "config_file_handler.h"
#include "gui.h"
#include "gui_window_handler.h"

//-----------------------------------------------------------------------------
GuiWindowHandler::GuiWindowHandler(Gui &gui) : gui_(gui)
{
}

//-----------------------------------------------------------------------------
GuiWindowHandler::~GuiWindowHandler()
{
}

//-----------------------------------------------------------------------------
void GuiWindowHandler::loadFromConfig()
{
    ConfigFileHandler &config = ConfigFileHandler::getInstance();

    gui_.setWindowTitle(config.getAppName());

    int w = config.getWindowSizeX();
    int h = config.getWindowSizeY();
    gui_.setGeometry(config.getWindowPosX(), config.getWindowPosY(), w, h);

    gui_.setWindowState((Qt::WindowState)config.getWindowState());

    if (!config.getWindowIsResizable()) {
        gui_.setFixedSize(w, h);
    }

    Qt::WindowFlags wflags = gui_.windowFlags();
    if (!config.getWindowIsMaximizable()) {
        wflags &= ~Qt::WindowMaximizeButtonHint;
    }
    if (!config.getWindowIsMinimizable()) {
        wflags &= ~Qt::WindowMinimizeButtonHint;
    }
    gui_.setWindowFlags(wflags);

    if (config.getWindowIsFullscreen()) {
        gui_.showFullScreen();
    }
}

//-----------------------------------------------------------------------------
void GuiWindowHandler::saveToConfig()
{
    ConfigFileHandler &config = ConfigFileHandler::getInstance();

    bool is_fullscreen = gui_.windowState() & Qt::WindowFullScreen;
    if (!is_fullscreen) {
        int x = gui_.x();
        int y = gui_.y();

        config.setWindowPosX(x < 0  ? 0  : x);
        config.setWindowPosY(y < 20 ? 20 : y);
        config.setWindowSizeX(gui_.width());
        config.setWindowSizeY(gui_.height());
        config.setWindowState((int)gui_.windowState());
    }

    config.setWindowIsResizable(gui_.minimumWidth() == gui_.maximumWidth());

    Qt::WindowFlags wflags = gui_.windowFlags();
    config.setWindowIsMaximizable(wflags.testFlag(Qt::WindowMaximizeButtonHint));
    config.setWindowIsMinimizable(wflags.testFlag(Qt::WindowMinimizeButtonHint));
}
