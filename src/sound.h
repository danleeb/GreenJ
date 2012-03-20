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

#ifndef SOUND_INCLUDE_H
#define SOUND_INCLUDE_H

#include <QObject>
#include <QSound>


/**
 * Singleton that handles sounds
 */
class Sound : QObject
{
    Q_OBJECT

public:
    /**
     * Get the instance of the object
     * @return Instance of the object
     */
    static Sound &getInstance();

    /**
     * Start ring sound
     */
    void startRing();

    /**
     * Start dial sound
     */
    void startDial();

    /**
     * Stop sounds
     */
    void stop();

signals:
    /**
     * Send signal to start ring sound
     */
    void signalStartRing();

    /**
     * Send signal to stop all running sounds
     */
    void signalStop();

    /**
     * Send signal to start dial sound
     */
    void signalStartDial();

private:
    QSound *ring_;
    QSound *dial_;

    Sound(void);
    Sound(const Sound &original);
    ~Sound(void);
};
#endif // SOUND_INCLUDE_H
