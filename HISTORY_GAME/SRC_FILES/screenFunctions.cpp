/*****************************************************************************
 * File Name: screenFunctions.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	Screen Functions Definitions used through-out game.

 *****************************************************************************/
#include "screenFunctions.hpp"

/******************************************************
Description:
	Erases all printed screen output. System specific.
	Will ERROR if macro system_type set incorrectly.
	Designed for Windows and Unix-like OS'.

*******************************************************/
void wipeScreen()
{
	if (system_type == G_WINDOWS) { system("cls"); }
	else if (system_type == G_UNIX) { system("clear"); }
}

/******************************************************
Description:
	Formatted printing used extensively throught game.
	Prints 'msg' followed by a prompt to enter.
	Pressing <ENTER> will then call wipeScreen().
	[SEE DESCRIPTION wipeScreen()]

*******************************************************/
void printEnterWipe(string msg)
{
	cout << "\n   " << msg << "\n\n   <ENTER>";
	fflush(stdout);
	cin.get();
	wipeScreen();
}

/******************************************************
Description:
	Formatted printing used extensively throught game.
	Prints 'msg' followed by a prompt to enter. It
	DOES NOT wipeScreen(). Output from this will
	function will not be cleared from screen.

*******************************************************/
void printThenEnter(string msg)
{
	cout << "\n   " << msg << "\n\n   <ENTER>";
	fflush(stdout);
	cin.get();
}

