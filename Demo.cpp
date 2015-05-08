//    File: Demo.cpp
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

#include <unistd.h>
#include <sstream>
#include <fstream>
#include <cstdio>
#include "Demo.h"
#include "Dialogue.h"
#include <dlfcn.h>
#include <unistd.h>
#include <cstdio>

Demo::Demo()
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	    string libPath(cwd);
		libPath.append("/controller.so");
		loadLibrary(libPath.c_str());
//		printf(">%s<\n", libPath.c_str());	
	} else throw "Cannot locate library!";
    
	m_initController();
	
	initMonitors();
	initOptions();

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
		
    m_MenuWindow = new MenuWindow();
    
}

Demo::~Demo()
{
	m_destroyController();
	
	dlclose(m_Library);
	
    delete m_MenuWindow;
    keypad(stdscr, FALSE);
    endwin();
}

void Demo::initOptions() {
	m_options.push_back(std::make_pair("Paralelism enabled", false));
}
void Demo::initMonitors() {
//	m_addMonitor("127.0.0.1", 12345, 666);
//	m_monitors[666] = std::make_pair("127.0.0.1", 12345);
//	m_addMonitor("127.0.0.1", 12346, 777);
//	m_monitors[777] = std::make_pair("127.0.0.1", 12346);
	m_addMonitor("10.0.0.100", 7142, 1);
	m_monitors[1] = std::make_pair("10.0.0.100", 7142);
	m_addMonitor("10.0.0.101", 7142, 2);
	m_monitors[2] = std::make_pair("10.0.0.101", 7142);
	m_addMonitor("10.0.0.102", 7142, 3);
	m_monitors[3] = std::make_pair("10.0.0.102", 7142);
	m_addMonitor("10.0.0.103", 7142, 4);
	m_monitors[4] = std::make_pair("10.0.0.103", 7142);
	m_addMonitor("10.0.0.104", 7142, 5);
	m_monitors[5] = std::make_pair("10.0.0.104", 7142);
	m_addMonitor("10.0.0.105", 7142, 6);
	m_monitors[6] = std::make_pair("10.0.0.105", 7142);
	m_addMonitor("10.0.0.106", 7142, 7);
	m_monitors[7] = std::make_pair("10.0.0.106", 7142);
	m_addMonitor("10.0.0.107", 7142, 8);
	m_monitors[8] = std::make_pair("10.0.0.107", 7142);
	m_addMonitor("10.0.0.108", 7142, 9);
	m_monitors[9] = std::make_pair("10.0.0.108", 7142);
	m_addMonitor("10.0.0.109", 7142, 10);
	m_monitors[10] = std::make_pair("10.0.0.109", 7142);
	m_addMonitor("10.0.0.110", 7142, 11);
	m_monitors[11] = std::make_pair("10.0.0.110", 7142);
	m_addMonitor("10.0.0.111", 7142, 12);
	m_monitors[12] = std::make_pair("10.0.0.111", 7142);
	m_addMonitor("10.0.0.112", 7142, 13);
	m_monitors[13] = std::make_pair("10.0.0.112", 7142);
	m_addMonitor("10.0.0.113", 7142, 14);
	m_monitors[14] = std::make_pair("10.0.0.113", 7142);
	m_addMonitor("10.0.0.114", 7142, 15);
	m_monitors[15] = std::make_pair("10.0.0.114", 7142);
	m_addMonitor("10.0.0.115", 7142, 16);
	m_monitors[16] = std::make_pair("10.0.0.115", 7142);
	m_addMonitor("10.0.0.116", 7142, 17);
	m_monitors[17] = std::make_pair("10.0.0.116", 7142);
	m_addMonitor("10.0.0.117", 7142, 18);
	m_monitors[18] = std::make_pair("10.0.0.117", 7142);
	m_addMonitor("10.0.0.118", 7142, 19);
	m_monitors[19] = std::make_pair("10.0.0.118", 7142);
	m_addMonitor("10.0.0.119", 7142, 20);
	m_monitors[20] = std::make_pair("10.0.0.119", 7142);
}
    
void Demo::loadLibrary(const char * libName) {	
	m_Library = dlopen ( libName, RTLD_NOW | RTLD_GLOBAL );
   	if(m_Library  == NULL) fprintf(stderr, "%s\n", dlerror());
//------------------------------------------------------------------
	m_initController = ( void (*)()) dlsym ( m_Library, "initController" );
   if ( !m_initController ) fprintf(stderr, "%s\n", dlerror());
   
	m_destroyController = ( void (*)()) dlsym ( m_Library, "destroyController" );
   if ( !m_destroyController ) fprintf(stderr, "%s\n", dlerror());
	
	m_addMonitor = ( void (*)(const char*, int, int)) dlsym ( m_Library, "addMonitor" );
   if ( !m_addMonitor ) fprintf(stderr, "%s\n", dlerror());
	
	m_deleteMonitor = ( void (*)(int)) dlsym ( m_Library, "deleteMonitor" );
   if ( !m_deleteMonitor ) fprintf(stderr, "%s\n", dlerror());
	
	m_connectMonitor = ( bool (*)(int)) dlsym ( m_Library, "connectMonitor" );
   if ( !m_connectMonitor ) fprintf(stderr, "%s\n", dlerror());
	
	m_disconnectMonitor = ( void (*)(int)) dlsym ( m_Library, "disconnectMonitor" );
   if ( !m_disconnectMonitor ) fprintf(stderr, "%s\n", dlerror());
	
	m_connectAll = ( bool (*)()) dlsym ( m_Library, "connectAll" );
   if ( !m_connectAll ) fprintf(stderr, "%s\n", dlerror());
	
	m_disconnectAll = ( void (*)()) dlsym ( m_Library, "disconnectAll" );
   if ( !m_disconnectAll ) fprintf(stderr, "%s\n", dlerror());
	
	m_isConnected = ( bool (*)(int)) dlsym ( m_Library, "isConnected" );
   if ( !m_isConnected ) fprintf(stderr, "%s\n", dlerror());
   
   	//------param-functions-----
	m_GetBacklightFunc = ( int (*)(int)) dlsym ( m_Library, "getBacklight" );
   if ( !m_GetBacklightFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_SetBacklightFunc = ( void (*)(int, int)) dlsym ( m_Library, "setBacklight" );
   if ( !m_SetBacklightFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetContrastFunc = ( int (*)(int)) dlsym ( m_Library, "getContrast" );
   if ( !m_GetContrastFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetContrastFunc = ( void (*)(int, int)) dlsym ( m_Library, "setContrast" );
   if ( !m_SetContrastFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetSharpnessFunc = ( int (*)(int)) dlsym ( m_Library, "getSharpness" );
   if ( !m_GetSharpnessFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetSharpnessFunc = ( void (*)(int, int)) dlsym ( m_Library, "setSharpness" );
   if ( !m_SetSharpnessFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetBrightnessFunc = ( int (*)(int)) dlsym ( m_Library, "getBrightness" );
   if ( !m_GetBrightnessFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetBrightnessFunc = ( void (*)(int, int)) dlsym ( m_Library, "setBrightness" );
   if ( !m_SetBrightnessFunc ) fprintf(stderr, "%s\n", dlerror());

	m_GetHueFunc = ( int (*)(int)) dlsym ( m_Library, "getHue" );
   if ( !m_GetHueFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetHueFunc = ( void (*)(int, int)) dlsym ( m_Library, "setHue" );
   if ( !m_SetHueFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetPalenessFunc = ( int (*)(int)) dlsym ( m_Library, "getPaleness" );
   if ( !m_GetPalenessFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetPalenessFunc = ( void (*)(int, int)) dlsym ( m_Library, "setPaleness" );
   if ( !m_SetPalenessFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetColorTemperatureFunc = ( int (*)(int)) dlsym ( m_Library, "getColorTemperature" );
   if ( !m_GetColorTemperatureFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetColorTemperatureFunc = ( void (*)(int, int)) dlsym ( m_Library, "setColorTemperature" );
   if ( !m_SetColorTemperatureFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetGammaCorrectionFunc = ( int (*)(int)) dlsym ( m_Library, "getGammaCorrection" );
   if ( !m_GetGammaCorrectionFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetGammaCorrectionFunc = ( void (*)(int, int)) dlsym ( m_Library, "setGammaCorrection" );
   if ( !m_SetGammaCorrectionFunc ) fprintf(stderr, "%s\n", dlerror());
	
	m_GetVolumeFunc = ( int (*)(int)) dlsym ( m_Library, "getVolume" );
   if ( !m_GetVolumeFunc ) fprintf(stderr, "%s\n", dlerror());

	m_SetVolumeFunc = ( void (*)(int, int)) dlsym ( m_Library, "setVolume" );
   if ( !m_SetVolumeFunc ) fprintf(stderr, "%s\n", dlerror());

    //-------COMMAND-FUNCTIONS-------	
	m_PowerStatusReadFunc = ( int (*)(int)) dlsym ( m_Library, "powerStatusRead" );
   if ( !m_PowerStatusReadFunc ) fprintf(stderr, "%s\n", dlerror());

	m_PowerControlFunc = ( void (*)(int, int)) dlsym ( m_Library, "powerControl" );
   if ( !m_PowerControlFunc ) fprintf(stderr, "%s\n", dlerror());
        
	m_FunctionNames.push_back("Backlight");
	m_FunctionNames.push_back("Contrast");
	m_FunctionNames.push_back("Sharpness");
	m_FunctionNames.push_back("Brightness");
	m_FunctionNames.push_back("Hue");
	m_FunctionNames.push_back("Paleness");
	m_FunctionNames.push_back("Color temperature");
	m_FunctionNames.push_back("Gamma correction");
	m_FunctionNames.push_back("Volume");
	m_CommandNames.push_back("Read power status");
	m_CommandNames.push_back("Change power status");
}

bool Demo::findDuplicate(int id, string addr, int port) const {
	for (map<int, pair<string, int>>::const_iterator it = m_monitors.begin(); it != m_monitors.end(); ++it) {
		if (it->first == id || (it->second.first == addr && it->second.second == port)) return true;
	}
	return false;
}

void Demo::addMonitorChoice() {
	m_MenuWindow->hideMenu();
	Dialogue addDialogue("Add monitor (address port id)", "", 15, 10);
	string res = addDialogue.showAddMonitorDialogue();
	if (res.length() < 1) {
	   	addDialogue.hideDialogue();
		return;
	}
	std::size_t firstSpace = res.find_first_of(' ');
	string addr = res.substr(0, firstSpace);
	int port = atoi(res.substr(firstSpace+1, res.find_first_of(' ', firstSpace+1)).c_str());
	int monitorID = atoi(res.substr(res.find_first_of(' ', firstSpace+1)+1).c_str());
	if (findDuplicate(monitorID, addr, port)) {
		addDialogue.hideDialogue();
		Dialogue d("ERROR", "This monitor with same (address, port) pair or ID already exists!", 25, 10);
		d.showDialogue(1, 1);
		touchwin(stdscr);
		refresh();
		return;
	}    
	try {
	   	m_addMonitor(addr.c_str(), port, monitorID);
	} catch (char const* s) {
		addDialogue.hideDialogue();
		Dialogue d("EXCEPTION", s, 25, 10);
		d.showDialogue(1, 1);
		return;
	}
	m_monitors[monitorID] = std::make_pair(addr, port);
	addDialogue.hideDialogue();
	
	stringstream ss; ss << "Add monitor: " << monitorID; 
	Dialogue dialS(ss.str().c_str(), "Monitor successfully added!", 25, 11);
	dialS.showDialogue(1, 0);
}
void Demo::deleteMonitorChoice() {

    int monitorDialogue_X = 18;
    int monitorDialogue_Y = 2;
    
	m_MenuWindow->hideMenu();
	Dialogue dial("Delete monitors (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
	vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
	dial.hideDialogue();
		
	vector<string> monitorsDone;
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin(); it != monitDialResult.end(); ++it)
	 {
		if (m_monitors.find(it->first) != m_monitors.end()) {
			m_monitors.erase(it->first);
			try {
				m_deleteMonitor(it->first);
			} catch ( char const* s) {
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				return;
			}
			stringstream ss; ss << "Monitor " << it->first << " successfully deleted!";
			monitorsDone.push_back(ss.str());	
		}
	 }
	if (monitorsDone.size() > 0) {
		Dialogue dialS("Delete monitors", monitorDialogue_X + 8, monitorDialogue_Y, monitorsDone);
		dialS.showDialogue(1, 0);
	}    
}
void Demo::connectMonitorChoice() {
    int monitorDialogue_X = 16;
    int monitorDialogue_Y = 2;

	m_MenuWindow->hideMenu();
	Dialogue dial("Connect monitor (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
	vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();	    
	dial.hideDialogue();
	vector<string> monitorsDone;
		    
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin(); it != monitDialResult.end(); ++it)
	{
		if (m_monitors.find(it->first) != m_monitors.end()) {
			try {
				if (!m_connectMonitor(it->first)) {
					stringstream ss; ss << "Monitor " << it->first << " could not connect!";
					monitorsDone.push_back(ss.str());	
				} else {
					stringstream ss; ss << "Monitor " << it->first << " successfully connected!";
					monitorsDone.push_back(ss.str());	
				}
			} catch ( char const* s) {
				dial.hideDialogue();
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				return;
			}
		}
	}
	if (monitorsDone.size() > 0) {
		Dialogue dialS("Connect monitors", monitorDialogue_X + 8, monitorDialogue_Y, monitorsDone);
		dialS.showDialogue(1, 0);
	}
}
void Demo::disconnectMonitorChoice() {
	int monitorDialogue_X = 16;
    int monitorDialogue_Y = 2;
    m_MenuWindow->hideMenu();
	Dialogue dial("Disonnect monitors (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
	vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
	dial.hideDialogue();
	vector<string> monitorsDone;
	   
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin(); it != monitDialResult.end(); ++it)
	{
		if (m_monitors.find(it->first) != m_monitors.end()) {
			try {
				m_disconnectMonitor(it->first);
				stringstream ss; ss << "Monitor " << it->first << " disconnected!";
				monitorsDone.push_back(ss.str());
			} catch ( char const* s) {
				dial.hideDialogue();
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				return;
			}
		}
	}
	if (monitorsDone.size() > 0) {
		Dialogue dialS("Disconnect monitors", monitorDialogue_X + 8, monitorDialogue_Y, monitorsDone);
		dialS.showDialogue(1, 0);
	}
}
void Demo::connectAllChoice() {
	m_MenuWindow->hideMenu();
	bool res;
	try {
	   	res = m_connectAll();
	} catch ( char const* s) {
	   	Dialogue dialE("ERROR", s, 25, 10);
	   	dialE.showDialogue(1, 1);
	   	return;
	}
	Dialogue dial("Connect all", res ?
	   			"All monitors successfully connected!" : "Some of monitors could not connect!", 25, 11);
	dial.showDialogue(1, 0);
}
void Demo::disconnectAllChoice() {
	m_MenuWindow->hideMenu();
	try {
	   	m_disconnectAll();
	} catch ( char const* s) {
	   	Dialogue dialE("ERROR", s, 25, 10);
	   	dialE.showDialogue(1, 1);
	   	return;
	}
	Dialogue dial("Disconnect all", "All monitors successfully disconnected!", 25, 11);
	dial.showDialogue(1, 0);
}
void Demo::isConnectedChoice() {
	int monitorDialogue_X = 16;
    int monitorDialogue_Y = 2;
    
    m_MenuWindow->hideMenu();
	Dialogue dial("Is connected (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
	vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
	dial.hideDialogue();
	vector<string> monitorsDone;
	   
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin(); it != monitDialResult.end(); ++it)
	{
		if (m_monitors.find(it->first) != m_monitors.end()) {
			try {
				if (m_isConnected(it->first)) {
					stringstream ss; ss << "Monitor " << it->first << " is connected!";
					monitorsDone.push_back(ss.str());	
				} else {
					stringstream ss; ss << "Monitor " << it->first << " is not connected!";
					monitorsDone.push_back(ss.str());	
				}
			} catch ( char const* s) {
				dial.hideDialogue();
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				return;
			}
		}
	}
	if (monitorsDone.size() > 0) {
		Dialogue dialS("Is connected", monitorDialogue_X + 8, monitorDialogue_Y, monitorsDone);
		dialS.showDialogue(1, 0);
	}
}
void Demo::getParameterParallel(vector<pair<int, bool>> & monitDialResult, int (*getterFunction) (int),
									int paramNum,
									vector<string> & monitorsDone) {
    	int thrCount = monitDialResult.size();
    	pthread_t * threads = new pthread_t[thrCount];
    	pthread_mutex_t printMtx;
		pthread_mutex_t dialogueMtx;
		pthread_mutex_init(&dialogueMtx, NULL);
		pthread_mutex_init(&printMtx, NULL);
		
		pthread_attr_t threadAttr;
		pthread_attr_init(&threadAttr);
		pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
		struct getterThrFuncAttr * attrArray[thrCount];
		
		for (int i = 0; i < thrCount; i++)
		{
			attrArray[i] = new struct getterThrFuncAttr(
												this,
												getterFunction,
												&monitorsDone,
												&printMtx,
												&dialogueMtx,
												monitDialResult[i].first, paramNum);

			pthread_create(&threads[i], NULL, (void*(*)(void*))&Demo::helperGetterFunc, attrArray[i]);
		}

		for (int i = 0; i < thrCount; i++) {
			pthread_join(threads[i], NULL);
		}
		
		for (int i = 0; i < thrCount; i++) delete attrArray[i];
		pthread_mutex_destroy(&printMtx);
		pthread_mutex_destroy(&dialogueMtx);
		pthread_attr_destroy(&threadAttr);  
		delete [] threads;
}
void Demo::getParameterSequential(vector<pair<int, bool>> & monitDialResult, int (*getterFunction) (int),
											int paramNum,
											vector<string> & monitorsDone) {
		for (vector<pair<int, bool>>::iterator it = monitDialResult.begin(); it != monitDialResult.end(); ++it)
		{
			if (m_monitors.find(it->first) != m_monitors.end()) {
				try {
					if ( !m_isConnected(it->first) ) {
						stringstream ss; ss << "Monitor " << it->first << " is not connected!";
						monitorsDone.push_back(ss.str());
						continue;
					}
					int result = getterFunction(it->first);
					stringstream ss;
					ss << m_FunctionNames[paramNum] << " of monitor "
						<< it->first << " is " << result << '.';
					monitorsDone.push_back(ss.str());
				} catch ( char const* s) {
					Dialogue dialE("ERROR", s, 25, 10);
					dialE.showDialogue(1, 1);
					dialE.hideDialogue();
					
					stringstream ss; ss << m_FunctionNames[paramNum] << " of monitor "
						<< it->first << " could not have been read.";
					monitorsDone.push_back(ss.str());
//					return;
				}
			}
		}
}
void Demo::getParameterChoice() {
    int monitorDialogue_X = 16;
    int monitorDialogue_Y = 2;
    
    m_MenuWindow->hideMenu();
    //parameter choice
    Dialogue chooseParam("Choose parameter", 25, 11, m_FunctionNames);
    int paramNum = chooseParam.showDialogue(0,0);
    chooseParam.hideDialogue();
    if (paramNum == -1) return;
    
    //monitor choice
    Dialogue dial("Choose monitor (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
    vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
    dial.hideDialogue();
    if (monitDialResult.size() == 0) {
		touchwin(stdscr);
		refresh();
		return;
    }
    
    vector<string> monitorsDone;
    
    int (* getterFunction)(int);
    switch(paramNum) {
    	case 0: { getterFunction = m_GetBacklightFunc; 			break;}
    	case 1: { getterFunction = m_GetContrastFunc; 			break;}
    	case 2: { getterFunction = m_GetSharpnessFunc; 			break;}
    	case 3: { getterFunction = m_GetBrightnessFunc;			break;}
    	case 4: { getterFunction = m_GetHueFunc; 				break;}
    	case 5: { getterFunction = m_GetPalenessFunc; 			break;}
    	case 6: { getterFunction = m_GetColorTemperatureFunc; 	break;}
    	case 7: { getterFunction = m_GetGammaCorrectionFunc; 	break;}
    	case 8: { getterFunction = m_GetVolumeFunc; 			break;}
    }
    if (m_options[0].second) { //parallelism enabled
		getParameterParallel(monitDialResult, getterFunction, paramNum, monitorsDone);
	} else { 
		getParameterSequential(monitDialResult, getterFunction, paramNum, monitorsDone);
	}
	
	Dialogue dialS("Get parameter", monitorDialogue_X, monitorDialogue_Y, monitorsDone);
	dialS.showDialogue(1, 0);
}
void Demo::setParameterParallel(vector<pair<int, bool>> & monitDialResult, void (*setterFunction) (int, int),
											int paramNum, int paramValue,
											vector<string> & monitorsDone) {
    	int thrCount = monitDialResult.size();
    	
    	pthread_t * threads = new pthread_t[thrCount];
    	pthread_mutex_t printMtx;
		pthread_mutex_t dialogueMtx;
		pthread_mutex_init(&dialogueMtx, NULL);
		pthread_mutex_init(&printMtx, NULL);
		
		pthread_attr_t threadAttr;
		pthread_attr_init(&threadAttr);
		pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
		struct setterThrFuncAttr * attrArray[thrCount];
		for (int i = 0; i < thrCount; i++)
		{
			attrArray[i] = new setterThrFuncAttr( this,
												setterFunction,
												&monitorsDone,
												&printMtx,
												&dialogueMtx,
												monitDialResult[i].first,
												paramNum, paramValue);
			
			pthread_create(&threads[i], NULL, (void*(*)(void*))&Demo::helperSetterFunc, attrArray[i]);
		}		
		for (int i = 0; i < thrCount; i++) {
			pthread_join(threads[i], NULL);
		}
		
		for (int i = 0; i < thrCount; i++) delete attrArray[i];
		pthread_mutex_destroy(&printMtx);
		pthread_mutex_destroy(&dialogueMtx);
		pthread_attr_destroy(&threadAttr);  
		delete [] threads;
}
void Demo::setParameterSequential(vector<pair<int, bool>> & monitDialResult, void (*setterFunction) (int, int),
											int paramNum, int paramValue,
											vector<string> & monitorsDone) {
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin(); it != monitDialResult.end(); ++it)
	{
		if (m_monitors.find(it->first) != m_monitors.end()) {
			try {
				if ( !m_isConnected(it->first) ) {
					stringstream ss; ss << "Monitor " << it->first << " is not connected!";
					monitorsDone.push_back(ss.str());
					continue;
				}
				setterFunction(it->first, paramValue);
				stringstream ss;
				ss << m_FunctionNames[paramNum] << " of monitor "
					<< it->first << " successfully changed to " << paramValue << '.';
				monitorsDone.push_back(ss.str());
			} catch ( char const* s) {
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				dialE.hideDialogue();
				
				stringstream ss; ss << m_FunctionNames[paramNum] << " of monitor "
				<< it->first << " could not have been changed.";
				monitorsDone.push_back(ss.str());
//				return;
			}
		}
	}
}
void Demo::setParameterChoice() {
    int monitorDialogue_X = 16;
    int monitorDialogue_Y = 2;

    m_MenuWindow->hideMenu();
    //param choice
    Dialogue chooseParam("Choose parameter", 25, 11, m_FunctionNames);
    int paramNum = chooseParam.showDialogue(0,0);
    chooseParam.hideDialogue();
    if (paramNum == -1) return;
  
    //value choice
    Dialogue getNumber("Enter parameter value!", "", 15, 10);
    int paramValue = getNumber.showGetNumberDialogue();
    getNumber.hideDialogue();
    if (paramValue == -1) return;
    
    //monitor choice
    Dialogue dial("Choose monitor (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
    vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
    dial.hideDialogue();
    if (monitDialResult.size() == 0) {
		touchwin(stdscr);
		refresh();
		return;
    }
    
    vector<string> monitorsDone;
    
    void (* setterFunction)(int, int);
    switch(paramNum) {
    	case 0: { setterFunction = m_SetBacklightFunc; 			break;}
    	case 1: { setterFunction = m_SetContrastFunc; 			break;}
    	case 2: { setterFunction = m_SetSharpnessFunc; 			break;}
    	case 3: { setterFunction = m_SetBrightnessFunc;			break;}
    	case 4: { setterFunction = m_SetHueFunc; 				break;}
    	case 5: { setterFunction = m_SetPalenessFunc; 			break;}
    	case 6: { setterFunction = m_SetColorTemperatureFunc; 	break;}
    	case 7: { setterFunction = m_SetGammaCorrectionFunc; 	break;}
    	case 8: { setterFunction = m_SetVolumeFunc; 			break;}
    }
    if (m_options[0].second) { //parallelism enabled
		setParameterParallel(monitDialResult, setterFunction, paramNum, paramValue, monitorsDone);
	} else { 
		setParameterSequential(monitDialResult, setterFunction, paramNum, paramValue, monitorsDone);
	}//if (!parallel)
	Dialogue dialS("Set parameter", monitorDialogue_X, monitorDialogue_Y, monitorsDone);
	dialS.showDialogue(1, 0);
}
void Demo::powerStatusReadSequential(vector<pair<int, bool>> & monitDialResult, vector<string>& monitorsDone) {
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin();
			it != monitDialResult.end(); ++it)
	{
		if (m_monitors.find(it->first) != m_monitors.end()) {
			try {
				if ( !m_isConnected(it->first) ) {
					stringstream ss; ss << "Monitor " << it->first << " is not connected!";
					monitorsDone.push_back(ss.str());
					continue;
				}
				int result = m_PowerStatusReadFunc(it->first);
				stringstream ss;
				ss << "Power status of monitor "
					<< it->first << " is ";
				switch(result) {
					case 1: { ss << "ON"; break;}
					case 2: { ss << "STAND-BY"; break;}
					case 3: { ss << "SUSPEND"; break;}
					case 4: { ss << "OFF"; break;}
					default: { ss << "UNDEFINED"; break;}
				} ss << '.';
				monitorsDone.push_back(ss.str());
			} catch ( char const* s) {
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				dialE.hideDialogue();
				
				stringstream ss; ss << "Power status of monitor "
				<< it->first << " could not have been read.";
				monitorsDone.push_back(ss.str());
//				return;
			}
		}
	}		
}
void Demo::powerControlParallel(vector<pair<int, bool>> & monitDialResult, int paramValue,
							vector<string>& monitorsDone) {
	int thrCount = monitDialResult.size();
	
	pthread_t * threads = new pthread_t[thrCount];
	pthread_mutex_t printMtx;
	pthread_mutex_t dialogueMtx;
	pthread_mutex_init(&printMtx, NULL);
	pthread_mutex_init(&dialogueMtx, NULL);
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);
	pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
	struct powerControlThrFuncAttr * attrArray[thrCount];
	for (int i = 0; i < thrCount; i++)
	{
		attrArray[i] = new powerControlThrFuncAttr(	this,
													&monitorsDone,
													&printMtx,
													&dialogueMtx,
													monitDialResult[i].first,
													paramValue);
			pthread_create(&threads[i], NULL,
			(void*(*)(void*))&Demo::helperPowerControlFunc, attrArray[i]);
	}		
	for (int i = 0; i < thrCount; i++) {
		pthread_join(threads[i], NULL);
	}
	for (int i = 0; i < thrCount; i++) delete attrArray[i];
	pthread_mutex_destroy(&printMtx);
	pthread_mutex_destroy(&dialogueMtx);
	pthread_attr_destroy(&threadAttr);  
	delete [] threads;
}
void Demo::powerControlSequential(vector<pair<int, bool>> & monitDialResult, int paramValue,
							vector<string>& monitorsDone) {
	for (vector<pair<int, bool>>::iterator it = monitDialResult.begin();
				it != monitDialResult.end(); ++it)
	{
		if (m_monitors.find(it->first) != m_monitors.end()) {
			try {
				if ( !m_isConnected(it->first) ) {
					stringstream ss; ss << "Monitor " << it->first << " is not connected!";
					monitorsDone.push_back(ss.str());
					continue;
				}
				m_PowerControlFunc(it->first, paramValue);
				stringstream ss;
				ss << "Power status of monitor "
					<< it->first << " successfully changed to ";
				switch(paramValue) {
					case 1: { ss << "ON"; break;}
					case 4: { ss << "OFF"; break;}
					default: { ss << "UNDEFINED"; break;}
				} ss << '.';
				monitorsDone.push_back(ss.str());
			} catch ( char const* s) {
				Dialogue dialE("ERROR", s, 25, 10);
				dialE.showDialogue(1, 1);
				dialE.hideDialogue();
				
				stringstream ss; ss << "Power status of monitor "
				<< it->first << " could not have been changed.";
				monitorsDone.push_back(ss.str());
//				return;
			}
		}
	}
}
void Demo::commandChoice() {
    int monitorDialogue_X = 16;
    int monitorDialogue_Y = 2;
		    m_MenuWindow->hideMenu();
		   //vyber parametru
		    Dialogue chooseCommand("Choose command", 25, 11, m_CommandNames);
		    int commandNum = chooseCommand.showDialogue(0,0);
		    chooseCommand.hideDialogue();
		    if (commandNum == -1) return;
		   	
		   	switch(commandNum) {
		   		case 0: {  //read power status
		   			//vyber monitoru
					Dialogue dial("Choose monitor (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
					vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
					dial.hideDialogue();
					if (monitDialResult.size() == 0) {
						touchwin(stdscr);
						refresh();
						return;
					}
					vector<string> monitorsDone;
		    		
		    		powerStatusReadSequential(monitDialResult, monitorsDone);
		    		
		    		Dialogue dialS("Power status read", monitorDialogue_X, monitorDialogue_Y, monitorsDone);
					dialS.showDialogue(1, 0);
			
		   			break;
		   		}
		   		case 1: { //change power status
					//value choice
					Dialogue getNumber("Enter command value! (1=ON or 4=OFF)", "", 15, 10);
					int paramValue = getNumber.showGetNumberDialogue();
					getNumber.hideDialogue();
					if (paramValue == -1) return;
					if (paramValue != 1 && paramValue != 4) {
						Dialogue invVal("ERROR", "Invalid power control value, possible: 1=ON or 4=OFF", 15, 10);
						invVal.showDialogue(1, 1);
						return;
					}
				
					//monitor choice
					Dialogue dial("Choose monitor (SPACE to select)", monitorDialogue_X, monitorDialogue_Y, m_monitors);
					vector<pair<int, bool>> monitDialResult = dial.showMonitorDialogue();
					dial.hideDialogue();
					if (monitDialResult.size() == 0) {
						touchwin(stdscr);
						refresh();
						return;
					}
				
					vector<string> monitorsDone;
					if (m_options[0].second) { //parallelism enabled
						powerControlParallel(monitDialResult, paramValue, monitorsDone);
					} else { 
						powerControlSequential(monitDialResult, paramValue, monitorsDone);
					}
					Dialogue dialS("Change power status", monitorDialogue_X-2, monitorDialogue_Y, monitorsDone);
					dialS.showDialogue(1, 0);
					break;
		   		}
		   	}
}

void Demo::startMenu(void)
{
start:
    int choice = m_MenuWindow->showMenu();
    switch (choice) {
		case 0: //ADD monitor
		{
		    addMonitorChoice();
			touchwin(stdscr);
			refresh();
		    goto start;
		    break;
		}
		case 1: //DELETE monitor
		{
			deleteMonitorChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 2: //CONNECT monitor
		{
			connectMonitorChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 3: //DISCONNECT monitor
		{
			disconnectMonitorChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 4: //CONNECT ALL
		{
			connectAllChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 5: //DISCONNECT ALL
		{
			disconnectAllChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 6: //IS CONNECTED
		{
			isConnectedChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 7: //GET PARAM
		{
			getParameterChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 8: //SET PARAM
		{
			setParameterChoice();
		    touchwin(stdscr);
		    refresh();
		    goto start;
		    break;
		}
		case 9: //COMMAND
		{
			commandChoice();
		   	touchwin(stdscr);
		    refresh();
		    goto start;
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

void * Demo::helperGetterFunc(struct getterThrFuncAttr * attr) {
	attr->demo->GetterThrFunc(attr);
	return NULL;
}

void Demo::GetterThrFunc (struct getterThrFuncAttr * attr) {

	if (m_monitors.find(attr->monitorID) != m_monitors.end()) {
		try {
			if ( !m_isConnected(attr->monitorID) ) {
				stringstream ss; ss << "Monitor " << attr->monitorID << " is not connected!";
				pthread_mutex_lock(attr->printMtx);
				attr->monitorsDone->push_back(ss.str());
				pthread_mutex_unlock(attr->printMtx);
				return;
			}
			int result = (attr->getterFunc)(attr->monitorID);
			stringstream ss;
			ss << m_FunctionNames[attr->paramNum] << " of monitor "
				<< attr->monitorID << " is " << result << '.';
			pthread_mutex_lock(attr->printMtx);
			attr->monitorsDone->push_back(ss.str());
			pthread_mutex_unlock(attr->printMtx);
		} catch ( char const* s) {
			Dialogue dialE("ERROR", s, 25, 10);
			pthread_mutex_lock(attr->dialogueMtx);
			dialE.showDialogue(1, 1);
			dialE.hideDialogue();
			pthread_mutex_unlock(attr->dialogueMtx);
			
			stringstream ss; ss << m_FunctionNames[attr->paramNum] << " of monitor "
				<< attr->monitorID << " could not have been read. ";
			pthread_mutex_lock(attr->printMtx);
			attr->monitorsDone->push_back(ss.str());
			pthread_mutex_unlock(attr->printMtx);
			return;
		}
	}
	return;
}

void * Demo::helperSetterFunc(struct setterThrFuncAttr * attr) {
	attr->demo->SetterThrFunc(attr);
	return NULL;
}
void Demo::SetterThrFunc (struct setterThrFuncAttr * attr) {
	if (m_monitors.find(attr->monitorID) != m_monitors.end()) {
		try {
			if ( !m_isConnected(attr->monitorID) ) {
				stringstream ss; ss << "Monitor " << attr->monitorID << " is not connected!";
				pthread_mutex_lock(attr->printMtx);
				attr->monitorsDone->push_back(ss.str());
				pthread_mutex_unlock(attr->printMtx);
				return;
			}
			attr->setterFunc(attr->monitorID, attr->paramValue);
			stringstream ss;
			ss << m_FunctionNames[attr->paramNum] << " of monitor "
				<< attr->monitorID << " successfully changed to " << attr->paramValue << '.';
			pthread_mutex_lock(attr->printMtx);
			attr->monitorsDone->push_back(ss.str());
			pthread_mutex_unlock(attr->printMtx);
		} catch ( char const* s) {
			Dialogue dialE("ERROR", s, 25, 10);
			pthread_mutex_lock(attr->dialogueMtx);
			dialE.showDialogue(1, 1);
			dialE.hideDialogue();
			pthread_mutex_unlock(attr->dialogueMtx);
			
			stringstream ss; ss << m_FunctionNames[attr->paramNum] << " of monitor "
				<< attr->monitorID << " could not have been changed.";
			pthread_mutex_lock(attr->printMtx);
			attr->monitorsDone->push_back(ss.str());
			pthread_mutex_unlock(attr->printMtx);
			
			return;
		}
	}
	return;
}

void * Demo::helperPowerControlFunc(struct powerControlThrFuncAttr * attr) {
	attr->demo->PowerControlThrFunc(attr);
	return NULL;
}
void Demo::PowerControlThrFunc (struct powerControlThrFuncAttr * attr) {
	if (m_monitors.find(attr->monitorID) != m_monitors.end()) {
		try {
			if ( !m_isConnected(attr->monitorID) ) {
				stringstream ss; ss << "Monitor " << attr->monitorID << " is not connected!";
				pthread_mutex_lock(attr->printMtx);
				attr->monitorsDone->push_back(ss.str());
				pthread_mutex_unlock(attr->printMtx);
				return;
			}
			m_PowerControlFunc(attr->monitorID, attr->paramValue);
			stringstream ss;
			ss << "Power status of monitor "
				<< attr->monitorID << " successfully changed to ";
				switch(attr->paramValue) {
				case 1: { ss << "ON"; break;}
				case 4: { ss << "OFF"; break;}
				default: { ss << "UNDEFINED"; break;}
			} ss << '.';
			pthread_mutex_lock(attr->printMtx);
			attr->monitorsDone->push_back(ss.str());
			pthread_mutex_unlock(attr->printMtx);
		} catch ( char const* s) {
			Dialogue dialE("ERROR", s, 25, 10);
			pthread_mutex_lock(attr->dialogueMtx);
			dialE.showDialogue(1, 1);
			dialE.hideDialogue();
			pthread_mutex_unlock(attr->dialogueMtx);
			
			stringstream ss; ss << "Power status of monitor "
				<< attr->monitorID << " could not have been changed.";
			pthread_mutex_lock(attr->printMtx);
			attr->monitorsDone->push_back(ss.str());
			pthread_mutex_unlock(attr->printMtx);
			
			return;
		}
	}
	return;
}
