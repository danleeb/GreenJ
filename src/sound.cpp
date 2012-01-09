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

#include "sound.h"

#include <QFile>

#include "config_file_handler.h"

//----------------------------------------------------------------------
Sound::Sound(void) :
    ring_(0), dial_(0)
{

    if (QFile::exists(ConfigFileHandler::getInstance().getSoundFilename()))
    {
        ring_ = new QSound(ConfigFileHandler::getInstance().getSoundFilename());
        ring_->setLoops(-1);
    }
    if (QFile::exists(ConfigFileHandler::getInstance().getSoundDialFilename()))
    {
        dial_ = new QSound(ConfigFileHandler::getInstance().getSoundDialFilename());
        dial_->setLoops(-1);
    }
}

//----------------------------------------------------------------------
Sound::~Sound(void)
{
    if (ring_)
        delete ring_;
    if (dial_)
        delete dial_;
}

//----------------------------------------------------------------------
Sound &Sound::getInstance()
{
    static Sound instance;
    return instance;
}

//----------------------------------------------------------------------
void Sound::startRing()
{
    if (ring_)
        ring_->play();
}


//----------------------------------------------------------------------
void Sound::startDialRing()
{
    if (dial_)
        dial_->play();
}


//----------------------------------------------------------------------
void Sound::stopRing()
{
    if (ring_)
        ring_->stop();
    if (dial_)
        dial_->stop();
}
