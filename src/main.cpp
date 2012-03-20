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

#include <QtGui/QApplication>
#include "gui.h"
#include "config_file_handler.h"


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":images/icon.xpm"));

    ConfigFileHandler &instance = ConfigFileHandler::getInstance();

    Gui w;
    w.show();

    return a.exec();
}
