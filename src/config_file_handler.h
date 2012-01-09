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

#ifndef CONFIG_FILE_HANDLER_H
#define CONFIG_FILE_HANDLER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QUrl>
#include <QSettings>

/**
 * This class is implemented as singleton.
 * It handles the settings file
 */
class ConfigFileHandler : public QObject
{
    Q_OBJECT

private:
    int log_level_;

    QString file_name_;
    QUrl url_;
    QString stun_;
    QString sound_file_name_;
    QString sound_dial_file_name_;

    QSettings my_settings_;

    ConfigFileHandler();
    ConfigFileHandler(const ConfigFileHandler&);
    ~ConfigFileHandler();

public:
    /**
     * Get instance of Singelton class
     * @return ConfigFileHandler the instance to this class
     */
    static ConfigFileHandler &getInstance();

    /**
     * Read the settings file
     * if settings file doesn't exist it creates one
     *    and fills it with default data
     */
    void init();

    /**
     * Get url of phone-webpage 
     * @return QUrl the phone-webpage location
     */
    const QUrl &getServerUrl() const;

    /**
     * Get address and port to stun-server
     * @return QString the address to stun-server
     */
    const QString &getStunServer() const;

    /**
     * Get filename of ringing sound
     * @return QString the ring-filename
     */
    const QString &getSoundFilename() const;

    /**
     * Get filename of dial sound
     * @return QString the dial-filename
     */
    const QString &getSoundDialFilename() const;

    /**
     * get stored log level
     * @return unsigned the log level
     */
    unsigned getLogLevel() const;

    /**
     * get config version
     * @return int the config version
     */
    int getConfigVersion();

    /**
     * get application version
     * @return QString the appication version
     */
    QString getAppVersion();

    /**
     * get name of application
     * @return QString the application name
     */
    QString getAppName();

    /**
     * get developers
     * @return QString the developers
     */
    QString getDeveloper();

    /**
     * Get saved position left of window
     * @return int position left in pixel
     */
    int getAppPosX();

    /**
     * Get saved position top of window
     * @return int position top in pixel
     */
    int getAppPosY();

    /**
     * Get saved horizontal size of window
     * @return int size in pixel
     */
    int getAppSizeX();

    /**
     * Get saved vertical size of window
     * @return int size in pixel
     */
    int getAppSizeY();

    /**
     * Get stored application state
     * @return int, the application state
     */
    int getAppState();

    /**
     * Get permission to minimize window
     * @return bool the permission
     */
    bool getAppIsMinimizeable();

    /**
     * Get permission to maximize window
     * @return bool the permission
     */
    bool getAppIsMaximizeable();

    /**
     * Get permission to resize window
     * @return bool the permission
     */
    bool getAppIsResizeable();

    /**
     * Ask if window is saved as fullscreen
     * @return bool 
     */
    bool getAppIsFullscreen();

    /**
     * Set log level
     * @param val int, the log level
     */
    void setLogLevel(const unsigned &val);

    /**
     * Set position left of window
     * @param val int, the position in pixel
     */
    void setAppPosX(const int &val);

    /**
     * Set position top of window
     * @param val int, the position in pixel
     */
    void setAppPosY(const int &val);

    /**
     * Set horizontal size of window
     * @param val int, the size in pixel
     */
    void setAppSizeX(const int &val);

    /**
     * Set vertical size of window
     * @param val int, the size in pixel
     */
    void setAppSizeY(const int &val);

    /**
     * Set application state
     * @param val int, the application state
     */
    void setAppState(const int &val);

    /**
     * Set permission to minimize window
     * @param val bool, permission
     */
    void setAppIsMinimizeable(const bool &val);

    /**
     * Set permission to maximize window
     * @param val bool, permission
     */
    void setAppIsMaximizeable(const bool &val);

    /**
     * Set permission to resize window
     * @param val bool, permission
     */
    void setAppIsResizeable(const bool &val);

    /**
     * get data of an option
     * @param name QString, the name of the option
     * @return QString the option data
     */
    QVariant getOption(const QString &name);

    /**
     * set new data to an option
     * @param name QString, the name of the option
     * @param option QString, the new data of the option
     */
    void setOption(const QString &name, const QVariant &option);

signals:
    /**
     * signals when weppage-url changesp
     */
    void signalWebPageChanged();
};

#endif // CONFIG_FILE_HANDLER_H
