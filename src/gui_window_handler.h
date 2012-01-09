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

#ifndef GUI_WINDOW_HANDLER_H
#define GUI_WINDOW_HANDLER_H

class Gui;

/**
 * Manages the position, size and restriction of the window-appearence
 * Those values get saved in the config file at shutdown and are loaded back on restart
 */
class GuiWindowHandler
{
private:
    Gui &the_gui_;

public:
    /**
     * Constructor taking reference to the GUI-Instance to manipulate it
     * \param gui The GUI
     */
    GuiWindowHandler(Gui &gui);
    ~GuiWindowHandler(void);

    /**
     * Loading the values from the config-file and set the GUI Appearence
     */
    void loadFromConfig();

    /**
     * Saving the values from the GUI to the config-file
     */
    void saveToConfig();
};


#endif
