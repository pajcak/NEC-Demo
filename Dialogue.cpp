#include "Dialogue.h"
#include "Colours.h"
#include <sstream>

using namespace std;

Dialogue::Dialogue(const string & title, const int & X, const int & Y, map<int, pair<string, int>> & monitors) {
    m_isConstant = false;
    m_Title = title;
    m_X = X;
    m_Y = Y;
    m_monitors.insert(monitors.begin(), monitors.end());
    
    m_Cols = 25;
    m_Lines = m_monitors.size() + 4;
    m_DialogueWindow = newwin(m_Lines, m_Cols, m_Y, m_X);
    m_isWarning = false;
}
Dialogue::Dialogue(const string & title, const int & X, const int & Y, const vector<string> & strings)
{
    m_isConstant = false;
    m_Title = title;
    m_X = X;
    m_Y = Y;
    int longestStr = m_Title.length();
    for (vector<string>::const_iterator it = strings.begin();
            it != strings.end();
            ++it) {
        m_Strings.push_back(*it);
        if ((int)it->length() > longestStr) longestStr = (int)it->length();
    }
    m_Cols = longestStr + 4;
    m_Lines = m_Strings.size() + 4;
    m_DialogueWindow = newwin(m_Lines, m_Cols, m_Y, m_X);
    m_isWarning = false;
}

Dialogue::Dialogue(const string & title, const string & text, const int & X, const int & Y)
: m_X(X),
  m_Y(Y),
  m_Title(title),
  m_Text(text)
{
    m_isConstant = true;
    m_Cols = title.length() + 20;
    m_Lines = m_Text.length()/m_Cols + 5;
    m_DialogueWindow = newwin(m_Lines, m_Cols, m_Y, m_X);
    m_isWarning = false;
}

Dialogue::~Dialogue()
{
    untouchwin(m_DialogueWindow);
    delwin(m_DialogueWindow);
}
int Dialogue::showMonitorDialogue() {
    int selected = 0, selectedMonitorID = -1;
    keypad(m_DialogueWindow, TRUE);
    int key = 0;
    while (key != KEY_ENTER && key != '\n') {
        if (key == KEY_UP) {
            if (selected == 0) selected = m_monitors.size() - 1;
            else selected--;
        }
        if (key == KEY_DOWN) {
            if (selected == (int)m_monitors.size() - 1) selected = 0;
            else selected++;
        }
        printUpdatedMonitorDialogue(selected, selectedMonitorID);
        key = wgetch(m_DialogueWindow);
		if (key == 27) return -1;
    }
    
    keypad(m_DialogueWindow, FALSE);
    return selectedMonitorID;
}
string Dialogue::showAddMonitorDialogue() {
	printUpdatedDialogue(-1);
	keypad(m_DialogueWindow, TRUE);
	int key = 0;
	int lineY = 3;
	int lineX = (m_Cols / 2) - m_Title.length() / 2;
	string result;
    while (key != KEY_ENTER && key != '\n') {
    	key = wgetch(m_DialogueWindow);
		if (key == 27) return "";
    	if ( !( (key >= '0' && key <= '9') || key == '.' || key == ' ' || key == KEY_BACKSPACE) ) continue;
    	
    	if (key == KEY_BACKSPACE) {
    		if (result.length() > 0) {
    			result.erase(result.length() - 1, 1);
				mvwaddch(m_DialogueWindow, lineY, --lineX, ' ');
    		}
    		continue;
    	}
		result.push_back(key);
		mvwaddch(m_DialogueWindow, lineY, lineX++, key);
		wrefresh(m_DialogueWindow);
	}
    keypad(m_DialogueWindow, FALSE);
return result;
}

int Dialogue::showDialogue()
{
    if (m_isConstant) {
        showConstantDialogue();
        return 0;
    }
    
    int selected = 0;
    keypad(m_DialogueWindow, TRUE);
    int key = 0;
    while (key != KEY_ENTER && key != '\n') {
        if (key == KEY_UP) {
            if (selected == 0) selected = m_Strings.size() - 1;
            else selected--;
        }
        if (key == KEY_DOWN) {
            if (selected == (int)m_Strings.size() - 1) selected = 0;
            else selected++;
        }
        printUpdatedDialogue(selected);
        key = wgetch(m_DialogueWindow);
    }
    
    keypad(m_DialogueWindow, FALSE);
    return selected;
}

void Dialogue::showConstantDialogue()
{
    keypad(m_DialogueWindow, TRUE);
    int key = 0;
    while (key != KEY_ENTER && key != '\n') {
        box(m_DialogueWindow, '|', '-');
        wattron(m_DialogueWindow, COLOR_PAIR(RED));
        wattron(m_DialogueWindow, A_UNDERLINE);
        wattron(m_DialogueWindow, A_BOLD);
        mvwprintw(m_DialogueWindow, 1, (m_Cols / 2) - m_Title.length() / 2, "%s", m_Title.c_str());
        wattroff(m_DialogueWindow, A_BOLD);
        wattroff(m_DialogueWindow, A_UNDERLINE);
        wattroff(m_DialogueWindow, COLOR_PAIR(RED));
        
        int idx = 0;
        if (m_isWarning) wattron(m_DialogueWindow, COLOR_PAIR(YELLOW));
        for (int i = 0; i < m_Lines - 4; i++) {
            for (int j = 1; j < m_Cols - 1; j++) {
                if (idx >= (int)m_Text.length()) break;
                mvwaddch(m_DialogueWindow, i + 3, j, m_Text.at(idx++));
            }
        }
        if (m_isWarning) wattroff(m_DialogueWindow, COLOR_PAIR(YELLOW));
        wrefresh(m_DialogueWindow);
        
        key = wgetch(m_DialogueWindow);
    }
    
    keypad(m_DialogueWindow, FALSE);
}

void Dialogue::hideDialogue()
{
    wclear(m_DialogueWindow);
    wrefresh(m_DialogueWindow);
    untouchwin(m_DialogueWindow);
}
void Dialogue::printUpdatedMonitorDialogue(const int & selected, int & monitorID)
{
    box(m_DialogueWindow, '|', '-');
    wattron(m_DialogueWindow, COLOR_PAIR(RED));
    wattron(m_DialogueWindow, A_UNDERLINE);
    wattron(m_DialogueWindow, A_BOLD);
    mvwprintw(m_DialogueWindow, 1, (m_Cols / 2) - m_Title.length() / 2, "%s", m_Title.c_str());
    wattroff(m_DialogueWindow, A_BOLD);
    wattroff(m_DialogueWindow, A_UNDERLINE);
    wattroff(m_DialogueWindow, COLOR_PAIR(RED));
    
    int i = 0;
    
    for (map<int, pair<string,int>>::const_iterator it = m_monitors.begin();
    		it != m_monitors.end(); ++it)
    {
    	ostringstream ss;
    	ss << it->first << ' ' << it->second.first << ' ' << it->second.second;
        if (i == selected) {
            wattron(m_DialogueWindow, COLOR_PAIR(GREEN));
            mvwprintw(m_DialogueWindow, i + 3, 2, "%s", ss.str().c_str());
            wattroff(m_DialogueWindow, COLOR_PAIR(GREEN));
            monitorID = it->first;
        } else mvwprintw(m_DialogueWindow, i + 3, 2, "%s", ss.str().c_str());
        i++;
    }
    wrefresh(m_DialogueWindow);
}

void Dialogue::printUpdatedDialogue(const int & selected)
{
    box(m_DialogueWindow, '|', '-');
    wattron(m_DialogueWindow, COLOR_PAIR(RED));
    wattron(m_DialogueWindow, A_UNDERLINE);
    wattron(m_DialogueWindow, A_BOLD);
    mvwprintw(m_DialogueWindow, 1, (m_Cols / 2) - m_Title.length() / 2, "%s", m_Title.c_str());
    wattroff(m_DialogueWindow, A_BOLD);
    wattroff(m_DialogueWindow, A_UNDERLINE);
    wattroff(m_DialogueWindow, COLOR_PAIR(RED));
    for (int i = 0; i < (int) m_Strings.size(); i++) {
        if (i == selected) {
            wattron(m_DialogueWindow, COLOR_PAIR(GREEN));
            mvwprintw(m_DialogueWindow, i + 3, 2, "%s", m_Strings[i].c_str());
            wattroff(m_DialogueWindow, COLOR_PAIR(GREEN));
        } else mvwprintw(m_DialogueWindow, i + 3, 2, "%s", m_Strings[i].c_str());
    }
    wrefresh(m_DialogueWindow);
}

void Dialogue::setWarningColor()
{
    m_isWarning = true;
}