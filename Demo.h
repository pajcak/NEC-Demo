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
	void initMonitors();
	void initFunctions();
	bool findDuplicate(int id, string adddr, int port) const;
	
	CController * m_controller;
    MenuWindow * m_MenuWindow;
    map<string, void(*)> m_Parameters;
    map<int, pair<string, int>> m_monitors;
    vector<pair<string,bool>> m_options;
    bool m_errorFlag_parallel;
    
	//----THR-FUNCS----------------------
	struct getterThrFuncAttr {
		getterThrFuncAttr( Demo * d, int(CController::* f)(int),
							vector<string> * monVect,
							pthread_mutex_t * prMtx,
							pthread_mutex_t * dialMtx,
							int id, int param)
		: thisObj(d), getterFunc(f),
		monitorsDone(monVect),
		printMtx(prMtx),
		dialogueMtx(dialMtx),
		monitorID(id), paramNum(param){}
		
		Demo * thisObj;
		int (CController::* getterFunc) (int);
		vector<string> * monitorsDone;
		pthread_mutex_t * printMtx;
		pthread_mutex_t * dialogueMtx;
		int monitorID;
		int paramNum;
	};
	static void * helperGetterThrFunc(struct getterThrFuncAttr * attr);
	void GetterThrFunc (struct getterThrFuncAttr * attr);
	//--------------------
	struct setterThrFuncAttr {
		setterThrFuncAttr( Demo * d, void(CController::* f)(int,int),
							vector<string> * monVect,
							pthread_mutex_t * prMtx,
							pthread_mutex_t * dialMtx,
							int id, int parNum, int parValue)
		: thisObj(d), setterFunc(f),
		monitorsDone(monVect),
		printMtx(prMtx),
		dialogueMtx(dialMtx),
		monitorID(id), paramNum(parNum), paramValue(parValue){}
		
		Demo * thisObj;
		void (CController::* setterFunc) (int, int);
		vector<string> * monitorsDone;
		pthread_mutex_t * printMtx;
		pthread_mutex_t * dialogueMtx;
		int monitorID;
		int paramNum;
		int paramValue;
	};
	static void * helperSetterThrFunc(struct setterThrFuncAttr * attr);
	void SetterThrFunc (struct setterThrFuncAttr * attr);
	//-----------------
	struct powerControlThrFuncAttr {
		powerControlThrFuncAttr( Demo * d,
								vector<string> * monVect,
								pthread_mutex_t * prMtx,
								pthread_mutex_t * dialMtx,
								int id, int val)
		: thisObj(d),
		monitorsDone(monVect),
		printMtx(prMtx),
		dialogueMtx(dialMtx),
		monitorID(id), paramValue(val) {}

		Demo * thisObj;
		vector<string> * monitorsDone;
		pthread_mutex_t * printMtx;
		pthread_mutex_t * dialogueMtx;
		int monitorID;
		int paramValue;
	};
	static void * helperPowerControlThrFunc(struct powerControlThrFuncAttr * attr);
	void PowerControlThrFunc (struct powerControlThrFuncAttr * attr);
	//----THR-FUNCS-END------------------
	
	//-------FUNCTIONS-------
    void (CController::* m_initController   ) (); 
    void (CController::* m_destroyController) (); 
    void (CController::* m_addMonitor       ) (const char *, int, int);
    void (CController::* m_deleteMonitor    ) (int);
    bool (CController::* m_connectMonitor   ) (int);
    void (CController::* m_disconnectMonitor) (int);
    bool (CController::* m_connectAll       ) ();
    void (CController::* m_disconnectAll    ) ();
    bool (CController::* m_isConnected      ) (int);
    //-------PARAM-FUNCTIONS-------
    
    vector<string> m_FunctionNames;
    
	int (CController::* m_GetBacklightFunc)(int);
	void (CController::* m_SetBacklightFunc)(int, int);

	int (CController::* m_GetContrastFunc)(int);
	void (CController::* m_SetContrastFunc)(int, int);

	int (CController::* m_GetSharpnessFunc)(int);
	void (CController::* m_SetSharpnessFunc)(int, int);

	int (CController::* m_GetBrightnessFunc)(int);
	void (CController::* m_SetBrightnessFunc)(int, int);

	int (CController::* m_GetHueFunc)(int);
	void (CController::* m_SetHueFunc)(int, int);

	int (CController::* m_GetPalenessFunc)(int);
	void (CController::* m_SetPalenessFunc)(int, int);

	int (CController::* m_GetColorTemperatureFunc)(int);
	void (CController::* m_SetColorTemperatureFunc)(int, int);

	int (CController::* m_GetGammaCorrectionFunc)(int);
	void (CController::* m_SetGammaCorrectionFunc)(int, int);

	int (CController::* m_GetVolumeFunc)(int);
	void (CController::* m_SetVolumeFunc)(int, int);
    //-------COMMAND-FUNCTIONS-------
    vector<string> m_CommandNames;
    
    int  (CController::* m_PowerStatusReadFunc)(int);
    void (CController::* m_PowerControlFunc)(int, int);
};

#endif	/* Demo_H */

