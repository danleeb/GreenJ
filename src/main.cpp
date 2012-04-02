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
#include "phone/Phone.h"
#include "phone/api/Sip.h"
#include "Gui.h"

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":images/icon.xpm"));

    phone::Phone phone(new phone::api::Sip());
    if (phone.init()) {
        Gui window(phone);
        window.show();
        return a.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Initialization failed.");
        msgBox.setInformativeText(phone.getErrorMessage());
#ifdef DEBUG
        msgBox.setDetailedText("See log files...");
#endif
        msgBox.exec();
    }
    return 1;
}
