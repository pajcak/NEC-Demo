#include <vector>
#include <string>
#include <ncurses.h>
#include <map>
#ifndef DIALOGUE_H
#define	DIALOGUE_H

using namespace std;

class Dialogue {
public:
    Dialogue(const string & title, const int & X, const int & Y, const vector<string> & strings);
    Dialogue(const string & title, const int & X, const int & Y, vector<pair<string, bool> > * options);
    Dialogue(const string & title, const int & X, const int & Y, map<int, pair<string, int>> & monitors);
    Dialogue(const string & title, const string & text, const int & X, const int & Y);
    virtual ~Dialogue();
    
    int 							showDialogue(bool isConstant, bool isWarning);
    vector<pair<int, bool>> 		showMonitorDialogue();
    string 							showAddMonitorDialogue();
    int 							showGetNumberDialogue();
    vector<pair<string, bool> > * 	showOptionsDialogue();
    void 							hideDialogue();
private:
    void printUpdatedDialogue(const int & selected, bool isWarning);
    void printUpdatedOptionsDialogue(const int & selected, bool changeNeeded);
    void printUpdatedMonitorDialogue(const int & selected,
                                    	vector<pair<int, bool>> & monitorsSelection);
    int m_Lines, m_Cols;
    int m_X;
    int m_Y;
    
    map<int, pair<string, int>> m_monitors;
    vector<string> m_Strings;
    vector<pair<string, bool>> * m_Options;
    
    string m_Title;
    WINDOW * m_DialogueWindow;
};

#endif	/* DIALOGUE_H */

