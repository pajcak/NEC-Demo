//    File: MenuWindow.cpp
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

#include "MenuWindow.h"
#include "GetParam.h"
#include "Colours.h"
#include "Dialogue.h"

const string MenuWindow::m_Logo [] = {
" _   _  _____ _____     ______ ________  ________ ",
"| \\ | ||  ___/  __ \\    |  _  \\  ___|  \\/  |  _  |",
"|  \\| || |__ | /  \\/    | | | | |__ | .  . | | | |",
"| . ` ||  __|| |        | | | |  __|| |\\/| | | | |",
"| |\\  || |___| \\__/\\    | |/ /| |___| |  | \\ \\_/ /",
"\\_| \\_/\\____/ \\____/    |___/ \\____/\\_|  |_/\\___/ "};

MenuWindow::MenuWindow()
{
    m_MenuStrings.push_back(pair<string, bool>("ADD MONITOR", true));
    m_MenuStrings.push_back(pair<string, bool>("DELETE MONITOR", true));
    m_MenuStrings.push_back(pair<string, bool>("CONNECT MONITOR", true));
    m_MenuStrings.push_back(pair<string, bool>("DISCONNECT MONITOR", true));
    m_MenuStrings.push_back(pair<string, bool>("CONNECT ALL", true));
    m_MenuStrings.push_back(pair<string, bool>("DISCONNECT ALL", true));
    m_MenuStrings.push_back(pair<string, bool>("IS CONNECTED", true));
    m_MenuStrings.push_back(pair<string, bool>("GET PARAMETER", true));
    m_MenuStrings.push_back(pair<string, bool>("SET PARAMETER", true));
    m_MenuStrings.push_back(pair<string, bool>("COMMAND", true));
    m_MenuStrings.push_back(pair<string, bool>("OPTIONS", true));
    m_MenuStrings.push_back(pair<string, bool>("EXIT", true));
    m_MenuWindow = newwin(18, 22, LINES / 2 - 5, COLS / 2 - 10);
    m_Window = newwin(LINES, COLS, 0, 0);
    box(m_MenuWindow, '|', '-');
    box(m_Window, '|', '-');
}

MenuWindow::~MenuWindow()
{
    delwin(m_MenuWindow);
    delwin(m_Window);
}

void MenuWindow::clearMenuWindow(void)
{
    wclear(m_MenuWindow);
    wrefresh(m_MenuWindow);
}
void MenuWindow::hideMenu(void)
{
    clearMenuWindow();
    wclear(m_Window);
    wrefresh(m_Window);
}

void MenuWindow::changeStateOfMenuItem(int choice) {
    m_MenuStrings.at(choice).second = !m_MenuStrings.at(choice).second;
}

void MenuWindow::printMenu(const int & selected)
{
    box(m_MenuWindow, '|', '-');
    wattron(m_MenuWindow, COLOR_PAIR(RED));
    wattron(m_MenuWindow, A_UNDERLINE);
    wattron(m_MenuWindow, A_BOLD);
    mvwprintw(m_MenuWindow, 1, 8, "%s", "MENU");
    wattroff(m_MenuWindow, A_BOLD);
    wattroff(m_MenuWindow, A_UNDERLINE);
    wattroff(m_MenuWindow, COLOR_PAIR(RED));
 	int y = 2;
    for (int i = 0; i < (int) m_MenuStrings.size(); i++) {
        y ++;
        if (y == 10 || y == 14) {mvwprintw(m_MenuWindow, y, 2, "%s", ""); y++;}    
        if (!m_MenuStrings[i].second) {
//            wattron(m_MenuWindow, COLOR_PAIR(BLACK))	;
            wattron(m_MenuWindow, A_INVIS);
            mvwprintw(m_MenuWindow, y, 2, "%s", m_MenuStrings[i].first.c_str());
            wattroff(m_MenuWindow, A_INVIS);
//            wattroff(m_MenuWindow, COLOR_PAIR(BLACK));
            continue;
        }
        if (i == selected) {
            wattron(m_MenuWindow, COLOR_PAIR(GREEN));
            mvwprintw(m_MenuWindow, y, 2, "%s", m_MenuStrings[i].first.c_str());
            wattroff(m_MenuWindow, COLOR_PAIR(GREEN));
        } else mvwprintw(m_MenuWindow, y, 2, "%s", m_MenuStrings[i].first.c_str());    
    }
    wrefresh(m_MenuWindow);
}

int MenuWindow::showMenu()
{
    box(m_Window, '|', '-');
    printLogo();
    clearMenuWindow();

    int selected = 0;
    m_MenuStrings[0].second ? selected = 0 : selected = 1;
    keypad(m_MenuWindow, TRUE);
    int key = 0;
    while (key != KEY_ENTER && key != '\n') {
        if (key == KEY_UP) {
            if (selected == 0) selected = m_MenuStrings.size() - 1;
            else selected--;
            while (!m_MenuStrings[selected].second) {
                selected--;
                if (selected < 0) selected = m_MenuStrings.size() - 1;
            }
        }
        if (key == KEY_DOWN) {
            if (selected == (int) m_MenuStrings.size() - 1) selected = 0;
            else selected++;
            while (!m_MenuStrings[selected].second) {
                selected++;
                if (selected == (int) m_MenuStrings.size() - 1) selected = 0;
            }
        }
        printMenu(selected);
        key = wgetch(m_MenuWindow);
    }
    
    keypad(m_MenuWindow, FALSE);
    return selected;
}

void MenuWindow::printLogo(void)
{
    wattron(m_Window, COLOR_PAIR(YELLOW));
    for (int i = 0; i < 6; i++) {
        mvwprintw(m_Window, i + 2, 15, "%s", m_Logo[i].c_str());
    }
    wattroff(m_Window, COLOR_PAIR(YELLOW));
    wrefresh(m_Window);
}
