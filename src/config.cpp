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

#include <QDir>
#include "LogHandler.h"
#include "Config.h"

const QString Config::SETTINGS_FILE = "/.greenj/settings.conf";

//-----------------------------------------------------------------------------
Config::Config() :
    file_name_(QDir::homePath() + SETTINGS_FILE), 
    settings_(file_name_, QSettings::IniFormat)
{
    if (!QFile::exists(file_name_) || getConfigVersion() != 11) {
        settings_.clear();
        setDefaults();
    }
}

//-----------------------------------------------------------------------------
Config::~Config()
{
}

//-----------------------------------------------------------------------------
void Config::setDefaults()
{
    settings_.setValue("configversion", 11);
    
    settings_.beginGroup("application");
    settings_.setValue("version", "1.0");
    settings_.setValue("name", "GreenJ");
    settings_.setValue("developer", "Lorem Ipsum");
    settings_.setValue("log_level", LogInfo::STATUS_WARNING);
    settings_.endGroup();

    settings_.beginGroup("sound");
    settings_.setValue("ringfile", "ring.wav");
    settings_.setValue("dialfile", "dial_tone.wav");
    settings_.endGroup();

    settings_.beginGroup("browser");
    settings_.setValue("url", "phone/index.html");
    settings_.endGroup();

    settings_.beginGroup("phone");
    settings_.setValue("port", 5060);
    settings_.setValue("stun_server", "");
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
Config &Config::getInstance()
{
    static Config instance;
    return instance;
}

//-----------------------------------------------------------------------------
const int Config::getConfigVersion() const
{
    return settings_.value("configversion").toInt();
}

//-----------------------------------------------------------------------------
const QString Config::getApplicationVersion() const
{
    return settings_.value("application/version").toString();
}

//-----------------------------------------------------------------------------
const QString Config::getApplicationName() const
{
    return settings_.value("application/name").toString();
}

//-----------------------------------------------------------------------------
const QString Config::getApplicationDeveloper() const
{
    return settings_.value("application/developer").toString();
}

//-----------------------------------------------------------------------------
const uint Config::getApplicationLogLevel() const
{
    return settings_.value("application/log_level").toUInt();
}

//-----------------------------------------------------------------------------
const QUrl Config::getBrowserUrl() const
{
    return settings_.value("browser/url").toString();
}

//-----------------------------------------------------------------------------
const uint Config::getPhonePort() const
{
    return settings_.value("phone/port").toUInt();
}

//-----------------------------------------------------------------------------
const QString Config::getPhoneStunServer() const
{
    return settings_.value("phone/stun_server").toString();
}

//-----------------------------------------------------------------------------
const QString Config::getSoundRingfile() const
{
    return settings_.value("sound/ringfile").toString();
}

//-----------------------------------------------------------------------------
const QString Config::getSoundDialfile() const
{
    return settings_.value("sound/dialfile").toString();
}

//-----------------------------------------------------------------------------
void Config::setLogLevel(const uint val)
{
    settings_.setValue("application/log_level", val);
}

//-----------------------------------------------------------------------------
QVariant Config::getOption(const QString &name) const
{
    QVariant result;

    if (name == "url") {
        result.setValue(getBrowserUrl());
    } else if (name == "stun_server") {
        result.setValue(getPhoneStunServer());
    } else if (name == "log_level") {
        result.setValue(getApplicationLogLevel());
    }
    return result;
}

//-----------------------------------------------------------------------------
void Config::setOption(const QString &name, const QVariant &option)
{
    if (name == "url") {
        settings_.setValue("browser/url", option.toString());
    } else if (name == "stun_server") {
        settings_.setValue("phone/stun_server", option.toString());
    } else if (name == "log_level") {
        settings_.setValue("application/log_level", option.toInt());
    }
}
