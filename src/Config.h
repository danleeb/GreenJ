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

#ifndef CONFIG_INCLUDE_H
#define CONFIG_INCLUDE_H

#include <QString>
#include <QUrl>
#include <QSettings>

/**
 * Singleton that handles configuration settings.
 */
class Config
{
public:
    /**
     * Get instance of singleton
     * @return Instance
     */
    static Config &getInstance();
    
    const int getConfigVersion() const;

    const QString getApplicationVersion() const;
    const QString getApplicationName() const;
    const QString getApplicationDeveloper() const;
    const uint getApplicationLogLevel() const;

    const uint getWindowMinimumWidth() const;
    const uint getWindowMinimumHeight() const;

    const QUrl getBrowserUrl() const;

    const uint getPhonePort() const;
    const QString getPhoneStunServer() const;
    const float getPhoneSoundLevel() const;
    const float getPhoneMicroLevel() const;

    const QString getSoundRingfile() const;
    const QString getSoundDialfile() const;

    /**
     * Set log level
     * @param val new log level
     */
    void setLogLevel(const uint val);

    /**
     * Get data of an option
     * @param name The name of the option that should be returned
     * @return Option data
     */
    QVariant getOption(const QString &name) const;

    /**
     * Set new data for an option
     * @param name The name of the option
     * @param option New data for the option
     */
    void setOption(const QString &name, const QVariant &option);
    
private:
    static const QString SETTINGS_FILE;

    QString file_name_;
    QSettings settings_;

    /**
     * Read the settings file.
     * If settings file doesn't exist it creates one and fills it with 
     * default data
     */
    Config();
    Config(const Config &original);
    virtual ~Config();

    /**
     * Set default configuration settings
     */
    void setDefaults();
};

#endif // CONFIG_INCLUDE_H
