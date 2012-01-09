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

#ifndef SOUND_H
#define SOUND_H

#include <QObject>
#include <QSound>

/**
 * This class handles sounds
 */
class Sound : QObject
{
    Q_OBJECT

private:
    QSound *ring_;
    QSound *dial_;

    Sound(void);
    Sound(const Sound &copy);
    ~Sound(void);

public:
    /**
     * get the instance of the object
     * @return Sound& the instance of the object
     */
    static Sound &getInstance();

    /**
     * start ring sound
     */
    void startRing();

    /**
     * start dial sound
     */
    void startDialRing();

    /**
     * stop sounds
     */
    void stopRing();

signals:
    /**
     * send a signal do start ring sound
     */
    void signalStartRing();
    /**
     * send a signal do stop all running sounds
     */
    void signalStopRing();
    /**
     * send a signal do start dial sound
     */
    void signalStartDialRing();
    
};
#endif // SOUND_H
