/*****************************************************************************
 * File Name: LevelClass.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	Function Definitions for Level Class.

 *****************************************************************************/
#include"LevelClass.hpp"

// Constructor
Level::Level()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	WON_GAME = false;
	LOST_GAME = false;

	location_Name = " ";
	date = NULL;
	description = "\n   You cannot go that way.\n";
	significance = "\n   Insignificant.\n";

	myInventory = new Inventory;
	itemStack = new Stack;
	droppedItems = new Queue;
}

// Destructor
Level::~Level()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	delete itemStack;
	itemStack = nullptr;
	delete myInventory;
	myInventory = nullptr;
	delete droppedItems; // Level
	droppedItems = nullptr; // Level

	delete date; date = nullptr;
}

/*****************************************************************************
* NAVIGATION FUNCTIONS BELOW FOR ACCESSING THE NODE AS LISTED

* NOTE*:
	They are at first relative to the starting position: UntouchablesOffice_Level.

 *****************************************************************************/

// UP
Level* Level::getUp()
{
	return up;
}

void Level::setUp(Level* above)
{
	up = above;
}

// DOWN
Level* Level::getDown()
{
	return down;
}

void Level::setDown(Level* below)
{
	down = below;
}

// LEFT
Level* Level::getLeft()
{
	return left;
}

void Level::setLeft(Level* before)
{
	left = before;
}

// RIGHT
Level* Level::getRight()
{
	return right;
}

void Level::setRight(Level* after)
{
	right = after;
}

// LOCATION NAME
void Level::setLocationName(string title)
{
	location_Name = title;
}

string Level::getLocationName() const
{
	return this->location_Name;
}

// DATE
void Level::setDate(string m, int d, int y)
{
	date = new Date(m, d, y);
}

Level::Date* Level::getDate()
{
	return this->date;
}

void Level::printDate()
{
	cout << "\n   ===================="
		 << "\n     Historical Date:"
		 << "\n   ===================="
		 << "\n   MONTH: " << date->month
		 << "\n     DAY: " << date->day
		 << "\n    YEAR: " << date->year
		 << "\n   ====================\n";
}

// DESCRIPTION
void Level::setDescription(string hint)
{
	description = hint;
}

string Level::getDescription() const
{
	return description;
} 

// SIGNIFICANCE
void Level::setSignificance(string hint)
{
	significance = hint;
}

string Level::getSignificance() const
{
	return significance;
}

/*****************************************************************************
* Function Names:
	isUncharted(), isDiscovered(), & getCharted()

* Description:
	 The following 3 functions relate if the Level has been visited
	 so that navigation options may be evaluated.

 *****************************************************************************/
void Level::isUncharted()
{
	uncharted = true;
}

void Level::isDiscovered()
{
	uncharted = false;
}

bool Level::getCharted()
{
	return uncharted;
}

/*******
  WIN
********/
// sets flag for winning the game accessed in GameClass
void Level::setWonGame()
{
	WON_GAME = true;
}

// returns flag for determining if the game was won 
// (accessed in GameClass)
bool Level::isGameWon()
{
	return this->WON_GAME;
}

/*******
  LOSE
********/
// sets flag for losing the game
// (accessed in GameClass)
void Level::setLostGame()
{
	LOST_GAME = true;
}
// returns flag for determining if the game was lost 
// (accessed in GameClass)
bool Level::isGameLost()
{
	return this->LOST_GAME;
}

/*****************************************************************************
 (helper) Function: checkYesNo()

 Parameters: 
	strint prompt. The input buffer data from keyboard.

Description:
		Looping yes or no input validator. 
		Depends upon:
			1. toUpper() library function
			2. wipeScreen() [global scope] function I created.
				function to clear screen 
				
				[SEE: wipeScreen() description in screenFunctions.cpp/.hpp]

Effect:
		Repeatedly asks for input, and will only accept a single:
		y, Y, n, N
		Prints prompt to indicate expected input.

Returns: 
		single char y/Y or n/N only 

  *****************************************************************************/
char Level::checkYesNo(string prompt)
{
	char input = ' ';
	cin >> input;

	while ((toupper(input) != 'Y') &&
		(toupper(input) != 'N'))
	{
		wipeScreen();
		cout << "\n  "
			 << prompt
			 << "\n\n   [enter] YES <y|Y> -or- NO <n|N>\n\n   =>";

		cin >> input;
		cout << std::endl;
	}
	return input;
}

/*****************************************************************************
 * Function Name: printNewItemFound()
 * Description:
		Helper. Takes a customized string as game prompt. Calls printItem()
		to display the item member variables

 *****************************************************************************/
void Level::printNewItemFound(Item* thing, string customSaying)
{
	cout << "\n   " << customSaying << ": ";
	thing->printItem();
	cout << "\n\n   Take it or leave it?"
		<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";
}

/*****************************************************************************
 * Function Name: printUpdatedQueue()
 * Description:
		Helper to display what is in the queue (satchel) of input 'whom'.
		Prints header, then calls printQueue on Character* whom.
		Does not call wipeScreen() function.

 *****************************************************************************/
void Level::printUpdatedQueue(Character* whom)
{
	// stored in Hero's satchel queue
	wipeScreen();
	cout << "\n   Let's see what's in " << whom->getName() << "\'s satchel now:"
		<< "\n   ======================================================================";
	whom->getSatchel()->printQueue();
	printThenEnter(" ");
}

/**********************************************************
(helper) Function: learnFrenchFromConcordance()

Parameters:
	Character* who. Typically the user's player.

Description:
	Gives 'who' the ability to translate into English,
	dialogue in French. Only applies to specific
	CHALLENGE function e.g.
		talkToWomen() & talkToGuard()

Effects:
	Input 'who' character now has special ability: FRENCH

***********************************************************/
void Level::learnFrenchFromConcordance(Character* who)
{
	if (who->getAbility() != FRENCH)
	{
		// attempt to understand french
		printEnterWipe("Maybe a concordance would be helpful?");
		printEnterWipe("Look for one in your satchel.");

		if (who->iHaveThis("\n   English-to-French Concordance"))
		{
			printEnterWipe("Nice! you got one. It\'s as if FRENCH is your speciality..");
			// you can now speak french!
			who->setAbility(FRENCH);
		}
		else
		{
			printEnterWipe("Hmm.. darn. You\'ll have to keep guessing.");
		}
	}
}

/*********************************************************************************
Function Name: chooseResponse()

Parameters:
	string* options. A string array of possible options for the user to choose.
	int size. size of options array.
	int key. The correct option within the options array parameter (not the index)

Description:
	User is presented with the 'options' to select, until the correct one is
	choosen. A score is returned based on the option selected. This is an
	abstraction for how well the player responds to game characters.

Returns:
	a score based on user's ability to select the correct options[key]

**********************************************************************************/
int Level::chooseResponse(string* options, int size, int key)
{
	int choice = 0;
	int score = 0;

	wipeScreen();

	// print out all response options
	cout << "\n   What should I say?\n   OPTIONS:\n";
	for (int i = 0; i < size; i++)
	{
		cout << "\n   " << i + 1 << ": " << options[i];
	}
	cout << "\n\n   [enter] =>";

	// user input
	cin >> choice;

	// test if equal to correct answer
	if (choice != key)
	{
		score -= 1;
	}
	else
	{
		score += 1;
	}

	return score;
}

/*********************************************************************************
Helper Function Name: speechConsequences()

Parameters:
	Character* person. THIS Character.
	int score. The result of chooseResponse() called by 
	(for example) talkToDelegate() 

Description:
	This increments/decrements person's strength by the value score. 
	It abstracts saying the wrong thing, and incurring the consequences.

Effect:
	person parameter strength affected by the value of input score

**********************************************************************************/
void Level::speechConsequences(Character* person, int score)
{
	int tempStrength = person->getStrength();
	tempStrength += score;
	person->setStrength(tempStrength);
}

/*****************************************************************************
Function: searchFound()

Parameters: 
	Character* hero. The user's Hero Character (usually).

Description:

	Handles action	RETRIEVE request in game.
	1. Takes user y/n input
	2. Validates input
	3. If the Stack that contains the dropped items
		is empty..will indicate that with print statements.

	4. Double checks if input was y/Y
		4b. If y/Y, then enters main branch
			4c. If Stack contains an item, will ask user if they
				want it.
					4d. If yes, then it goes in whom's 'satchel'
						4e. Will print the updated 'satchel' (queue) contents

	5. Otherwise prints a different statement

*****************************************************************************/
void Level::searchFound(Character *hero)
{
	cout << "\n   Now where did I leave that thing?? ~~"
		 << "\n   Do you want to get something you already found here?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	string leftBehind = "Oh it's here alright..I think."
		"\n                      ...do you still want to look for what you left behind?";

	// validate
	char choice = checkYesNo(leftBehind);

	// If the local droppedItems queue is empty
	if (this->droppedItems->isEmpty())
	{
		printThenEnter("I thought it was here, but now it's gone.");
	}
	// If there is something you left behind
	else
	{
		if (toupper(choice) == 'Y')
		{
			wipeScreen();
			cout << "\n   I located:"
				 << "\n\n   " << this->droppedItems->peekSatchel() << " right where I left it."
				 << "\n   Should I pick this up again?"
				 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

			// validate 
			char grab = checkYesNo("um ...do you still want to get what you left behind?");

			if (toupper(grab) == 'Y')
			{
				Item* temp = nullptr;
				temp = this->droppedItems->getFront();

				// place in satchel
				hero->getSatchel()->enqueue(temp);
				// show effect
				this->printUpdatedQueue(hero);
			}

		}
		else
		{
			printThenEnter("I can get that later.");
		}
	}

}

/***********************************************************************
Function: leaveItem()

Parameters: 
	Character* hero. The user's Hero Character (usually).

Description:

	Handles action DROP request in game.
	1. Prompts user input yes or no
	2. Validates input
	3. If yes:
		3b. print hero's 'satchel' (queue) contents
		3c. prompts again yes or no to drop an item
		3d. Validates input
		3e. If yes:
			3f. Show item at front of queue
			3g. prompt if that is the item to dump
			3h. If yes:
				place item in droppedItems Stack
			3i. If no:
				enqueue back into hero's queue
			3j. REPEAT 3f.->3i. until an item dropped
	4. If no:
		print statement about not needing it
				
***********************************************************************/
void Level::leaveItem(Character *hero)
{
	cout << "\n   Do you want to see what\'s in your satchel first?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char view = checkYesNo("You just have to open it. Do you want to look in your satchel first?");

	// yes I want to look in my bag first
	if (toupper(view) == 'Y')
	{
		wipeScreen();
		cout << "\n   Satchel contents:"
			 << "\n   =================";
		hero->getSatchel()->printQueue();
		cin.ignore();
		printEnterWipe(" ");
	}

	cout << "\n   Do you still want to drop an item off at this location?"
	     << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	view = checkYesNo("Huh? You can always pick it up later.");

	// yes I want to drop something here
	if (toupper(view) == 'Y' && !hero->getSatchel()->isEmpty())
	{
		char repeatDump = ' ';

		// this do/while allows you to drop more than 1 item while you are here
		do
		{
			bool dumped = false;
			char leavingThis = ' ';

			// this is the process of looking, and choosing what item to dump here
			do
			{
				if (!hero->getSatchel()->isEmpty())
				{
					wipeScreen();
					cout << "\n   Is this it? :"
						<< "\n   ============="
						<< "\n   "
						<< hero->getSatchel()->peekSatchel()
					    << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

					// validate
					leavingThis = checkYesNo("Huh? You can always pick it up later."
						"\n   Do you want to dump this item?:");
				}

				// yes I found the item already
				if (toupper(leavingThis) == 'Y')
				{
					Item *transfer = nullptr;

					transfer = hero->getSatchel()->getFront();
					this->droppedItems->enqueue(transfer);

					transfer = nullptr;
					dumped = true;
				}
				// didn't find it yet..I need to search through my other items
				else
				{
					hero->getSatchel()->rotateSatchel();
				}

			} while (dumped == false && !hero->getSatchel()->isEmpty());

			wipeScreen(); 

			cout << "\n   Do you want to leave anything else here?"
				 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

			// validate leave more items here
			repeatDump = checkYesNo("Huh? You can always pick it up later."
				"\n   Do you want to dump another item here?");
			
		} while (toupper(repeatDump) == 'Y');
	}
	else
	{
		cout << "\n   I guess I don't have anything to drop off.\n\n   <ENTER>";
		cin.get();
	}

}

//***************************** E O F ********************************
