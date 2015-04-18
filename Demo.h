#include "MenuWindow.h"
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
	void initMonitors();
	void initOptions();
	void loadLibrary(const char * libName);
	bool findDuplicate(int id, string adddr, int port) const;
	
	void * m_Library;
	
    MenuWindow * m_MenuWindow;
    map<string, void(*)> m_Parameters;
    map<int, pair<string, int>> m_monitors;
    vector<pair<string,bool>> m_options;
    bool m_errorFlag_parallel;
    
	//----THR-FUNCS----------------------
	struct getterThrFuncAttr {
		getterThrFuncAttr( int( *f)(int),
							vector<string> * monVect,
							pthread_mutex_t * prMtx,
							pthread_mutex_t * dialMtx,
							int id, int param)
		: getterFunc(f),
		monitorsDone(monVect),
		printMtx(prMtx),
		dialogueMtx(dialMtx),
		monitorID(id), paramNum(param){}
		
		int ( *getterFunc) (int);
		vector<string> * monitorsDone;
		pthread_mutex_t * printMtx;
		pthread_mutex_t * dialogueMtx;
		int monitorID;
		int paramNum;
	};
	void GetterThrFunc (struct getterThrFuncAttr * attr);
	//--------------------
	struct setterThrFuncAttr {
		setterThrFuncAttr( void( *f)(int,int),
							vector<string> * monVect,
							pthread_mutex_t * prMtx,
							pthread_mutex_t * dialMtx,
							int id, int parNum, int parValue)
		: setterFunc(f),
		monitorsDone(monVect),
		printMtx(prMtx),
		dialogueMtx(dialMtx),
		monitorID(id), paramNum(parNum), paramValue(parValue){}
		
		void ( *setterFunc) (int, int);
		vector<string> * monitorsDone;
		pthread_mutex_t * printMtx;
		pthread_mutex_t * dialogueMtx;
		int monitorID;
		int paramNum;
		int paramValue;
	};
	void SetterThrFunc (struct setterThrFuncAttr * attr);
	//-----------------
	struct powerControlThrFuncAttr {
		powerControlThrFuncAttr( vector<string> * monVect,
								pthread_mutex_t * prMtx,
								pthread_mutex_t * dialMtx,
								int id, int val)
		: monitorsDone(monVect),
		printMtx(prMtx),
		dialogueMtx(dialMtx),
		monitorID(id), paramValue(val) {}

		vector<string> * monitorsDone;
		pthread_mutex_t * printMtx;
		pthread_mutex_t * dialogueMtx;
		int monitorID;
		int paramValue;
	};
	void PowerControlThrFunc (struct powerControlThrFuncAttr * attr);
	//----THR-FUNCS-END------------------
	
	//-------FUNCTIONS-------
    void ( *m_initController   ) (); 
    void ( *m_destroyController) (); 
    void ( *m_addMonitor       ) (const char *, int, int);
    void ( *m_deleteMonitor    ) (int);
    bool ( *m_connectMonitor   ) (int);
    void ( *m_disconnectMonitor) (int);
    bool ( *m_connectAll       ) ();
    void ( *m_disconnectAll    ) ();
    bool ( *m_isConnected      ) (int);
    //-------PARAM-FUNCTIONS-------
    
    vector<string> m_FunctionNames;
    
	int (*m_GetBacklightFunc)(int);
	void (*m_SetBacklightFunc)(int, int);

	int (*m_GetContrastFunc)(int);
	void (*m_SetContrastFunc)(int, int);

	int (*m_GetSharpnessFunc)(int);
	void (* m_SetSharpnessFunc)(int, int);

	int ( *m_GetBrightnessFunc)(int);
	void ( *m_SetBrightnessFunc)(int, int);

	int ( *m_GetHueFunc)(int);
	void ( *m_SetHueFunc)(int, int);

	int ( *m_GetPalenessFunc)(int);
	void ( *m_SetPalenessFunc)(int, int);

	int ( *m_GetColorTemperatureFunc)(int);
	void ( *m_SetColorTemperatureFunc)(int, int);

	int ( *m_GetGammaCorrectionFunc)(int);
	void ( *m_SetGammaCorrectionFunc)(int, int);

	int ( *m_GetVolumeFunc)(int);
	void ( *m_SetVolumeFunc)(int, int);
    //-------COMMAND-FUNCTIONS-------
    vector<string> m_CommandNames;
    
    int  ( *m_PowerStatusReadFunc)(int);
    void ( *m_PowerControlFunc)(int, int);
};

#endif	/* Demo_H */

