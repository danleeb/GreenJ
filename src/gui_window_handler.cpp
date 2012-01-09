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

#include "gui_window_handler.h"
#include "gui.h"
#include "config_file_handler.h"
#include <QString>

//----------------------------------------------------------------------
GuiWindowHandler::GuiWindowHandler(Gui &gui) :
    the_gui_(gui)
{
}

//----------------------------------------------------------------------
void GuiWindowHandler::loadFromConfig()
{
    QString str;

    //Window Title
    str = ConfigFileHandler::getInstance().getAppName();
    the_gui_.setWindowTitle(str);

    //Window position & size
    int x = ConfigFileHandler::getInstance().getAppPosX();
    int y = ConfigFileHandler::getInstance().getAppPosY();
    int w = ConfigFileHandler::getInstance().getAppSizeX();
    int h = ConfigFileHandler::getInstance().getAppSizeY();
    the_gui_.setGeometry(x,y,w,h);

    int state = ConfigFileHandler::getInstance().getAppState();
    the_gui_.setWindowState((Qt::WindowState)state);

    //Window restrictions
    bool is_minizeable, is_maximizeable, is_resizeable, is_fullscreen;
    is_minizeable = ConfigFileHandler::getInstance().getAppIsMinimizeable();
    is_maximizeable = ConfigFileHandler::getInstance().getAppIsMaximizeable();
    is_resizeable = ConfigFileHandler::getInstance().getAppIsResizeable();
    is_fullscreen = ConfigFileHandler::getInstance().getAppIsFullscreen();

    //Resizeable
    if (is_resizeable == false)
        the_gui_.setFixedSize(w,h);

    //Maximizeable
    if (is_maximizeable == false)
    {
        Qt::WindowFlags wflags = the_gui_.windowFlags() & ~Qt::WindowMaximizeButtonHint;
        the_gui_.setWindowFlags(wflags);
    }

    //Minimizeable
    if (is_minizeable == false)
    {
        Qt::WindowFlags wflags = the_gui_.windowFlags() & ~Qt::WindowMinimizeButtonHint;
        the_gui_.setWindowFlags(wflags);
    }

    //Fullscreen
    if (is_fullscreen)
    {
        the_gui_.showFullScreen();
    }
}

//----------------------------------------------------------------------
void GuiWindowHandler::saveToConfig()
{
    bool is_fullscreen = the_gui_.windowState() & Qt::WindowFullScreen;
    if (!is_fullscreen)
    {
        int x = the_gui_.x();
        int y = the_gui_.y();

        if (x<0)
            x=0;
        if (y<20)
            y=20;

        ConfigFileHandler::getInstance().setAppPosX(x);
        ConfigFileHandler::getInstance().setAppPosY(y);
        ConfigFileHandler::getInstance().setAppSizeX(the_gui_.width());
        ConfigFileHandler::getInstance().setAppSizeY(the_gui_.height());
        ConfigFileHandler::getInstance().setAppState((int)the_gui_.windowState());
    }

    //Resizeable
    bool is_resizeable =true;
    if (the_gui_.minimumWidth() == the_gui_.maximumWidth())
        is_resizeable = false;
    ConfigFileHandler::getInstance().setAppIsResizeable(is_resizeable);

    //Maximizeable
    Qt::WindowFlags wflags = the_gui_.windowFlags();
    bool is_maximizeable= wflags.testFlag(Qt::WindowMaximizeButtonHint);
    ConfigFileHandler::getInstance().setAppIsMaximizeable(is_maximizeable);

    //Minimizeable
    bool is_minimizeable= wflags.testFlag(Qt::WindowMinimizeButtonHint);
    ConfigFileHandler::getInstance().setAppIsMinimizeable(is_minimizeable);

}


//----------------------------------------------------------------------
GuiWindowHandler::~GuiWindowHandler(void)
{
}
