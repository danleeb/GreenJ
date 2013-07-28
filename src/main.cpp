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
#include "Config.h"
#include "phone/Phone.h"
#include "phone/api/Sip.h"
#include "Gui.h"

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":images/icon.xpm"));

    Config &config = Config::getInstance();
    phone::Settings settings;
    settings.port_ = config.getPhonePort();
    settings.stun_server_ = config.getPhoneStunServer();
    settings.sound_level_ = config.getPhoneSoundLevel();
    settings.micro_level_ = config.getPhoneMicroLevel();
    settings.srtp = (phone::Settings::Srtp)config.getPhoneSrtp();
    settings.srtp_signaling = (phone::Settings::SrtpSignaling)config.getPhoneSrtpSignaling();

    phone::Phone phone(new phone::api::Sip());
    if (phone.init(settings)) {
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
