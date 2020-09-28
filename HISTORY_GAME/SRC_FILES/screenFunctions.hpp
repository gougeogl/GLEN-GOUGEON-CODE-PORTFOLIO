/*****************************************************************************
 * File Name: screenFunctions.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	Screen Functions Specifications used through-out game.

 *****************************************************************************/
#ifndef GGOUGEON_SYS_TYPES
#define GGOUGEON_SYS_TYPES
#define G_WINDOWS 1977 // Star Wars A New Hope Released
#define G_UNIX 1642 // English Revolution
//#define system_type G_WINDOWS
#define system_type G_UNIX
#endif

#ifndef GOUGEON_SCREEN_FUNCTIONS_HPP
#define GOUGEON_SCREEN_FUNCTIONS_HPP

#include<cstdlib>
#include<iostream>
using std::cin;
using std::cout;
using std::endl;

#include<string>
using std::string;

// Custom Outputs
void wipeScreen(); // handles cls(WINDOWS) & clear(UNIX)
void printEnterWipe(string);
void printThenEnter(string);

#endif
