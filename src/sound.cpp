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

#include <QFile>
#include "Config.h"
#include "Sound.h"

//-----------------------------------------------------------------------------
Sound::Sound() : ring_(0), dial_(0)
{
    Config &config = Config::getInstance();

    if (QFile::exists(config.getRingSoundFilename())) {
        ring_ = new QSound(config.getRingSoundFilename());
        ring_->setLoops(-1);
    }
    if (QFile::exists(config.getDialSoundFilename())) {
        dial_ = new QSound(config.getDialSoundFilename());
        dial_->setLoops(-1);
    }
}

//-----------------------------------------------------------------------------
Sound::~Sound()
{
    if (ring_) {
        delete ring_;
    }
    if (dial_) {
        delete dial_;
    }
}

//-----------------------------------------------------------------------------
Sound &Sound::getInstance()
{
    static Sound instance;
    return instance;
}

//-----------------------------------------------------------------------------
void Sound::startRing()
{
    if (ring_) {
        ring_->play();
    }
}

//-----------------------------------------------------------------------------
void Sound::startDial()
{
    if (dial_) {
        dial_->play();
    }
}

//-----------------------------------------------------------------------------
void Sound::stop()
{
    if (ring_) {
        ring_->stop();
    }
    if (dial_) {
        dial_->stop();
    }
}
