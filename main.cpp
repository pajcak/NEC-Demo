//    File: main.cpp
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


#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <string>
#include "MenuWindow.h"
//#include "Controls.h"
#include "Demo.h"
#include "Dialogue.h"
#include <signal.h>

using namespace std;

void signalHandler(int signalValue) {
	endwin();
	printf("Segmentation fault!\n");
	exit(1);
}


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
    
	signal(SIGSEGV, signalHandler);

    system("resize -s 30 80");
    Demo * demo = new Demo();
    demo->startMenu();
    delete demo;
    
    return 0;
}

