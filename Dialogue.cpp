#include "Dialogue.h"
#include "Colours.h"
#include <iomanip>
#include <sstream>

using namespace std;

Dialogue::Dialogue(const string & title, const int & X, const int & Y, map<int, pair<string, int>> & monitors) {
    m_Title = title;
    m_X = X;
    m_Y = Y;
    m_monitors.insert(monitors.begin(), monitors.end());
    
    m_Cols = 46;
    m_Lines = (m_monitors.size() + 4) ;
    m_DialogueWindow = newwin(m_Lines, m_Cols, m_Y, m_X);
}
Dialogue::Dialogue(const string & title, const int & X, const int & Y, vector<pair<string, bool>> * options) 
: m_Options(options)
{
    m_Title = title;
    m_X = X;
    m_Y = Y;
    int longestStr = m_Title.length();
    for (vector<pair<string,bool>>::const_iterator it = m_Options->begin();
            it != m_Options->end();
            ++it) {
        if ((int)it->first.length() > longestStr) longestStr = (int)it->first.length();
    }
    m_Cols = longestStr + 4;
    m_Lines = m_Options->size() + 5;
    m_DialogueWindow = newwin(m_Lines, m_Cols, m_Y, m_X);
}
Dialogue::Dialogue(const string & title, const int & X, const int & Y, const vector<string> & strings)
{
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
}

Dialogue::Dialogue(const string & title, const string & text, const int & X, const int & Y)
: m_X(X),
  m_Y(Y),
  m_Title(title)
{
	m_Strings.push_back(text);
    m_Cols = title.length() + 20;
    m_Lines = text.length()/m_Cols + 5;
    m_DialogueWindow = newwin(m_Lines, m_Cols, m_Y, m_X);
}

Dialogue::~Dialogue()
{
    untouchwin(m_DialogueWindow);
    delwin(m_DialogueWindow);
}
vector<pair<int, bool>> Dialogue::showMonitorDialogue() {
    vector<pair<int, bool>> monitorsSelection;
    for (map<int, pair<string,int>>::const_iterator it = m_monitors.begin();
    		it != m_monitors.end(); ++it)
    {
    	monitorsSelection.push_back(make_pair(it->first, false));
    }
    int scrollDirection = 0;
    int scrollThreshold = LINES - m_Y - 5;
    int scrollMax = -1; 
    int selected = 0;
    int key = 0;
    scrollok(m_DialogueWindow, true);
    keypad(m_DialogueWindow, TRUE);
    while (key != KEY_ENTER && key != '\n') {
        if (key == 27) return vector<pair<int, bool>>();
        if (key == KEY_UP) {
        	if (selected >= scrollThreshold) scrollDirection--;
            if (selected == 0) {
            	selected = m_monitors.size() - 1;
            	scrollDirection = scrollMax;
            } else selected--;
        }
        if (key == KEY_DOWN) {
        	if (selected >= scrollThreshold) {
        		scrollDirection++;
        		if (scrollDirection > scrollMax) scrollMax = scrollDirection;
        	}
            if (selected == (int)m_monitors.size() - 1) {
            	selected = 0;
            	scrollDirection = 0;
            }
            else selected++;
        }
        if (key == ' ') monitorsSelection[selected].second = !monitorsSelection[selected].second;

        printUpdatedMonitorDialogue(selected, monitorsSelection);
        
		if (selected >= scrollThreshold) {
			wscrl(m_DialogueWindow, scrollDirection);
		}
        key = wgetch(m_DialogueWindow);
	}
	keypad(m_DialogueWindow, FALSE);
    
    vector<pair<int, bool>> retVect;
	for (vector<pair<int, bool>>::const_iterator it = monitorsSelection.begin();
		it != monitorsSelection.end(); ++it)
	{
		if (it->second) retVect.push_back(make_pair(it->first, it->second));
    }
    return retVect;
}
int	Dialogue::showGetNumberDialogue() {
	printUpdatedDialogue(-1, 0);
	keypad(m_DialogueWindow, TRUE);
	int key = 0;
	int lineY = 3;
	int lineX = (m_Cols / 2) - m_Title.length() / 2;
	string buff;
    while (key != KEY_ENTER && key != '\n') {
    	key = wgetch(m_DialogueWindow);
		if (key == 27) return -1;
    	if ( !( (key >= '0' && key <= '9') || key == KEY_BACKSPACE) ) continue;
    	
    	if (key == KEY_BACKSPACE) {
    		if (buff.length() > 0) {
    			buff.erase(buff.length() - 1, 1);
				mvwaddch(m_DialogueWindow, lineY, --lineX, ' ');
    		}
    		continue;
    	}
		buff.push_back(key);
		mvwaddch(m_DialogueWindow, lineY, lineX++, key);
		wrefresh(m_DialogueWindow);
	}
	int result = atoi(buff.c_str());
    keypad(m_DialogueWindow, FALSE);
	return result;
}
string Dialogue::showAddMonitorDialogue() {
	printUpdatedDialogue(-1, 0);
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
vector<pair<string, bool> >* Dialogue::showOptionsDialogue() {
	int selected = 0;
	printUpdatedOptionsDialogue(selected, 0);
	keypad(m_DialogueWindow, TRUE);
	int key = 0;
	while (true) {
    	if (key == 27) return m_Options;
        if (key == KEY_UP) {
            if (selected == 0) selected = m_Options->size();
            else selected--;
        }
        if (key == KEY_DOWN) {
            if (selected == (int)m_Options->size()) selected = 0;
            else selected++;
        }
        if (key == ' ' || key == KEY_ENTER || key == '\n') {
        	if (selected == (int)m_Options->size()) return m_Options;
        	printUpdatedOptionsDialogue(selected, 1);
        } else printUpdatedOptionsDialogue(selected, 0);
        key = wgetch(m_DialogueWindow);
    }
    keypad(m_DialogueWindow, FALSE);
return m_Options;
}

int Dialogue::showDialogue(bool isConstant, bool isWarning)
{
    int selected = 0;
    keypad(m_DialogueWindow, TRUE);
    int key = 0;
 
    while (key != KEY_ENTER && key != '\n') {
    	if (key == 27) return -1;
        if (!isConstant) {
		    if (key == KEY_UP) {
		        if (selected == 0) selected = m_Strings.size() - 1;
		        else selected--;
		    }
		    if (key == KEY_DOWN) {
		        if (selected == (int)m_Strings.size() - 1) selected = 0;
		        else selected++;
		    }
		} else selected = -1;
        printUpdatedDialogue(selected, isWarning);
        key = wgetch(m_DialogueWindow);
    }
    
    keypad(m_DialogueWindow, FALSE);
    return selected;
}

void Dialogue::hideDialogue()
{
    wclear(m_DialogueWindow);
    wrefresh(m_DialogueWindow);
    untouchwin(m_DialogueWindow);
}
void Dialogue::printUpdatedMonitorDialogue(const int & selected,
											vector<pair<int, bool>> & monitorsSelection)
{
    wclear(m_DialogueWindow);
    box(m_DialogueWindow, '|', '-');
    wattron(m_DialogueWindow, COLOR_PAIR(RED));
    wattron(m_DialogueWindow, A_UNDERLINE);
    wattron(m_DialogueWindow, A_BOLD);
    mvwprintw(m_DialogueWindow, 1, (m_Cols / 2) - m_Title.length() / 2, "%s", m_Title.c_str());
    wattroff(m_DialogueWindow, A_BOLD);
    wattroff(m_DialogueWindow, A_UNDERLINE);
    wattroff(m_DialogueWindow, COLOR_PAIR(RED));
    
    wattron(m_DialogueWindow, COLOR_PAIR(BLUE));
    mvwprintw(m_DialogueWindow, 2, (m_Cols / 2) - m_Title.length() / 2, "%s", "  ID     ADDRESS     PORT");
    wattroff(m_DialogueWindow, COLOR_PAIR(BLUE));
    
    int i = 0;
    
    for (map<int, pair<string,int>>::const_iterator it = m_monitors.begin();
    		it != m_monitors.end(); ++it)
    {
    	ostringstream ss;
    	ss << left << setw(6) << it->first <<  setw(13) << it->second.first << setw(6) << it->second.second;
    	
        if (monitorsSelection[i].second) {
    		wattron(m_DialogueWindow, COLOR_PAIR(RED));
           	mvwprintw(m_DialogueWindow, i + 3, 9, "%s", ss.str().c_str());
           	mvwaddch(m_DialogueWindow, i + 3, 8, '*');
           	wattroff(m_DialogueWindow, COLOR_PAIR(RED));
        } else {
        	mvwprintw(m_DialogueWindow, i + 3, 9, "%s", ss.str().c_str());
        	mvwaddch(m_DialogueWindow, i + 3, 8, '*');
        }
    	if (i == selected) {
            wattron(m_DialogueWindow, COLOR_PAIR(GREEN));
            mvwprintw(m_DialogueWindow, i + 3, 9, "%s", ss.str().c_str());
            wattroff(m_DialogueWindow, COLOR_PAIR(GREEN));
        }
        i++;
    }
    wrefresh(m_DialogueWindow);
}

void Dialogue::printUpdatedOptionsDialogue(const int & selected, bool changeNeeded) {
    box(m_DialogueWindow, '|', '-');
    wattron(m_DialogueWindow, COLOR_PAIR(RED));
    wattron(m_DialogueWindow, A_UNDERLINE);
    wattron(m_DialogueWindow, A_BOLD);
    mvwprintw(m_DialogueWindow, 1, (m_Cols / 2) - m_Title.length() / 2, "%s", m_Title.c_str());
    wattroff(m_DialogueWindow, A_BOLD);
    wattroff(m_DialogueWindow, A_UNDERLINE);
    wattroff(m_DialogueWindow, COLOR_PAIR(RED));
    
    int i = 0;
    
    for (vector<pair<string,bool>>::iterator it = m_Options->begin();
    		it != m_Options->end(); ++it)
    {
    	ostringstream ss;
    	if (changeNeeded) it->second = !it->second;
    	ss << it->first << setfill('.') << setw(10) << (it->second == true ? "YES" : "NO");
        if (i == selected) {
            wattron(m_DialogueWindow, COLOR_PAIR(GREEN));
            mvwprintw(m_DialogueWindow, i + 3, 2, "%s", ss.str().c_str());
            wattroff(m_DialogueWindow, COLOR_PAIR(GREEN));
        } else mvwprintw(m_DialogueWindow, i + 3, 2, "%s", ss.str().c_str());
        i++;
    }
    if (i == selected) {
        wattron(m_DialogueWindow, COLOR_PAIR(GREEN));
        mvwprintw(m_DialogueWindow, i + 3, 2, "%s", "LEAVE");
        wattroff(m_DialogueWindow, COLOR_PAIR(GREEN));
    } else mvwprintw(m_DialogueWindow, i + 3, 2, "%s", "LEAVE");
        
    wrefresh(m_DialogueWindow);
}

void Dialogue::printUpdatedDialogue(const int & selected, bool isWarning)
{
    box(m_DialogueWindow, '|', '-');
    wattron(m_DialogueWindow, COLOR_PAIR(RED));
    wattron(m_DialogueWindow, A_UNDERLINE);
    wattron(m_DialogueWindow, A_BOLD);
    mvwprintw(m_DialogueWindow, 1, (m_Cols / 2) - m_Title.length() / 2, "%s", m_Title.c_str());
    wattroff(m_DialogueWindow, A_BOLD);
    wattroff(m_DialogueWindow, A_UNDERLINE);
    wattroff(m_DialogueWindow, COLOR_PAIR(RED));
    
    if (isWarning) wattron(m_DialogueWindow, COLOR_PAIR(YELLOW));
    
    for (int i = 0; i < (int) m_Strings.size(); i++) {
        if (i == selected) {
            wattron(m_DialogueWindow, COLOR_PAIR(GREEN));
            mvwprintw(m_DialogueWindow, i + 3, 2, "%s", m_Strings[i].c_str());
            wattroff(m_DialogueWindow, COLOR_PAIR(GREEN));
        } else mvwprintw(m_DialogueWindow, i + 3, 2, "%s", m_Strings[i].c_str());
    }
    if (isWarning) wattroff(m_DialogueWindow, COLOR_PAIR(YELLOW));
    wrefresh(m_DialogueWindow);
}
