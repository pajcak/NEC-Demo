//    File: MenuWindow.h
//    Copyright (C) 2015  Patrik Faistaver
//    
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

#include <string>
#include <vector>
#include <ncurses.h>

#ifndef GAMEMENU_H
#define	GAMEMENU_H

using namespace std;

class MenuWindow {
public:
    MenuWindow();
    virtual ~MenuWindow();

    void printMenu(const int & selected);
    void printLogo(void);
    int showMenu(void);
    void hideMenu(void);
    void clearMenuWindow(void);
    void changeStateOfMenuItem(int choice);
private:
    vector<pair<string, bool> > m_MenuStrings;
    WINDOW * m_MenuWindow; //window with menu choices on the top of m_Window
    WINDOW * m_Window; // basic window with logo
    static const string m_Logo[];
    //-------------------FUNC-PTRS--------
    
};

#endif	/* GAMEMENU_H */

