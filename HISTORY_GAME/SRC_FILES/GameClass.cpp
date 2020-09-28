/*****************************************************************************
 * File Name: GameClass.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:

		Function Definitions for Game Class :
			1. Instantiates Levels
			2. Connects adjacent pointers for all levels
			3. Houses Navigation & Selection functions & Menus

 *****************************************************************************/
#include"GameClass.hpp"

// Constructor
Game::Game()
{
	where_am_I = nullptr;
	position = nullptr;
	heart = 100;

	// UntouchablesOffice_Level
	office = new Office();

	// now set for beginning of game
	where_am_I = office;

	raid = new Liquor();
	lindy = new Lindy();
	estates = new EstatesGeneral();
	bastille = new Bastille();
	fear = new GreatFear();
	october = new OctoberDays();
	king = new KingFlees();
	algiers = new Algiers();
	bridge = new Bridge();

	// *********************************************
	// Linking of Spaces to create actual map below
	// *********************************************

	// UntouchablesOffice_Level
	office->setUp(lindy);
	office->setDown(raid);
	office->setRight(estates);
	office->setLeft(algiers);

	// Liquor Raid Level
	raid->setUp(office);

	// Lindy Level
	lindy->setDown(office);
	lindy->setRight(estates);

	// EstatesGeneral_Level
	estates->setUp(lindy);
	estates->setDown(fear);
	estates->setLeft(office); 
	estates->setRight(bastille);

	// Bastille_Level
	bastille->setDown(october);
	bastille->setLeft(estates);

	// GreatFear_Level
	fear->setUp(estates);
	fear->setDown(king);
	fear->setRight(october);

	// OctoberDays_Level
	october->setUp(bastille);
	october->setDown(king);
	october->setLeft(fear);

	// KingFlees_Level
	king->setUp(fear);
	king->setRight(october);

	// Algiers_Level
	algiers->setUp(bridge);
	algiers->setRight(office);

	// RR_Bridge_Level
	bridge->setDown(algiers);

}

// Destructor
Game::~Game()
{
	delete bridge; bridge = nullptr;
	delete algiers; algiers = nullptr;
	delete king; king = nullptr;
	delete october; october = nullptr;
	delete fear; fear = nullptr;
	delete bastille; bastille = nullptr;
	delete estates; estates = nullptr;
	delete lindy; lindy = nullptr;
	delete raid; raid = nullptr;
	delete office; office = nullptr;
}

// Used by setNewPosition() to determine current location
Level* Game::getWhere() { return where_am_I; }

/*****************************************************************************
 * Function Name: enterPrimaryGameMenu()
 * Description:

	Prints starting and ending game titles/credits.
	Calls runGameShell() function to perform most of gameplay

 *****************************************************************************/
int Game::enterPrimaryGameMenu()
{
	bool didWin = false;
	
	// *note: you have to buid map before you can be there
	printTitleBlock();

	// answer of <n|N> will exit program
	if (!startGameYesNo()) { return 0; }
	else { menuAfterYes(); }

	// HERE IS WHERE YOU ENTER THE GUTS OF THE GAME
	didWin = runGameShell();

	// if didWin true, then print
	printWhenAwake(didWin);

	// ending Title Block
	printEnd();
	return 0;

}

/*********************************
		PRINTING DIALOGUE
**********************************/
void Game::printTitleBlock()
{
	cout << "\n\n ______________________________________________________________________________________"
		<< "\n        - ~ WELCOME TO: GLEN GOUGEON\'s"
		<< "\n            A Nap in Time :"
		<< "\n                A History Major\'s Unexpected Journey ~ -"
		<< "\n ______________________________________________________________________________________"
		<< "\n\n   You are an undergrad History major in your senior year. All that remains for you is"
		<< "\n   to fill out units in an effort to meet the graduation requirements, but are hopelessly,"
		<< "\n   painfully, bored by the lecture. It is probably the worst GE course ever devised by man."
		<< "\n   Zoning out, you begin to nod off, but awaken in the past. \"WAKE UP\" you order yourself,"
		<< "\n   but it is no good. You are stuck in a dream. The only way out is forward. You must explore"
		<< "\n   the past to get back to the future. What will you do?";
}

/*******************************************************************
(helper) Function: startGameYesNo()

Description:
	Validates if user wants to play the game or not. Re-prompts
	where invalid input entered.

Effect:
	If <n|N> : prompts a message, then exits program.
	If <y|Y> : continues into gameplay.

********************************************************************/
bool Game::startGameYesNo()
{
	bool result = false;
	char answer = ' ';

	const char* prompt = "\n\n   You must CHOOSE:"
		"\n\n   To Play: press <y|Y> + <ENTER>"
		"\n   To Quit: press <n|N> + <ENTER>"
		"\n\n   =>";

	cout << prompt;
	answer = cin.get();
	//cin >> answer;

	while ((toupper(answer) != 'Y') &&
		(toupper(answer) != 'N'))
	{
		wipeScreen();
		cout << "\n   Please try again." << prompt;

		//cin >> answer;
		answer = cin.get();
		cout << std::endl;
	}

	if (toupper(answer) == 'N')
	{
		wipeScreen();
		cout << "\n   You don't know what you are missing."
			<< "\n\n   Press <ENTER> to Exit Game.";

		cin.ignore();
	}
	else { result = true; }

	cin.get();
	wipeScreen();

	return result;
}

// Output following selection of <y|Y> by startGameYesNo() function
void Game::menuAfterYes()
{
	// answer of <y|Y>
	printEnterWipe("DONK! (hit head on desk)");
	printEnterWipe("You're head is swimming.."
		"\n   ..and everything is a little fuzzy.. but.."
		"\n   You slowly start to see a desk.");

	printEnterWipe("Look at the desk.");

	printEnterWipe("You look down and there are several objects on the desk"
		"\n   ..right in front of you."
		"\n   You decide to pick one of them up.");

}

// Prints if WON GAME only
void Game::printWhenAwake(bool status)
{
	if (status)
	{
		wipeScreen();
		printEnterWipe("You start to hear the all too familiar commotion of"
			"\n\tplastic chairs and you see other students again.. getting up.");
		printEnterWipe("You're Awake! You made it!");
		printEnterWipe("Next time.. ask around before you pick boring classes!");
	}
}

// Conclusion
void Game::printEnd()
{
	wipeScreen();
	cout << "\n   Thank you for playing: A Nap in Time: A History Major\'s Unexpected Journey."
		<< "\n\n   The version you have just played is only a small sample of its potiential for"
		<< "\n   expansion. Originally the theme involved the works of several popular copywrited"
		<< "\n   film franchise characters, and actors. This version still demonstrates the same"
		<< "\n   essential structure used in the original variant. I hope you enjoyed it."
		<< "\n\n   The End"
		<< "\n   Press <ENTER> to exit game.";

	cin.get();
	wipeScreen();
}

/*****************************************************************************
 * Function Name: runGameShell()
 * Description:

	Performs majority of History Game functionality.

	OPTIONS/ACTIONS:
		1 ACTION   : enter mission sequence for level
		2 RECOVER  : retreive items dropped at current location
		3 SEARCH   : search level for new items
		4 DROP     : drop off an item here you don't want
		5 EAT	   : like search but for food (increase health)
		6 NAVIGATE : go to adjacent location
		
	
	HELPERS: 
		pickObjectToSetLocation() :	pick a desk object to set 1st location
		printShellHeader()	 :	prints recurring menu info. at each level	
		getAndValidateCommandChoice() : provides input validation for cmd choice
		selectNextCommand() : calls various functions from OPTIONS/ACTIONS list
		getNavChoice() : provides input validation for navigation choice
		navigationMenu() : prints navigation menu options based on position member
		setNewPosition() : sets Level* position member to point to new location
		                   updates where_am_I member
		stayInLoop()	 :  special controller to break from shell loop

	Validation done by helpers to establish correct ascii int
	equivalent of letters & numbers. SEE: getAndValidateCommandChoice | getNavChoice

 * Returns: 
		If WON GAME: TRUE
		Otherwise : FALSE
						
 *****************************************************************************/
bool Game::runGameShell()
{
		Hero* hero = new Hero();

		char option = 0;
		bool gameStatusToReturn = false;

		// available selections
		string optionsMenu =
			"\n   What do I do now?: + <ENTER>"
			"\n   ============================"
			"\n   <1|a|A>: ACTION"
			"\n   <2|r|R>: RECOVER"
			"\n   <3|s|S>: SEARCH (here)"
			"\n   <4|d|D>: DROP"
			"\n   <5|e|E>: EAT" 
			"\n   <6|n|N>: Navigate\n\n   >";

		// determines which level to start in
		pickObjectToSetLocation();

		do
		{
			// info displayed at each level
			printShellHeader(hero);

			// SEE: string : optionsMenu above
			option = getAndValidateCommandChoice(optionsMenu);

			selectNextCommand(hero, option);

		} while (stayInLoop());

		// EVALUATE WIN || LOSE || heart < 0
		gameStatusToReturn = evaluateWinLoseStatus();

		// free Hero
		delete hero; hero = nullptr;

		return gameStatusToReturn;
}

/*****************************************************************************
(helper) Function: pickObjectToSetLocation()

Description:
	Calls getNavChoice with deskObjects as menu to display. Output from
	getNavChoice is used to call setNewPosition(). This effectively sets
	the starting game location based on which item the user picks.

******************************************************************************/
void Game::pickObjectToSetLocation() 
{
	// default behavior of getNavChoice()
	// is to call: navigationMenu()
	int USE_LOCAL_MENU = 1;

	// beginning options
	string deskObjects =
		"\n   Choose an object: + <ENTER>"
		"\n\n   1. aviator goggles"
		"\n   2. a flask"
		"\n   3. .45 ACP casing"
		"\n   4. fabric scrap with Fleurs-de-lis\n\n   >";

	// validate navigation input
	char nextLocation = getNavChoice(deskObjects, USE_LOCAL_MENU);
	// determines Level you will be on
	setNewPosition(nextLocation);

}

/*****************************************************************************
(helper) Function: printShellHeader()

Description:
	Prints recurring info. for each Level of the game. Also decrements heart.
	
	Includes:
	-stats for main character
	-current location name
	-if a new level is found, print its description
	-current 'heart' [# of possible choices left before you die]
	-decrement 'heart' member tracker.

******************************************************************************/
void Game::printShellHeader(Hero* hero)
{
	// prints hero's current stats
	hero->printStats();

	cout << "\n\n   LOCATION:\n";
	// this is how I know where I am
	cout << position->getLocationName();

	// if the current location has not been discovered yet,
	// then it will print the level's description
	if (position->getCharted() == false)
	{
		cout << position->getDescription();
	}

	cout << "\n   Current Heart: " << heart << std::endl;

	// for each action the timer goes down by one
	// if heart is less than zero, GAME OVER
	heart -= 1;

}

/*****************************************************************************
 Function: getAndValidateCommandChoice()

 Parameters:
	string menu. The menu options to display

 Description:

	Performs input validation for selection of choice:
	ACTION, RECOVER, SEARCH, DROP, EAT,  & NAVIGATE.

	Won't exit until a valid choice entered by player.

Returns:
	char value of correct choice.
	(will be interpreted as ascii equivalent)

 *****************************************************************************/
char Game::getAndValidateCommandChoice(string menu)
{
	cout << menu;
	char choice = cin.get();

	// validate choice w/ ascii integer equivalents
	while (
		choice != 49 && // 1
		choice != 97 && // a
		choice != 65 && // A

		choice != 50 && // 2
		choice != 114 && // r
		choice != 82 && // R

		choice != 51 && // 3
		choice != 115 && // s
		choice != 83 && // S

		choice != 52 && // 4
		choice != 100 && // d
		choice != 68 && // D

		choice != 53 && // 5
		choice != 101 && // e
		choice != 69 && // E

		choice != 54 &&  // 6
		choice != 110 && // n
		choice != 78) // N
	{
		wipeScreen();
		cin.ignore();
		cout << "\n   Entry was: " << choice;
		printEnterWipe("Invalid Selection.");

		// re-print options
		cout << menu;

		// re-try get user choice
		choice = cin.get();

	}
	return choice;
}

/*****************************************************************************
 Function: selectNextCommand()

 Parameters:
	Hero* hero. The user's main character

 Description:

	Calls one of several functions to perform one of:
	ACTION, RECOVER, SEARCH, DROP, EAT,  & NAVIGATE.

	Won't exit until a valid choice entered by player.

Returns:
	char value of correct choice.
	(will be interpreted as ascii equivalent)

 *****************************************************************************/
void Game::selectNextCommand(Hero* hero, char option)
{
	wipeScreen();
	char navigateTo = ' ';

	switch (option)
	{
		case 49:	// ascii 1
		case 97:	// ascii a
		case 65:	// ascii A
					position->action(hero);
					break;

		case 50:	// search for what was there before : ascii 2
		case 114:   // ascii r
		case 82:    // ascii R
					position->searchFound(hero);
					cin.get();
					wipeScreen();
					break;

		case 51:	// search for new item in the Level you are in : ascii 3
		case 115:	// ascii s
		case 83:	// ascii S
					position->search(hero);
					cin.get();
					wipeScreen();
					break;

		case 52:	// leave something here : ascii 4
		case 100:	// ascii d
		case 68:	// ascii D
					position->leaveItem(hero);
					cin.get();
					wipeScreen();
					break;

		case 53:	// eat from your satchel : ascii 5
		case 101:	// ascii e
		case 69:	// ascii E
					hero->eat();
					cin.ignore();
					cin.get();
					wipeScreen();
					break;

		case 54:	// navigate somewhere else : ascii 6
		case 110:
		case 78:
					cin.ignore(); // need this to ignore 54/110/78
					navigateTo = getNavChoice();

					// determines Level you will be on
					setNewPosition(navigateTo);

					wipeScreen();
					break;
	}

}

/*****************************************************************************
(helper) Function: getNavChoice()

Parameters:	
	string menu (optional) the deskObjects menu
	int flag. If set, will print deskObjects/nothing(default)

Description:

	Validates selection of:
		(char : int)
		49 : ascii 1
		50 : ascii 2
		51 : ascii 3
		52 : ascii 4

	Loops through until 1 - 4 chosen. Prints last entry on error.
	Used to validate:
		1. deskObjects (beginning of game)
		2. direction choice (NAVIGATION)
				* calls navigationMenu() function

 Returns:	
	char choice of direction

 *****************************************************************************/
char Game::getNavChoice(string menu, int flag)
{
	if (flag == 1) { cout << menu; } // display input menu
	else { this->navigationMenu(); }


	char choice = ' ';
	choice = cin.get();


	// validate input
	while (choice != 49 && // ascii 1
		choice != 106 && // ascii j
		choice != 74 &&	// ascii J

		choice != 50 && // ascii 2
		choice != 107 && // ascii k
		choice != 75 && // ascii K

		choice != 51 && // ascii 3
		choice != 108 && // ascii l
		choice != 76 && // ascii L

		choice != 52 && // ascii 4
		choice != 59) //ascii ;
	{
		wipeScreen();
		cin.ignore();
		printEnterWipe("Invalid choice. Try again.");

		choice = ' ';
		if (flag == 1) { cout << menu; cout.flush(); }
		else { this->navigationMenu(); cout.flush(); }

		choice = cin.get();
		cout << std::endl;
	}
	return choice;
}

/*****************************************************************************
 (helper) Function Name: navigationMenu()

 Description:

	getNavChoice() helper function.
	Prints possible directions from current location (Level)
	in History Game. 
	
	Relies upon Level* 'position' to access names of levels.
	called by: getNavChoice

 *****************************************************************************/
void Game::navigationMenu()
{
	string idk = "\n   UNKNOWN";
	string up, down, left, right;
	string here = position->getLocationName();

	// UP
	if (position->getUp() != nullptr &&
	   !position->getUp()->getCharted() // already visited
	) { up = position->getUp()->getLocationName(); }
	else { up = idk; }

	// DOWN
	if (position->getDown() != nullptr &&
	   !position->getDown()->getCharted() // already visited
	) { down = position->getDown()->getLocationName(); }
	else { down = idk; }

	// LEFT
	if (position->getLeft() != nullptr &&
	   !position->getLeft()->getCharted() // already visited
	) { left = position->getLeft()->getLocationName(); }
	else { left = idk; }

	// RIGHT
	if (position->getRight() != nullptr &&
	   !position->getRight()->getCharted() // already visited
	) { right = position->getRight()->getLocationName(); }
	else { right = idk; }


	cout << "\n\n   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
		   << "\n   YOU ARE HERE:                     [USE KEYS]\n"
		   << here
		   << "\n   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		   << up 
		   << "\n   UP                                     <1|j>"
		   << "\n   ^"
		   << "\n   |-------------------------------------------"
		   << "\n   v"
		   << "\n   DOWN                                   <2|k>\n"
		   << down
		   << "\n   ============================================"
		   << "\n   <== LEFT                               <3|l>\n" 
		   << left 
		   << "\n   --------------------------------------------"
		   << "\n   RIGHT ==>                              <4|;>\n" 
		   << right 
		   << "\n   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"
		   << "\n\n   Select a direction: ==>";
}

/*****************************************************************************
 (helper) Function Name: setNewPosition()

 Description:
	Sets new location of where user has chosen to go.
	Evaluates ascii value of 'choice' to determine switch case for branching
	then sets the Level* position accordingly. Sets isDiscovered() for new
	level chosen. UPDATES where_am_I member so that getWhere() calls work
	properly.

 *****************************************************************************/
void Game::setNewPosition(char choice)
{
	wipeScreen();
	switch (choice)
	{
		case 49:	/* Gold Dice : ascii 1 */
		case 106:	// ascii j
		case 74:	// ascii J
					if (this->getWhere()->getUp())
					{
						position = this->getWhere()->getUp();
						position->isDiscovered();
						cin.get();
					}
					else
					{
						cout << "\n   You cannot move there."
							<< "\n\n   <ENTER>";
						cin.ignore();
						cin.get();
						wipeScreen();
					}
					break;

		case 50:	/* Teacup : ascii 2 */
		case 107:	// ascii k
		case 75:	// ascii K
					if (this->getWhere()->getDown())
					{
						position = this->getWhere()->getDown();
						position->isDiscovered();
						cin.get();
					}
					else
					{
						cout << "\n   You cannot move there."
							<< "\n\n   <ENTER>";
						cin.ignore();
						cin.get();
						wipeScreen();
					}
					break;

		case 51:	/* Shell casing : ascii 3 */
		case 108:	// ascii l
		case 76:	// ascii L
					if (this->getWhere()->getLeft())
					{
						position = this->getWhere()->getLeft();
						position->isDiscovered();
						cin.get();
					}
					else
					{
						cout << "\n   You cannot move there."
							<< "\n\n   <ENTER>";
						cin.ignore();
						cin.get();
						wipeScreen();
					}
					break;

		case 52:	/* Checkbook : ascii 4 */
		case 59:	//ascii ;
					if (this->getWhere()->getRight())
					{
						position = this->getWhere()->getRight();
						position->isDiscovered();
						cin.get();
					}
					else
					{
						cout << "\n   You cannot move there."
							<< "\n\n   <ENTER>";
						cin.ignore();
						cin.get();
						wipeScreen();
					}
					break;
	}
	// UPDATE WHERE YOU ARE <== IMPORTANT
	where_am_I = position;

}



/*****************************************************************************
 (helper) Function Name: stayInLoop()
 
 Description:
	Evaluates when to break out of shell (do/while) loop. Default true.
	Check is game is lost, won, or 'heart' Level member is less than zero.
	
 Returns: 
	If game is won/lost or 'heart' member is < 0 : FALSE
	Otherwise : TRUE

 *****************************************************************************/
bool Game::stayInLoop()
{
	bool status = true;

	// WON GAME
	if (position->isGameWon() == true)
	{
		status = false;
	}
	// LOST GAME
	else if (position->isGameLost() == true)
	{
		status = false;
	}
	// HEART STOPPED
	else if (heart < 0)
	{
		status = false;
	}
	return status;
}

/*********************************************************
 (helper) Function Name: evaluateWinLoseStatus()

 Description:
	Evaluates if game was won, or if heart was negative

 Returns:
	If WON GAME : TRUE
	Otherwise : FALSE

 *********************************************************/
bool Game::evaluateWinLoseStatus()
{
	bool result = false;
	if (position->isGameWon() == true)
	{
		result = true;
	}

	if (heart < 0)
	{
		cout << "\n   Your heart stopped! GAME OVER!"
			<< "\n\n   Press <ENTER> to Exit.";
		cin.get();
	}
	return result;
}
//***************************** E O F ********************************


