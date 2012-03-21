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
#include "log_info.h"
#include "config.h"

//-----------------------------------------------------------------------------
Config::Config() :
    file_name_(QDir::homePath() + "/.greenj/settings.conf"), url_(""),
    settings_(file_name_, QSettings::IniFormat)
{
    // Create basic config if file does not exist
    if (!QFile::exists(file_name_)) {
        settings_.beginGroup("application");
        settings_.setValue("configversion", "1");
        settings_.setValue("version", "1.0");
        settings_.setValue("name", "GreenJ");
        settings_.setValue("developer", "Lorem Ipsum");
        settings_.setValue("log_level", LogInfo::STATUS_WARNING);
        settings_.endGroup();

        settings_.beginGroup("sound");
        settings_.setValue("soundfile", "ring.wav");
        settings_.setValue("sounddialfile", "dial_tone.wav");
        settings_.endGroup();

        settings_.beginGroup("server");
        settings_.setValue("url", "phone/index.html");
        settings_.setValue("stun", "");
        settings_.endGroup();
    }

    settings_.beginGroup("application");
    log_level_ = settings_.value("log_level").toUInt();
    settings_.endGroup();

    settings_.beginGroup("server");
    url_ = settings_.value("url").toUrl();
    stun_ = settings_.value("stun").toString();
    settings_.endGroup();

    settings_.beginGroup("sound");
    sound_file_name_ = settings_.value("soundfile").toString();
    sound_dial_file_name_ = settings_.value("sounddialfile").toString();
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
Config::~Config()
{
}

//-----------------------------------------------------------------------------
Config &Config::getInstance()
{
    static Config instance;
    return instance;
}

//-----------------------------------------------------------------------------
const QUrl &Config::getWebpageUrl() const
{
    return url_;
}

//-----------------------------------------------------------------------------
const QString &Config::getStunServer() const
{
    return stun_;
}

//-----------------------------------------------------------------------------
const QString &Config::getRingSoundFilename() const
{
    return sound_file_name_;
}

//-----------------------------------------------------------------------------
const QString &Config::getDialSoundFilename() const
{
    return sound_dial_file_name_;
}

//-----------------------------------------------------------------------------
unsigned Config::getLogLevel() const
{
    return log_level_;
}

//-----------------------------------------------------------------------------
int Config::getConfigVersion()
{
    settings_.beginGroup("application");
    int ret = settings_.value("configversion").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
QString Config::getAppVersion()
{
    settings_.beginGroup("application");
    QString ret = settings_.value("version").toString();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
QString Config::getAppName()
{
    settings_.beginGroup("application");
    QString ret = settings_.value("name").toString();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
QString Config::getAppDeveloper()
{
    settings_.beginGroup("application");
    QString ret = settings_.value("developer").toString();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
void Config::setLogLevel(const unsigned val)
{
    log_level_ = val;
    settings_.beginGroup("application");
    settings_.setValue("log_level", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
QVariant Config::getOption(const QString &name)
{
    QVariant result(0);

    if (name == "url") {
        result.setValue(getWebpageUrl());
    } else if (name == "stun") {
        result.setValue(getStunServer());
    } else if (name == "log_level") {
        result.setValue(log_level_);
    }

    return result;
}

//-----------------------------------------------------------------------------
void Config::setOption(const QString &name, const QVariant &option)
{
    if (name == "url") {
        url_ = option.toString();
        settings_.beginGroup("server");
        settings_.setValue("url", url_);
        settings_.endGroup();
    } else if (name == "stun") {
        stun_ = option.toString();
        settings_.beginGroup("server");
        settings_.setValue("stun", stun_);
        settings_.endGroup();
    } else if (name == "log_level") {
        log_level_ = option.toInt();
        settings_.beginGroup("application");
        settings_.setValue("log_level", log_level_);
        settings_.endGroup();
    }
}
