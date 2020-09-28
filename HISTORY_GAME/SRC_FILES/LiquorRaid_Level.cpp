/*****************************************************************************
 * File Name: LiquorRaid_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Function Definitions for Liquor Class.

 *****************************************************************************/
#include"LiquorRaid_Level.hpp"

// Default Constructor
Liquor::Liquor()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->shotgun());
	itemStack->push(myInventory->warrant());
	itemStack->push(myInventory->hamburger());

	// Untouchables Office
	this->setLocationName("\n   \'Outfit\' Bootleggin\' operation.\n");
	this->setDate("June", 1, 1930);
	this->setDescription("\n   One of many. Suspected operation run by the gangster, one Alfonso Capone.\n");

	string explanation = "\n   The Volstead Act made the consumption and production of Alcohol illegal"
		"\n   in the United States of America by means of the 18th amendment to the Constitution."
		"\n   The black-market on boos was a staple of organized crime personalities such as Al Capone."
		"\n   Bribery, murder, and intimidation were his tools of choice. Elliot Ness and the \'Untouchables\'"
		"\n   acted as special agents to expose and publicize Capone\'s illegal activities. The tactic was meant"
		"\n   to apply negative pressure to Capone\'s image and income.";

	this->setSignificance(explanation);

	introRead = false;
	raidFinished = false;
}

// Destructor
Liquor::~Liquor() {}

/**********************************************************************************
Function Name: search()
Description:
		Performs action SEARCH option on a Level

		STEPS:
			1. Prompt yes or no input to search
			2. Validates step 1 input
			3. If yes:
				AND itemStack is empty:
					prints indicate so
				AND itemStack is NOT empty:

					a) pop item off 'itemStack' (BASE class member of LevelClass)
					b) print item popped
					c) ask if user wants it y/Y/n/N only (validates)
					d) If doesn't want:
						push onto 'droppedItems' Stack (BASE class member of LevelClass)
					e) If wants:
						enqueue in hero's 'satchel' (queue)

			4. If no: exit SEARCH

Parameters: Character* hero (YOU in game)

Effect:
		Gain item popped or it is left for recovery later

*************************************************************************************/
void Liquor::search(Character* hero)
{
	cout << "\n   Should I be looking for something here?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("Why can't I make up my mind?");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("They didn\'t leave a trace. Nothing.");
		}
		else
		{
			Item *temp = nullptr;
			temp = itemStack->pop();

			wipeScreen();

			// displays what was found w/ custom dialogue
			this->printNewItemFound(temp, "what\'s this here? looks like");

			// validate
			char response = checkYesNo("Ah.. Mr. Ness? Sir?");

			// leaving behind at this location
			if (toupper(response) == 'N')
			{
				printThenEnter("I don't care about that.");

				// this will place it in a queue at this location
				this->droppedItems->enqueue(temp);
			}
			else
			{
				// place in satchel
				hero->getSatchel()->enqueue(temp);
				// show effect
				this->printUpdatedQueue(hero);
			}

		}
	}

}

/*********************************************************************************
PURE VIRTUAL Function Name: action()
Description:
	Executes ACTION cmd option for this level. Hero is attacked by 4 henchmen
	in a liquor raid. Requires a warrant first.

Effect:
	If successful: hero gains Al Capone's ledger. Ledger can WIN GAME.
	If failure: Possibly death [LOSE GAME]

**********************************************************************************/
void Liquor::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { liquorRaidIntro(); }
	
	// MISSION
	if (!raidFinished) { raidFinished = liquorRaid(hero); }

	// level completed
	else{ printEnterWipe("We already raided this spot. They won\'t try that here again."); }
}

// Basic historical information, and dialogue
void Liquor::liquorRaidIntro()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("It is the height of Prohibition in America.");
	printEnterWipe("The summer of 1930, Chicago, USA.");
	printEnterWipe("You\'ve been hitting Capone where it hurts..");
	printEnterWipe("and making it public but it still isn\'t enough.");
	printEnterWipe("Treasury wants evidence of his accounts but..");
	printEnterWipe("I\'ve never seen ANY paper trail with these guys.");
	printEnterWipe("They\'re very cool customers.");
	printEnterWipe("Stay alive.. but look for something we can nail this guy with!");

	introRead = true;
}

/****************************************************************
Function: liquorRaid()

Parameters:
	Character* ness. The user's player.

Description:
	
*****************************************************************/
bool Liquor::liquorRaid(Character* ness)
{
	bool successfulRaid = false;

	printEnterWipe("Your \'Untouchables\' are ready.. all ten of them.");
	printEnterWipe("But this has to be legal. We need a warrant.");

	cout << "\n   Did you bring it?"
		<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("You know.. that thing we need to break in legally?");
	if (toupper(password) == 'Y')
	{
		// formatting
		wipeScreen();
		cin.ignore();

		// validate you have a warrant
		if (ness->iHaveThis("\n   Arrest warrant"))
		{
			printEnterWipe("Okay good.. it\'s time to break down the door.");
			// action
			successfulRaid = fightFourHenchmen(ness);
		}
		// lied: no warrant
		else
		{
			printEnterWipe("C\'mon Ness.. don\'t mess this up!");
			printEnterWipe("You\'ll have to go back to the Bureau and get one!");
		}
	}
	else
	{
		wipeScreen();
		cin.ignore();
		printEnterWipe("You\'ll have to go back to the Bureau and get one then!");
	}

	return successfulRaid;
}
/****************************************************************
(helper) Function: fightFourHenchmen()

Parameters:
	Character* ness. Elliot Ness. The user's player.

Description:
	Liquor raid has ness fight 4 henchmen to retrieve 
	Al Capone's Accounting Ledger.

Effect
	Possibilities:
		1. gain weapons (trophies)
		2. healing (strength increased)
		3. gain Al Capone's Ledger Book
		4. leave Ledger at this level
		5. death and LOSE GAME 
			-shot to death
			-explosion

*****************************************************************/
bool Liquor::fightFourHenchmen(Character* ness)
{
	bool survived = false;
	bool blownUp = false;

	printEnterWipe("Bureau of Alcohol!");
	printEnterWipe("You are all under arrest..");
	printEnterWipe("Uh-oh! They\'re pulling out an arsenal!");
	printEnterWipe("I see Capone\'s thugs.. there\'s..");

	Henchmen* nitti = new Henchmen();
	nitti->setName("Frank Nitti");
	nitti->setAbility(DUMB);
	nitti->printStats();
	printEnterWipe(" ");
	nitti->addWeaponBonus(myInventory->shotgun());

	Henchmen* jack = new Henchmen();
	jack->setName("\"Machine-Gun\" Jack McGurn");
	jack->setStrength(10);
	jack->printStats();
	printEnterWipe(" ");
	jack->addWeaponBonus(myInventory->Thompson());

	Henchmen* belcastro = new Henchmen();
	belcastro->setName("James Belcastro");
	belcastro->printStats();
	printEnterWipe(" ");
	belcastro->getSatchel()->enqueue(myInventory->exploders());

	Henchmen* gio = new Henchmen();
	gio->setName("Giovanni");
	gio->setStrength(10);
	gio->printStats();
	printEnterWipe(" ");
	gio->addWeaponBonus(myInventory->Thompson());

	// 'Sound' Effects (onomatopoeia)
	string MG = "\n   pap-pap-pap-pap-pap-pap!!! -pap ~CLICK~";
	string shotgun = "\n   BOOM! BOOM!";
	string revolver = "\n   BANG ~~ .  whiz";

	int combatResults = 0;

	combatResults = twoCharactersCombat(ness, revolver, nitti, shotgun);

	if (combatResults == 1)
	{
		// TROPHY
		ness->getNewTrophy(nitti, "\n   Double Barrel Shotgun");

		combatResults = twoCharactersCombat(ness, shotgun, jack, MG);

		if (combatResults == 1)
		{
			// ironically have a drink
			drinkWhiskey(ness);

			// TROPHY
			ness->getNewTrophy(jack, "\n   Thompson sub-machine-gun");

			combatResults = twoCharactersCombat(ness, MG, belcastro, "tick-tock");

			if (combatResults == 1)
			{
				printEnterWipe("BELCASTRO HAD A BOMB LOOK OUT!");
				Item* bomb = belcastro->getSatchel()->searchSatchel("\n   Suitcase of Explosives");
				printEnterWipe(" ");

				if (ness->rollInitiative() > 3) 
				{
					printEnterWipe("Geeeezz! COME ON!");

					combatResults = twoCharactersCombat(ness, MG, gio, MG);

					if (combatResults == 1)
					{
						ledgerFound(ness);
						survived = true;
						wipeScreen();
					}
				}
				else
				{
					printEnterWipe("James Belcastro just blew you up!");
					ness->setStrength(-100);
					blownUp = true;
				}

				// disarm bomb
				delete bomb; bomb = nullptr;
			}

		}
	}

	if (survived) { printEnterWipe("That raid was too close."); }

	if (ness->getStrength() < 0)
	{
		if (!blownUp) 
		{ 
			printEnterWipe("\n   Capone\'s thugs shot you to death!"); 
		}
		printEnterWipe("GAME OVER !!");
		this->setLostGame();
	}
	else
	{
		// printing
		ness->postCombatStats();

		// heal some
		ness->health();
	}

	delete gio; gio = nullptr;
	delete belcastro; belcastro = nullptr;
	delete jack; jack = nullptr;
	delete nitti; nitti = nullptr;

	return survived;
}

/***************************************************************
(helper) Function: drinkWhiskey()

Description:
	Calls addFoodBonus() [Character Class] helper to give
	ness a shot of whiskey between combat.

Effect:
	Ness' strength increased by drinking whiskey.

****************************************************************/
void Liquor::drinkWhiskey(Character* ness)
{
	ness->addFoodBonus(myInventory->whiskey());
	cin.get();
	wipeScreen();
}

/***************************************************************
(helper) Function: ledgerFound()

Parameters:
	Character* ness. Elliot Ness. The user's player.

Description:
	Performs option to take important Al Capone Item* 
	following final combat with Henchmen. Option to leave
	Ledger will place it into Level's itemStack.

Effect:
	If select to keep: Ledger is in ness' satchel (queue)
	If select to leave: Ledger goes to itemStack for possible
						later retreival.

****************************************************************/
void Liquor::ledgerFound(Character* ness)
{
	printEnterWipe("NO WAY!");
	Item* ledger = myInventory->caponesLedger();
	printNewItemFound(ledger, "Looking down you find..");

	// validate
	char password = checkYesNo("It\'s the evidence we\'ve been hunting for!");
	if (toupper(password) == 'Y')
	{
		ness->getSatchel()->enqueue(ledger);

		// print ness' satchel (queue)
		printUpdatedQueue(ness);
		cin.ignore();
	}
	else
	{
		// place in Level stack
		this->droppedItems->enqueue(ledger);

		wipeScreen();
		cin.ignore();
		printEnterWipe("You\'re the boss, Boss.");
		printEnterWipe("We can come back for it.");
	}

}

//***************************** E O F ********************************
