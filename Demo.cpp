#include <unistd.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "Demo.h"
#include "Dialogue.h"

Demo::Demo()
{
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
	
	
	m_controller = new CController();
	m_controller->initController();
	
	m_controller->addMonitor("127.0.0.1", 12345, 1);
	m_monitors[1] = std::make_pair("127.0.0.1", 12345);

	
	initParams();
	m_options.push_back(std::make_pair("Paralelism enabled", false));
			

    m_MenuWindow = new MenuWindow();
    
}

Demo::~Demo()
{
	m_controller->destroyController();
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
			
			stringstream ss; ss << "Add monitor: " << monitorID; 
			Dialogue dialS(ss.str().c_str(), "Monitor successfully added!", 25, 11);
		    dialS.showDialogue();
		    
		    touchwin(stdscr);
		    refresh();
			
		    goto start;
		    break;
		}
		case 1: //DELETE monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Delete monitor", 25, 10, m_monitors);
		    int monitID = dial.showMonitorDialogue();
		    if (m_monitors.find(monitID) != m_monitors.end()) {
		    	m_monitors.erase(monitID);
		    	try {
		    		m_controller->deleteMonitor(monitID);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
			    dial.hideDialogue();
		    
		    	stringstream ss; ss << "Delete monitor: " << monitID; 
				Dialogue dialS(ss.str().c_str(), "Monitor successfully deleted!", 25, 11);
		    	dialS.showDialogue();
		    }
		    
		    touchwin(stdscr);
		    refresh();
			
		    goto start;
		    break;
		}
		case 2: //CONNECT monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Connect monitor", 25, 10, m_monitors);
		    int monitID = dial.showMonitorDialogue();
		    bool res;
		    if (m_monitors.find(monitID) != m_monitors.end()) {
		    	try {
		    		res = m_controller->connectMonitor(monitID);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
		    }
		    dial.hideDialogue();
		    stringstream ss; ss << "Connect monitor: " << monitID;
		    Dialogue dialS(ss.str().c_str(), res ?
		    			"Monitor successfully connected!" : "Monitor could not connect!", 25, 11);
		    dialS.showDialogue();
		    
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 3: //DISCONNECT monitor
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Disconnect monitor", 25, 10, m_monitors);
		    int monitID = dial.showMonitorDialogue();
		    if (m_monitors.find(monitID) != m_monitors.end()) {
		    	try {
		    		m_controller->disconnectMonitor(monitID);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
		    }
		    dial.hideDialogue();
		    stringstream ss; ss << "Disconnect monitor: " << monitID;
		    Dialogue dialS(ss.str().c_str(), "Monitor successfully disconnected!", 25, 11);
		    dialS.showDialogue();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 4: //CONNECT ALL
		{
		    m_MenuWindow->hideMenu();
		    bool res;
		    try {
		    	res = m_controller->connectAll();
		    } catch ( char const* s) {
		    	Dialogue dialE("ERROR", s, 25, 10);
		    	dialE.setWarningColor();
		    	dialE.showDialogue();
		    	goto start;
		    }
		    Dialogue dial("Connect all", res ?
		    			"All monitors successfully connected!" : "Some of monitors could not connect!", 25, 11);
		    dial.showDialogue();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 5: //DISCONNECT ALL
		{
		    m_MenuWindow->hideMenu();
		    try {
		    	m_controller->disconnectAll();
		    } catch ( char const* s) {
		    	Dialogue dialE("ERROR", s, 25, 10);
		    	dialE.setWarningColor();
		    	dialE.showDialogue();
		    	goto start;
		    }
		    Dialogue dial("Disconnect all", "All monitors successfully disconnected!", 25, 11);
		    dial.showDialogue();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 6: //IS CONNECTED
		{
		    m_MenuWindow->hideMenu();
		    Dialogue dial("Is connected", 25, 10, m_monitors);
		    int res = dial.showMonitorDialogue();
		    bool b;
		    if (m_monitors.find(res) != m_monitors.end()) {
		    	try {
		    		b = m_controller->isConnected(res);
		    	} catch ( char const* s) {
		    		Dialogue dialE("ERROR", s, 25, 10);
		    		dialE.setWarningColor();
		    		dialE.showDialogue();
		    		goto start;
		    	}
		    	stringstream ss; ss << "Monitor: " << res;
		    	Dialogue dialSucc(ss.str().c_str(), b ? "Monitor is connected!" : "Monitor is disconnected!", 25, 10);
		    	dialSucc.showDialogue();
		    }
		    dial.hideDialogue();
		    touchwin(stdscr);
		    refresh();
		    goto start;
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
		case 10: //OPTIONS
		{
		    m_MenuWindow->hideMenu();
		    
			Dialogue d("OPTIONS (press SPACE to enable or disable)", 25, 11, &m_options);
			m_options = *d.showOptionsDialogue();
			
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		default: 
		{
		    m_MenuWindow->hideMenu();
		    break;
		}
    }
    touchwin(stdscr);
    refresh();
}
