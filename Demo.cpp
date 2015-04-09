#include <unistd.h>
#include <sstream>
#include <fstream>

#include "Demo.h"
#include "Dialogue.h"

Demo::Demo()
{
	m_controller = new CController();
    initscr();
    cbreak();
    noecho();
    start_color();
    curs_set(0);
    keypad(stdscr, TRUE);
    refresh();
    init_pair(0, COLOR_BLACK, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	
	initParams();

    m_MenuWindow = new MenuWindow();
}

Demo::~Demo()
{
	delete m_controller;
    delete m_MenuWindow;
    keypad(stdscr, FALSE);
    endwin();
}
    
void Demo::initParams() {
//	m_Parameters["Backlight"] = ;
//	m_Parameters["Contrast"] = ;
//	m_Parameters["Sharpness"] = ;
//	m_Parameters["Brightness"] = ;
//	m_Parameters["Hue"] = ;
//	m_Parameters["Color paleness"] = ;
//	m_Parameters["Color temeprature"] = ;
//	m_Parameters["Color control"] = ;
//	m_Parameters["Gamma correction"] = ;
//	m_Parameters["Volume"] = ;
}

bool Demo::findDuplicate(int id, string addr) const {
	for (map<int, pair<string, int>>::const_iterator it = m_monitors.begin(); it != m_monitors.end(); ++it) {
		if (it->first == id || it->second.first == addr) return true;
	}
	return false;
}
void Demo::startMenu(void)
{
start:
    int choice = m_MenuWindow->showMenu();
    switch (choice) {
		case 0: //ADD monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue addDialogue("Add monitor (address port id)", "", 15, 10);
		    string res = addDialogue.showAddMonitorDialogue();
		    if (res.length() < 1) {
		    	addDialogue.hideDialogue();
				goto start;
		    }
		    std::size_t firstSpace = res.find_first_of(' ');
		    string addr = res.substr(0, firstSpace);
		    int port = atoi(res.substr(firstSpace+1, res.find_first_of(' ', firstSpace+1)).c_str());
		    int monitorID = atoi(res.substr(res.find_first_of(' ', firstSpace+1)+1).c_str());
		    if (findDuplicate(monitorID, addr)) {
				addDialogue.hideDialogue();
				Dialogue d("ERROR", "This monitor address or ID already exists!", 25, 10);
				d.setWarningColor();
				d.showDialogue();
				touchwin(stdscr);
				refresh();
				goto start;
		    }    
		    try {
				m_controller->addMonitor(addr.c_str(), port, monitorID);
			} catch (char const* s) {
				addDialogue.hideDialogue();
				Dialogue d("EXCEPTION", s, 25, 10);
				d.setWarningColor();
				d.showDialogue();
				goto start;
			}
			m_monitors[monitorID] = std::make_pair(addr, port);
			
			addDialogue.hideDialogue();
		    touchwin(stdscr);
		    refresh();
			
		    goto start;
		    break;
		}
		case 1: //DELETE monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Delete monitor", 25, 10, m_monitors);
		    int res = dial.showMonitorDialogue();
		    if (m_monitors.find(res) != m_monitors.end()) {
		    	m_monitors.erase(res);
		    	try {
		    		m_controller->deleteMonitor(res);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
		    }
		    dial.hideDialogue();
		    touchwin(stdscr);
		    refresh();
			
		    goto start;
		    break;
		}
		case 2: //CONNECT monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Connect monitor", 25, 10, m_monitors);
		    int res = dial.showMonitorDialogue();
		    if (m_monitors.find(res) != m_monitors.end()) {
		    	try {
		    		m_controller->connectMonitor(res);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
		    }
		    dial.hideDialogue();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 3: //DISCONNECT monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Disconnect monitor", 25, 10, m_monitors);
		    int res = dial.showMonitorDialogue();
		    if (m_monitors.find(res) != m_monitors.end()) {
		    	try {
		    		m_controller->disconnectMonitor(res);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
		    }
		    dial.hideDialogue();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 4: //CONNECT ALL
		{
		    m_MenuWindow->hideMenu();
		    
		    touchwin(stdscr);
		    refresh();
		    break;
		}
		case 5: //DISCONNECT ALL
		{
		    m_MenuWindow->hideMenu();
		    touchwin(stdscr);
		    refresh();
		    break;
		}
		case 6: //IS CONNECTED
		{
		    m_MenuWindow->hideMenu();
		    touchwin(stdscr);
		    refresh();
		    break;
		}
		case 7: //GET PARAM
		{
		    m_MenuWindow->hideMenu();
		    touchwin(stdscr);
		    refresh();
		    break;
		}
		case 8: //SET PARAM
		{
		    m_MenuWindow->hideMenu();
		    touchwin(stdscr);
		    refresh();
		    break;
		}
		case 9: //COMMAND
		{
		    m_MenuWindow->hideMenu();
		    touchwin(stdscr);
		    refresh();
		    break;
		}
		default: 
		{
		    m_MenuWindow->hideMenu();
		    touchwin(stdscr);
		    refresh();
		    break;
		}
    }
    touchwin(stdscr);
    refresh();
}