/* 
 * File:   main.cpp
 * Author: Patrik Faistaver
 *
 * Created on May 17, 2014, 7:08 PM
 */

#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string>
#include "MenuWindow.h"
//#include "Controls.h"
#include "Demo.h"
#include "Dialogue.h"

using namespace std;

int main(int argc, char** argv)
{
//    initscr();
//    keypad(stdscr, TRUE);
//    cout << getch();
//    endwin();
//    return 0;
    
//    initscr();
//    start_color();
//    Dialogue * d = new Dialogue("Dialog", "Enemy has crushed you with his Mortal Strike!asdasddasdsadasdsa", Position(10,10));
//    d->showDialogue();
//    delete d;
//    return 0;
//    endwin();
    
    system("resize -s 26 80");
    Demo * demo = new Demo();
    demo->startMenu();
    delete demo;
    
    return 0;
}
