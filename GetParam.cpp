//    File: GetParam.cpp
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

#include <ncurses.h>

#include "GetParam.h"
#include "Colours.h"

const std::string GetParam::m_GetParamText [] = {
    "< : MOVE LEFT",
    "> : MOVE RIGHT",
    "^ : MOVE UP",
    "v : MOVE DOWN",
    "H : HELP",
    "M : MENU",
    "Press any key!"
};

GetParam::GetParam()
{
    m_window = newwin(11, 18, LINES / 2, COLS / 2 - 7);
    box(m_window, '|', '-');
}

GetParam::~GetParam()
{
    delwin(m_window);
}

void GetParam::showMenu(void)
{
    keypad(m_window, TRUE);
    wattron(m_window, COLOR_PAIR(RED));
    wattron(m_window, A_UNDERLINE);
    wattron(m_window, A_BOLD);
    mvwprintw(m_window, 1, 5, "%s", "GetParam");
    wattroff(m_window, A_BOLD);
    wattroff(m_window, A_UNDERLINE);
    wattroff(m_window, COLOR_PAIR(RED));
    
    wattron(m_window, COLOR_PAIR(CYAN));
    for (int i = 0; i < 6; i++) {
        mvwprintw(m_window, i + 2, 2, "%s", m_GetParamText[i].c_str());
    }
    wattroff(m_window, COLOR_PAIR(CYAN));
    
    wattron(m_window, COLOR_PAIR(GREEN));
    mvwprintw(m_window, 9, 2, "%s", m_GetParamText[6].c_str());
    wattroff(m_window, COLOR_PAIR(GREEN));
    
    wrefresh(m_window);
    
    while (true) {
        if (getch()) {
            wclear(m_window);
            wrefresh(m_window);
            break;
        }
    }
    keypad(m_window, FALSE);
}
