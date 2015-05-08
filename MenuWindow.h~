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

