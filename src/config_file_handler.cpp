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

#include <QMessageBox>
#include <QDir>
#include "log_info.h"
#include "config_file_handler.h"


//-----------------------------------------------------------------------------
ConfigFileHandler::ConfigFileHandler() :
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
        
        settings_.beginGroup("window");
        settings_.setValue("minimizable", "true");
        settings_.setValue("maximizable", "true");
        settings_.setValue("fullscreen", "true");
        settings_.setValue("resizable", "true");
        settings_.setValue("state", "2");
        settings_.endGroup();

        settings_.beginGroup("gui");
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

    settings_.beginGroup("gui");
    sound_file_name_ = settings_.value("soundfile").toString();
    sound_dial_file_name_ = settings_.value("sounddialfile").toString();
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
ConfigFileHandler::~ConfigFileHandler()
{
}

//-----------------------------------------------------------------------------
ConfigFileHandler &ConfigFileHandler::getInstance()
{
    static ConfigFileHandler instance;
    return instance;
}

//-----------------------------------------------------------------------------
const QUrl &ConfigFileHandler::getWebpageUrl() const
{
    return url_;
}

//-----------------------------------------------------------------------------
const QString &ConfigFileHandler::getStunServer() const
{
    return stun_;
}

//-----------------------------------------------------------------------------
const QString &ConfigFileHandler::getRingSoundFilename() const
{
    return sound_file_name_;
}

//-----------------------------------------------------------------------------
const QString &ConfigFileHandler::getDialSoundFilename() const
{
    return sound_dial_file_name_;
}

//-----------------------------------------------------------------------------
unsigned ConfigFileHandler::getLogLevel() const
{
    return log_level_;
}

//-----------------------------------------------------------------------------
int ConfigFileHandler::getConfigVersion()
{
    settings_.beginGroup("application");
    int ret = settings_.value("configversion").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
QString ConfigFileHandler::getAppVersion()
{
    settings_.beginGroup("application");
    QString ret = settings_.value("version").toString();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
QString ConfigFileHandler::getAppName()
{
    settings_.beginGroup("application");
    QString ret = settings_.value("name").toString();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
QString ConfigFileHandler::getAppDeveloper()
{
    settings_.beginGroup("application");
    QString ret = settings_.value("developer").toString();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
int ConfigFileHandler::getWindowPosX()
{
    settings_.beginGroup("window");
    int ret = settings_.value("posx").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
int ConfigFileHandler::getWindowPosY()
{
    settings_.beginGroup("window");
    int ret = settings_.value("posy").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
int ConfigFileHandler::getWindowSizeX()
{
    settings_.beginGroup("window");
    int ret = settings_.value("sizex").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
int ConfigFileHandler::getWindowSizeY()
{
    settings_.beginGroup("window");
    int ret = settings_.value("sizey").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
int ConfigFileHandler::getWindowState()
{
    settings_.beginGroup("window");
    int ret = settings_.value("state").toInt();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
bool ConfigFileHandler::getWindowIsMinimizable()
{
    settings_.beginGroup("window");
    bool ret = settings_.value("minimizeable").toBool();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
bool ConfigFileHandler::getWindowIsMaximizable()
{
    settings_.beginGroup("window");
    bool ret = settings_.value("maximizeable").toBool();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
bool ConfigFileHandler::getWindowIsResizable()
{
    settings_.beginGroup("window");
    bool ret = settings_.value("resizeable").toBool();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
bool ConfigFileHandler::getWindowIsFullscreen()
{
    settings_.beginGroup("window");
    bool ret = settings_.value("fullscreen").toBool();
    settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setLogLevel(const unsigned val)
{
    log_level_ = val;
    settings_.beginGroup("application");
    settings_.setValue("log_level", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowPosX(const int val)
{
    settings_.beginGroup("window");
    settings_.setValue("posx", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowPosY(const int val)
{
    settings_.beginGroup("window");
    settings_.setValue("posy", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowSizeX(const int val)
{
    settings_.beginGroup("window");
    settings_.setValue("sizex", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowSizeY(const int val)
{
    settings_.beginGroup("window");
    settings_.setValue("sizey", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowState(const int val)
{
    settings_.beginGroup("window");
    settings_.setValue("state", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowIsMinimizable(const bool val)
{
    settings_.beginGroup("window");
    settings_.setValue("minimizeable", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowIsMaximizable(const bool val)
{
    settings_.beginGroup("window");
    settings_.setValue("maximizeable", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
void ConfigFileHandler::setWindowIsResizable(const bool val)
{
    settings_.beginGroup("window");
    settings_.setValue("resizeable", val);
    settings_.endGroup();
}

//-----------------------------------------------------------------------------
QVariant ConfigFileHandler::getOption(const QString &name)
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
void ConfigFileHandler::setOption(const QString &name, const QVariant &option)
{
    if (name == "url") {
        url_ = option.toString();
        settings_.beginGroup("server");
        settings_.setValue("url", url_);
        settings_.endGroup();
        signalWebPageChanged();
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
