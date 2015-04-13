#include "MenuWindow.h"
#include "../NEC-Controller/headers/CController.h"
#include <map>

#ifndef Demo_H
#define	Demo_H

using namespace std;

class Demo {
public:
    Demo();
    ~Demo();
    void startMenu();
private:
	void initParams();
	bool findDuplicate(int id, string adddr) const;
	
	CController * m_controller;
    MenuWindow * m_MenuWindow;
    map<string, void(*)> m_Parameters;
    map<int, pair<string, int>> m_monitors;
    vector<pair<string,bool>> m_options;
};

#endif	/* Demo_H */

