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

#include "config_file_handler.h"

#include <QMessageBox>
#include <QDir>
#include "log_info.h"

//----------------------------------------------------------------------
ConfigFileHandler::ConfigFileHandler() :
    file_name_(QDir::homePath()+"/.greenj/settings.conf"), url_(""),
    my_settings_(file_name_,QSettings::IniFormat)
{
}

//----------------------------------------------------------------------
ConfigFileHandler::~ConfigFileHandler()
{
}

//----------------------------------------------------------------------
ConfigFileHandler &ConfigFileHandler::getInstance()
{
    static ConfigFileHandler instance;
    return instance;
}


//----------------------------------------------------------------------
void ConfigFileHandler::init()
{
    if (!QFile::exists(file_name_))
    {
        // create basic config
        my_settings_.beginGroup("application");
        my_settings_.setValue("configversion", "1");
        my_settings_.setValue("appversion", "1.0");
        my_settings_.setValue("appname", "GreenJ");
        my_settings_.setValue("developer", "Lorem Ipsum");
        my_settings_.setValue("app_minimizeable", "true");
        my_settings_.setValue("app_maximizeable", "true");
        my_settings_.setValue("app_fullscreenable", "true");
        my_settings_.setValue("app_resizeable", "true");
        my_settings_.setValue("app_state", "2");
        my_settings_.setValue("log_level", LogInfo::STATUS_WARNING);
        my_settings_.endGroup();

        my_settings_.beginGroup("gui");
        my_settings_.setValue("soundfile", "ring.wav");
        my_settings_.setValue("sounddialfile", "dial_tone.wav");
        my_settings_.endGroup();

        // url to the web-page
        my_settings_.beginGroup("server");
        my_settings_.setValue("url", "phone/index.html");
        my_settings_.setValue("stun", "");
        my_settings_.endGroup();
    }

    my_settings_.beginGroup("application");
    log_level_ = my_settings_.value("log_level").toUInt();
    my_settings_.endGroup();

    my_settings_.beginGroup("server");
    url_ = my_settings_.value("url").toUrl();
    stun_ = my_settings_.value("stun").toString();
    my_settings_.endGroup();

    my_settings_.beginGroup("gui");
    sound_file_name_ = my_settings_.value("soundfile").toString();
    sound_dial_file_name_ = my_settings_.value("sounddialfile").toString();
    my_settings_.endGroup();
}

//----------------------------------------------------------------------
const QUrl &ConfigFileHandler::getServerUrl() const
{
    return url_;
}

//----------------------------------------------------------------------
const QString &ConfigFileHandler::getStunServer() const
{
    return stun_;
}

//----------------------------------------------------------------------
const QString &ConfigFileHandler::getSoundFilename() const
{
    return sound_file_name_;
}

//----------------------------------------------------------------------
const QString &ConfigFileHandler::getSoundDialFilename() const
{
    return sound_dial_file_name_;
}

//----------------------------------------------------------------------
unsigned ConfigFileHandler::getLogLevel() const
{
    return log_level_;
}

//-----------------------------------------------------------------------
int ConfigFileHandler::getConfigVersion()
{
    my_settings_.beginGroup("application");
    int ret =  my_settings_.value("configversion").toInt();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
QString ConfigFileHandler::getAppVersion()
{
    my_settings_.beginGroup("application");
    QString ret =  my_settings_.value("appversion").toString();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
QString ConfigFileHandler::getAppName()
{
    my_settings_.beginGroup("application");
    QString ret =  my_settings_.value("appname").toString();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
QString ConfigFileHandler::getDeveloper()
{
    my_settings_.beginGroup("application");
    QString ret =  my_settings_.value("developer").toString();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
int ConfigFileHandler::getAppPosX()
{
    my_settings_.beginGroup("application");
    int ret =  my_settings_.value("app_posx").toInt();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
int ConfigFileHandler::getAppPosY()
{
    my_settings_.beginGroup("application");
    int ret =  my_settings_.value("app_posy").toInt();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
int ConfigFileHandler::getAppSizeX()
{
    my_settings_.beginGroup("application");
    int ret =  my_settings_.value("app_sizex").toInt();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
int ConfigFileHandler::getAppSizeY()
{
    my_settings_.beginGroup("application");
    int ret =  my_settings_.value("app_sizey").toInt();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
int ConfigFileHandler::getAppState()
{
    my_settings_.beginGroup("application");
    int ret =  my_settings_.value("app_state").toInt();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
bool ConfigFileHandler::getAppIsMinimizeable()
{
    my_settings_.beginGroup("application");
    bool ret =  my_settings_.value("app_minimizeable").toBool();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
bool ConfigFileHandler::getAppIsMaximizeable()
{
    my_settings_.beginGroup("application");
    bool ret =  my_settings_.value("app_maximizeable").toBool();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
bool ConfigFileHandler::getAppIsResizeable()
{
    my_settings_.beginGroup("application");
    bool ret =  my_settings_.value("app_resizeable").toBool();
    my_settings_.endGroup();
    return ret;
}

//-----------------------------------------------------------------------
bool ConfigFileHandler::getAppIsFullscreen()
{
    my_settings_.beginGroup("application");
    bool ret =  my_settings_.value("app_fullscreen").toBool();
    my_settings_.endGroup();
    return ret;
}

//----------------------------------------------------------------------
void ConfigFileHandler::setLogLevel(const unsigned &val)
{
    log_level_ = val;
    my_settings_.beginGroup("application");
    my_settings_.setValue("log_level", val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppPosX(const int &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_posx",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppPosY(const int &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_posy",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppSizeX(const int &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_sizex",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppSizeY(const int &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_sizey",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppState(const int &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_state",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppIsMinimizeable(const bool &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_minimizeable",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppIsMaximizeable(const bool &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_maximizeable",val);
    my_settings_.endGroup();
}

//-----------------------------------------------------------------------
void ConfigFileHandler::setAppIsResizeable(const bool &val)
{
    my_settings_.beginGroup("application");
    my_settings_.setValue("app_resizeable",val);
    my_settings_.endGroup();
}

//----------------------------------------------------------------------
QVariant ConfigFileHandler::getOption(const QString &name)
{
    QVariant result(0);

    if (name == "url")
        result.setValue(getServerUrl());

    if (name == "stun")
        result.setValue(getStunServer());

    if (name == "log_level")
        result.setValue(log_level_);

    return result;
}

//----------------------------------------------------------------------
void ConfigFileHandler::setOption(const QString &name, const QVariant &option)
{
    if (name == "url")
    {
        url_ =  option.toString();
        my_settings_.beginGroup("server");
        my_settings_.setValue("url",url_);
        my_settings_.endGroup();
        signalWebPageChanged();
    }
    if (name == "stun")
    {
        stun_ =  option.toString();
        my_settings_.beginGroup("server");
        my_settings_.setValue("stun",stun_);
        my_settings_.endGroup();
    }
    if (name == "log_level")
    {
        log_level_ = option.toInt();
        my_settings_.beginGroup("application");
        my_settings_.setValue("log_level",log_level_);
        my_settings_.endGroup();
    }
}
