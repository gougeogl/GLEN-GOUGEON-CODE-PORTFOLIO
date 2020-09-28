/*****************************************************************************
 * File Name: Bastille_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20

 * Description: Function Definitions for Bastille (Level) Class.

	SEE: Action() below for Level description.

 *****************************************************************************/
#include"Bastille_Level.hpp"

// Default Constructor
Bastille::Bastille()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->staleBread());

	itemStack->push(myInventory->RoundShot());
	itemStack->push(myInventory->RoundShot());
	itemStack->push(myInventory->RoundShot());
	itemStack->push(myInventory->RoundShot());
	itemStack->push(myInventory->RoundShot());

	itemStack->push(myInventory->Artillery());
	itemStack->push(myInventory->RoundShot());

	itemStack->push(myInventory->Artillery());
	itemStack->push(myInventory->RoundShot());

	itemStack->push(myInventory->Artillery());
	itemStack->push(myInventory->RoundShot());

	itemStack->push(myInventory->Artillery());
	itemStack->push(myInventory->RoundShot());

	itemStack->push(myInventory->Artillery());
	itemStack->push(myInventory->RoundShot());


	// Apocalypse Now
	this->setLocationName("\n   Bastille Saint-Antoine, Paris, France.\n");
	this->setDate("July", 14, 1789);
	this->setDescription("\n   Parisians attack a prison known as \'The Bastille.\'\n");

	string explanation = "\n   Political prisoners of the King were sent to this fortress"
		"\n   turned prison, and it eventually became a symbol of the monarch\'s despotic rule."
		"\n   When parisians of the third estate successfully captured the Bastille by force,"
		"\n   it proved a turning point in the revolution as the beginning of hostilities.";

	this->setSignificance(explanation);

	allCannonsIn = false;
	cannonCount = 0;

	fullMagazine = false;
	ammoCount = 0;

	introCompleted = false;
	sneakAttackCompleted = false;
	reload = false;
	missionCompleted = false;
}

// Destructor
Bastille::~Bastille() {}

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

Parameters: Character* hero ( YOU in game)

Effect:
		Gain item popped or it is left for recovery later

*************************************************************************************/
void Bastille::search(Character* hero)
{
	cout << "\n   Care to loot the Bastille?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("It\'s a horrific place.. are you sure you don\'t want take something back?");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("It\'s been pillaged already.");
		}
		else
		{
			Item *temp = nullptr;
			temp = itemStack->pop();

			wipeScreen();

			// displays what was found w/ custom dialogue
			this->printNewItemFound(temp, "what\'s this here? looks like");

			// validate
			char response = checkYesNo("Monsieur? .. your requisition orders Sir?");

			// leaving behind at this location
			if (toupper(response) == 'N')
			{
				printThenEnter("hmm..   I don't want that now.");

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

	The storming of the Bastille prision on July 14, 1789 was the first
	violent act of the first French Revolution. The Fortress turned prision
	was where the Monarchy sent embarrassments to the state, and opposers of
	the King. It was pillaged in effigy by a cohort of small shop keepers,
	day laborers, and vagrants.

**********************************************************************************/
void Bastille::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	if (!introCompleted) { introSequence(hero); }

	if (!sneakAttackCompleted)
	{
		printEnterWipe("See what you can find for weaponry.. then attack the Bastille!");

		if (!hero->findAnyWeapon())
		{
			printEnterWipe("Bare hands won\'t work here.. go find a weapon.");
		}
		else { sneakAttackFirst(hero); }
	}
	// 2nd + visit to level
	else { printEnterWipe("You\'re back.."); }

	// Get Cannons in Position
	if (sneakAttackCompleted && !allCannonsIn)
	{
		printEnterWipe("PEEEEESSHSHSHSHSH-WHOOOOOOOOOOOOO..");
		printEnterWipe("BOOOOOOM !!!!!!!!!");
		printEnterWipe("THAT CANNON\'LL KILL US!!");
		printEnterWipe("(groaning) Aahh ~ uuhg ~ .. ahhh..");
		printEnterWipe("We need a Cannon ourselves...");
		printEnterWipe("You need to gather 5 Cannons and 5 rounds of ammo for them!");

		wheelCannon(hero);
	}

	// Load Cannons
	if (allCannonsIn && !fullMagazine)
	{
		printEnterWipe("You\'ve got the Cannons..");
		printEnterWipe("..but they\'re useless without some Round-Shot.");
		printEnterWipe("Keep loading!");

		loadCannons(hero);
	}

	if (missionCompleted)
	{
		printEnterWipe("You\'ve already taken the Bastille.. but there still more to do!");
	}
	// Everything is set.. now FIRE those Guns!
	if (sneakAttackCompleted && allCannonsIn && fullMagazine && !missionCompleted)
	{
		printEnterWipe("Okay now FIRE before it\'s too late!");
		int roll = hero->rollInitiative();

		if (roll > 3)
		{
			printEnterWipe("BOOM ! - BOOM ! - BOOOM ! - BOOM ! - BA-BOOM !");
			printEnterWipe("I see a white flag! You did it! ..");
			printEnterWipe("The Commandant has surrendered to the people!");
			missionCompleted = true;

			// farewell to arms
			destroyMagazine();
			destroyArmory();
		}
		else
		{
			// If you miss the first time.. you have to reload and try again
			if (!reload)
			{
				destroyMagazine();
				fullMagazine = false;
				printEnterWipe("You\'ll have to get more Round-Ball and try again!");
				reload = true;
			}
			else
			{
				roll = hero->rollInitiative();

				if (roll > 3)
				{
					printEnterWipe("BOOM ! - BOOM ! - BOOOM ! - BOOM ! - BA-BOOM !");
					printEnterWipe("I see a white flag! You did it! ..");
					printEnterWipe("The Commandant has surrendered to the people!");
					missionCompleted = true;
				}
				else
				{
					printEnterWipe("we\'re done for..");
					printEnterWipe("GAME OVER !!");
					this->setLostGame();
				}

				// farewell to arms
				destroyMagazine();
				destroyArmory();
			}
		}

	}

}

// Displays Historical information and dialogue
void Bastille::introSequence(Character* hero)
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("The Bastille Fortress turned political prison has become a symbol of the King\'s treachery.");
	printEnterWipe("You\'ld like to raze it to the ground.. but there\'ld be no going back.");
	printEnterWipe("Are you that radical?");

	printEnterWipe("Yup. You are.");
	printEnterWipe("First thing to do.. is gather people.. shopkeepers.. vagrants..");
	printEnterWipe("Anyone that is tired of putting up with this injustice..");
	printEnterWipe("See how many will follow you..");

	int roll = hero->rollInitiative();
	cout << "\n   Looks like there are " << roll << " willing souls to follow you.";
	printEnterWipe("They have a single Cannon.. but I bet we could take \'em.");
	printEnterWipe("A few Palace Guards and maybe a Cannon.");

	introCompleted = true;
}

/************************************************************************************
Function Name: sneakAttackFirst()
Description:
	Level ACTION for Bastille Level. Character* hero attempts to either:
	1. Sneakily Escape -OR-
	2. Successfully beat 3 Royal Guards

	NaziAttack() will only return true if you beat all three.
	If you do not, you can try again if you are not dead.
	The idea is to fight all of them to get the explosives to
	blow up the Dam and WIN the game

**************************************************************************************/
bool Bastille::sneakAttackFirst(Character *hero)
{
	PalaceGuards* guard1 = new PalaceGuards();
	guard1->setName("Jean Pierre");
	guard1->getSatchel()->enqueue(myInventory->charleville());

	PalaceGuards* guard2 = new PalaceGuards();
	guard2->setName("Jean Luc");
	guard2->getSatchel()->enqueue(myInventory->charleville());

	PalaceGuards* guard3 = new PalaceGuards();
	guard3->setName("Capitan Marc");
	guard3->getSatchel()->enqueue(myInventory->sabre());

	printEnterWipe("Looks like there are only 3 of them..");
	printEnterWipe("See if you can sneak past them or not..");

	bool escaped = false;

	int roll = hero->rollInitiative();

	if (roll >= 5 || (hero->getAbility() == SNEAKY))
	{
		printEnterWipe("You managed to evade them.. now for the next step..");
		escaped = true;
	}
	else
	{
		printEnterWipe("Nope.. you\'ve been spotted. Time to fight them.");
		printEnterWipe("Better get a weapon if you have one..");
		printEnterWipe(".. anything will do");

		hero->findAnyWeapon();

		// fight guard1
		int combatResult = twoCharactersCombat(hero, "Phooom ~*", guard1, "WHIZ-pink!");

		if (combatResult == 1)
		{
			Item* trophy1 = guard1->getSatchel()->getFront();
			printEnterWipe("You earned a trophy!");
			hero->addWeaponBonus(trophy1);

			printEnterWipe("Two more of them! GET DOWN !!");
			printEnterWipe("Arret! restez en arriere ou je tirerai!\n\n   Bang! * sheeeooo-oop! *");

			// fight guard2
			combatResult = twoCharactersCombat(hero, "Phooom ~*", guard2, "WHIZ-pink!");

			if (combatResult == 1)
			{
				Item* trophy2 = guard2->getSatchel()->getFront();
				printEnterWipe("You earned a trophy!");
				hero->addWeaponBonus(trophy2);

				printEnterWipe("Fire back!");

				// fight guard3
				combatResult = twoCharactersCombat(hero, "THUMP-BANG!", guard3, "ting ~ ting ~ tang !!");

				// finally set escaped as TRUE
				if (combatResult == 1) 
				{ 
					Item* trophy3 = guard3->getSatchel()->getFront();
					printEnterWipe("You earned a trophy!");
					hero->addWeaponBonus(trophy3);

					printEnterWipe("You\'re still kickin\' .. that was close.");
					hero->postCombatStats();
					hero->health();
					escaped = true; 
				}
			}
		}

	} 

	if (hero->getStrength() < 0)
	{
		printEnterWipe("\n   The Royal Guards shot you to death!");
		printEnterWipe("GAME OVER!");
		this->setLostGame();
	}
	else { sneakAttackCompleted = true; }
	
	delete guard1; guard1 = nullptr;
	delete guard2; guard2 = nullptr;
	delete guard3; guard3 = nullptr;

	return escaped;
}

/*********************************************************************************
Function Name: wheelCannon()
Description:
	Determines if hero has the required 5 Cannons required to overtake the Bastille.

	It does this by calling iHaveThis(), and searchSatchel() to find the Cannon
	and then place them into an Item* array which is a private member of this
	class. The purpose of this is to allow the player to leave the level and
	return to fill the array.

	If the array is full, cannonCount will equal 5, and the function will return
	the member variable allCannonsIn as true

Returns:
	If allCannonsIn is true : TRUE
	Otherwise :	FALSE

**********************************************************************************/
bool Bastille::wheelCannon(Character* hero)
{
	if (cannonCount < 5)
	{
		for (int j = cannonCount; j < 5; j++)
		{
			// You must possess a Cannon in your satchel
			if (hero->iHaveThis("\n   Cannon"))
			{
					Item* bigun = hero->getSatchel()->searchSatchel("\n   Cannon");
					cout << "\n\n   <ENTER>";
					cin.get();
					wipeScreen();

					printEnterWipe("Whew! that was heavy!");
					armory[j] = bigun;
					this->cannonCount++;
			}
			else
			{
				printEnterWipe("We need another Cannon. Wheel over another one!");
				break;
			}
		}
		if (cannonCount == 5) { allCannonsIn = true; }
	}
	return allCannonsIn;
}

/*********************************************************************************
Function Name: destroyArmory()
Description:
	Deletes all Cannons placed in the armory array.
	Calls destroyLastCannon() until cannonCount is zero.

**********************************************************************************/
void Bastille::destroyArmory()
{
	while (cannonCount > 0)
	{
		destroyLastCannon();
	}
}

/*********************************************************************************
Function Name: destroyLastCannon()
Description:
	Deletes the Cannon placed in last position of armory array.

	Where is it called? :
		1. 
		2. Also called by a 'destructor' for the array.
		3. The actual destructor calls this function during cleanup.

	Decrements cannonCount member variable

Effect: Last Cannon in armory array member
		is freed and set to to nullptr

**********************************************************************************/
void Bastille::destroyLastCannon()
{
	if (cannonCount > 0)
	{
		if (armory[cannonCount - 1] != nullptr)
		{
			cannonCount--;
			delete armory[cannonCount];
			armory[cannonCount] = nullptr;
		}
	}
}

/*********************************************************************************
Function Name: loadCannons()
Description:
	Determines if hero has the required 5 round required to load all
	5 Cannons.

	It does this by calling iHaveThis(), and searchSatchel() to find the ammo
	and then place them into an Item* array which is a private member of this
	class. The purpose of this is to allow the player to leave the level and
	return to fill the array.

	If the array is full, ammoCount will equal 5, and the function will return
	the member variable fullMagazine as true

Returns:
	If fullMagazine is true : TRUE
	Otherwise :	FALSE

**********************************************************************************/
bool Bastille::loadCannons(Character* hero)
{
	if (ammoCount < 5)
	{
		for (int j = ammoCount; j < 5; j++)
		{
			// You must possess a Cannon in your satchel
			if (hero->iHaveThis("\n   1 Round Shot"))
			{
				Item* ball = hero->getSatchel()->searchSatchel("\n   1 Round Shot");
				cout << "\n\n   <ENTER>";
				cin.get();
				wipeScreen();

				printEnterWipe("SHOO CLUNK!");
				magazine[j] = ball;
				this->ammoCount++;

			}
			else
			{
				printEnterWipe("You still need more Round-Shot!");
				break;
			}
		}
		if (ammoCount == 5) { fullMagazine = true; }
	}
	return fullMagazine;
}

/*********************************************************************************
Function Name: destroyMagazine()
Description:
	Deletes all Rounds placed in the magazine array.
	Calls destroyLastRound() until ammoCount is zero.

**********************************************************************************/
void Bastille::destroyMagazine()
{
	while (ammoCount > 0)
	{
		destroyLastRound();
	}
}

/*********************************************************************************
Function Name: destroyLastRound()
Description:
	Deletes the Round placed in last position of magazine array.

	Where is it called? :
		1.
		2. Also called by a 'destructor' for the array.
		3. The actual destructor calls this function during cleanup.

	Decrements ammoCount member variable

Effect: Last Round in magazine array member
		is freed and set to to nullptr

**********************************************************************************/
void Bastille::destroyLastRound()
{
	if (ammoCount > 0)
	{
		if (magazine[ammoCount - 1] != nullptr)
		{
			ammoCount--;
			delete magazine[ammoCount];
			magazine[ammoCount] = nullptr;
		}
	}
}

//***************************** E O F ********************************
