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
    int showDialogue();
    int showMonitorDialogue();
    string showAddMonitorDialogue();
    vector<pair<string, bool> > * showOptionsDialogue();
    void hideDialogue();
    void setWarningColor();
private:
    void showConstantDialogue();
    void printUpdatedDialogue(const int & selected);
    void printUpdatedOptionsDialogue(const int & selected, bool changeNeeded);
    void printUpdatedMonitorDialogue(const int & selected, int & monitorID);
    bool m_isConstant;
    int m_Lines, m_Cols;
    int m_X;
    int m_Y;
    
    map<int, pair<string, int>> m_monitors;
    vector<string> m_Strings;
    vector<pair<string, bool>> * m_Options;
    
    string m_Title, m_Text;
    WINDOW * m_DialogueWindow;
    bool m_isWarning;
};

#endif	/* DIALOGUE_H */

