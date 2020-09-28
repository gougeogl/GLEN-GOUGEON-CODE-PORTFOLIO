/*****************************************************************************
 * File Name: LevelClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	Function Specifications for Level Class.

	A Level is a level of the game, a location on a map, and a period in time.
	The game is a history major's dream-scape so it bounds places and times.
	Each level portrays some historical event in which the user's main game
	Character [HeroClass] must attempt to accomplish some goal usually
	requiring specific objects either found or earned in the game. 

	Each Level has:
		1. a MISSION OBJECTIVE [ACTION cmd]
		2. A place to look for new items [SEARCH cmd]
			may include:
				-FOOD	-WEAPONS	-BOMBS	-DOCUMENTS	-THINGS
				-special items that help you win the game

		3. A place to drop items from your satchel(queue) [DROP cmd]
		4. A place to recover dropped items [RECOVER cmd]

 *****************************************************************************/
#ifndef GGOUGEON_LEVEL_CLASS_HPP
#define GGOUGEON_LEVEL_CLASS_HPP
#define _CRT_SECURE_NO_WARNINGS

// file access macros
// ******************
#ifdef _WIN32
#include <direct.h>
#define cwd _getcwd
#define cd _chdir
#else
#include <unistd.h>
#define cwd getcwd
#define cd chdir
#endif
// *******************
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include<cstdlib>
#include<iostream>
using std::cin;
using std::cout;
using std::endl;

#include<string>
using std::string;

#include <fstream>
using std::ifstream;

/**************************************
GAME CHARACTERS:
----------------
	BASE: CharacterClass.hpp
	SUB-CLASSES: below

***************************************/
#include "HeroClass.hpp" // <--- user's game Character

// French Revolution
#include "LouisXVI.hpp"
#include "MarieAntoinette.hpp"
#include "Priest.hpp"
#include "NobleSword.hpp"
#include "Bourgeoisie.hpp"
#include "PalaceGuards.hpp"
#include "Peasant.hpp"
#include "HouseWife.hpp"

// WWII
#include "Nazi.hpp"

// PROHIBITION
#include "Henchmen.hpp"

// Combat
#include "twoCharactersCombat.hpp"

/**************************************
VEHICLES:
----------------
	BASE: VehicleClass.hpp
	SUB-CLASSES: below

***************************************/
#include "FoodCart_Vehicle.hpp"
#include "Carriage_Vehicle.hpp"
#include "SpiritStLouis_Vehicle.hpp"
#include "FishingBoat_Vehicle.hpp"

/************************
	DATA STRUCTURES
*************************/
#include"QueueClass.hpp"
#include"stack.hpp"

/************************
 LEVEL STORAGE / ITEMS
*************************/
#include "item.hpp"
#include "InventoryClass.hpp"

// output formatting
#include "screenFunctions.hpp"

#define MAX_BUFFER 2048

class Level 
{
	protected:
		Level *up;
		Level *down;
		Level *right;
		Level *left;

		struct Date
		{
			string month;
			int day;
			int year;

			Date(string m, int d, int y)
			{
				month = m;
				day = d;
				year = y;
			}

			~Date()
			{
				month = " ";
				day = 0;
				year = 0;
			}
		};

		string location_Name;
		Date* date;
		string description;
		string significance;

		// holds whether a Level has been visited yet
		bool uncharted;

		// flags for winning / losing the game
		bool WON_GAME;
		bool LOST_GAME;

		// Level Storage (data structures)
		Inventory * myInventory;
		Stack * itemStack; 
		Queue * droppedItems;
		
	public:

		Level();		
		virtual ~Level();

		// connect to other spaces
		Level* getUp();
		void setUp(Level*);

		Level* getDown();
		void setDown(Level*);

		Level* getLeft();
		void setLeft(Level*);

		Level* getRight();
		void setRight(Level*);
		
		// LOCATION NAME
		void setLocationName(string);
		string getLocationName() const;

		// DATE
		void setDate(string, int, int);
		Date* getDate();
		void printDate();

		// DESCRIPTION
		string getDescription() const;
		void setDescription(string);	

		// SIGNIFICANCE
		string getSignificance() const;
		void setSignificance(string);

		// DISCOVERY related
		void isUncharted(); // sets uncharted variable to true, meaning hasn't been visited yet
		void isDiscovered();
		bool getCharted();
		
		// WON
		void setWonGame();
		bool isGameWon();

		// LOST
		void setLostGame();
		bool isGameLost();

		// VALIDATION
		char checkYesNo(string);

		// PRINTING
		void printNewItemFound(Item*, string);
		void printUpdatedQueue(Character*);

		// Talking to Characters
		void learnFrenchFromConcordance(Character*);
		int chooseResponse(string*, int, int);
		void speechConsequences(Character*, int);

		// ACTION (pure virtual)
		virtual void action(Character*) = 0;

		// SEARCH (pure virtual)
		virtual void search(Character*) = 0;

		// RECOVER
		void searchFound(Character*);

		// DROP
		void leaveItem(Character *);

};			
#endif 

//***************************** E O F ********************************
