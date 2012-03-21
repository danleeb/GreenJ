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

    /**
     * Get url of the webpage 
     * @return webpage url
     */
    const QUrl &getWebpageUrl() const;

    /**
     * Get address and port of the stun server
     * @return address of the stun server
     */
    const QString &getStunServer() const;

    /**
     * Get filename of ringing sound
     * @return ring-filename
     */
    const QString &getRingSoundFilename() const;

    /**
     * Get filename of dial sound
     * @return dial-filename
     */
    const QString &getDialSoundFilename() const;

    /**
     * Get stored log level
     * @return log level
     */
    unsigned int getLogLevel() const;

    /**
     * Get config version
     * @return config version
     */
    int getConfigVersion();

    /**
     * Get application version
     * @return Appication version as a string
     */
    QString getAppVersion();

    /**
     * Get name of application
     * @return application name
     */
    QString getAppName();

    /**
     * Get name of the developer
     * @return developer name
     */
    QString getAppDeveloper();

    /**
     * Set log level
     * @param val new log level
     */
    void setLogLevel(const unsigned val);

    /**
     * Get data of an option
     * @param name The name of the option that should be returned
     * @return Option data
     */
    QVariant getOption(const QString &name);

    /**
     * Set new data for an option
     * @param name The name of the option
     * @param option New data for the option
     */
    void setOption(const QString &name, const QVariant &option);
    
private:
    int log_level_;

    QString file_name_;
    QUrl url_;
    QString stun_;
    QString sound_file_name_;
    QString sound_dial_file_name_;

    QSettings settings_;

    /**
     * Read the settings file.
     * If settings file doesn't exist it creates one and fills it with 
     * default data
     */
    Config();
    Config(const Config&);
    virtual ~Config();
};

#endif // CONFIG_INCLUDE_H
