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

#ifndef CONFIGFILEHANDLER_INCLUDE_H
#define CONFIGFILEHANDLER_INCLUDE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QUrl>
#include <QSettings>


/**
 * Singleton that handles the settings file.
 */
class ConfigFileHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * Get instance of singleton
     * @return instance
     */
    static ConfigFileHandler &getInstance();

    /**
     * Read the settings file.
     * If settings file doesn't exist it creates one and fills it with 
     * default data
     */
    void init();

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
     * Get x position of main window
     * @return left-position in pixel
     */
    int getWindowPosX();

    /**
     * Get y position of main window
     * @return top-position in pixel
     */
    int getWindowPosY();

    /**
     * Get width of main window
     * @return size in pixel
     */
    int getWindowSizeX();

    /**
     * Get height of main window
     * @return size in pixel
     */
    int getWindowSizeY();

    /**
     * Get stored application state
     * @return application state
     */
    int getWindowState();

    /**
     * Get permission if window is minimizable
     * @return true, if window is minimizable
     */
    bool getWindowIsMinimizable();

    /**
     * Get permission if window is maximizable
     * @return true, if window is maximizable
     */
    bool getWindowIsMaximizable();

    /**
     * Get permission if window is resizable
     * @return true, if window is resizable
     */
    bool getWindowIsResizable();

    /**
     * Get information if window is saved as fullscreen
     * @return true, if window is in fullscreen
     */
    bool getWindowIsFullscreen();

    /**
     * Set log level
     * @param val new log level
     */
    void setLogLevel(const unsigned val);

    /**
     * Set x position of main window
     * @param val position in pixel
     */
    void setWindowPosX(const int val);

    /**
     * Set y position of main window
     * @param val position in pixel
     */
    void setWindowPosY(const int val);

    /**
     * Set width of main window
     * @param val size in pixel
     */
    void setWindowSizeX(const int val);

    /**
     * Set height of main window
     * @param val size in pixel
     */
    void setWindowSizeY(const int val);

    /**
     * Set application state
     * @param val new application state
     */
    void setWindowState(const int val);

    /**
     * Set permission to minimize window
     * @param val permission
     */
    void setWindowIsMinimizable(const bool val);

    /**
     * Set permission to maximize window
     * @param val permission
     */
    void setWindowIsMaximizable(const bool val);

    /**
     * Set permission to resize window
     * @param val permission
     */
    void setWindowIsResizable(const bool val);

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

signals:
    /**
     * Signal when weppage-url changed
     */
    void signalWebPageChanged();
    
private:
    int log_level_;

    QString file_name_;
    QUrl url_;
    QString stun_;
    QString sound_file_name_;
    QString sound_dial_file_name_;

    QSettings settings_;

    ConfigFileHandler();
    ConfigFileHandler(const ConfigFileHandler&);
    virtual ~ConfigFileHandler();
};

#endif // CONFIGFILEHANDLER_INCLUDE_H
