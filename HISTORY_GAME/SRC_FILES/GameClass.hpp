/*****************************************************************************
 * File Name: GameClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:
		
			Specification for Game Class :
				1. Instantiates Levels
				2. Connects adjacent pointers for all levels
				3. Houses Navigation & Selection functions & Menus

 *****************************************************************************/
#ifndef GGOUGEON_GAME_CLASS_HPP
#define GGOUGEON_GAME_CLASS_HPP

#include<cstdlib>
#include<iostream>
using std::cin;
using std::cout;

#include<string>
using std::string;

/**************************************
LEVELS:
-------
	BASE: LevelClass.hpp
	SUB-LEVELS: below

***************************************/
#include "UntouchablesOffice_Level.hpp"
#include "EstatesGeneral_Level.hpp"
#include "Bastille_Level.hpp"
#include "GreatFear_Level.hpp"
#include "OctoberDays_Level.hpp"
#include "FlightOfKing_Level.hpp"
#include "Algiers_Level.hpp"
#include "RR_Bridge_Level.hpp"
#include "LiquorRaid_Level.hpp"
#include "Lindy_Level.hpp"

class Game
{
	private:

		// health meter
		int heart;

		// map holds locations
		Level *where_am_I;
		Level *position;

		// Level pointers
		Office* office;
		Liquor* raid;
		EstatesGeneral* estates;
		Bastille* bastille;
		GreatFear* fear;
		OctoberDays* october;
		KingFlees* king;
		Algiers* algiers;
		Bridge* bridge;
		Lindy* lindy;

	public:
		Game();
		~Game();

		Level* getWhere();

		int enterPrimaryGameMenu();
		void printTitleBlock();
		bool startGameYesNo();
		void menuAfterYes();

		// SHELL FUNCTIONS
		void printWhenAwake(bool);
		void printEnd();

		// SHELL WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
		bool runGameShell();
		void pickObjectToSetLocation();
		void printShellHeader(Hero*);

		// command selection
		char getAndValidateCommandChoice(string);
		void selectNextCommand(Hero*, char);

		// level navigation 
		char getNavChoice(string = " ", int = 0);
		void navigationMenu();
		void setNewPosition(char); 

		bool stayInLoop(); 
		// MMMMMMMMMMMMMMMMMMMMMMMMMMM end SHELL

		bool evaluateWinLoseStatus();
};
#endif 

//***************************** E O F ********************************
