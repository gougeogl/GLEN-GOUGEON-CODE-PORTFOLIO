/*****************************************************************************
 * File Name: Algiers_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Algiers Class (SUB) Level

	SEE: Action() below for Level description

 *****************************************************************************/
#include"Algiers_Level.hpp"

// default Constructor
Algiers::Algiers()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->bugs());

	// Estates General
	this->setLocationName("\n   Algiers, North Africa.\n");
	this->setDate("July", 31, 1944);
	this->setDescription("\n   U.S. Army Ranger Special Operations. Jedburgh Unit."
		"\n   Parachute drop to Lazare Dept., Rhone Valley-France during the Second World War.\n");

	string explanation = "\n   Connect with French-Partisan resistence groups,"
		"\n   ahead of 7th Army invasion of the French Riviera for training and armament."
		"\n   Assist trainees in demolition of railroad-bridges and tunnels, roadblocks,"
		"\n   roadblocks, and other targets of opportunity as available."
		"\n   Intelligence gathering encouraged as appropriate.";

	this->setSignificance(explanation);

	introRead = false;
}

// Destructor
Algiers::~Algiers() {}

/**********************************************************************************
Function Name: search()
Description:
		Performs action SEARCH option on a Level

		STEPS:
			1. Prompt yes or no input to search
			2. Validates step 1 input
			3. If yes:
				AND itemStack is empty:
					enters NaziAttack() function sequence
				AND itemStack is NOT empty:

					a) pop item off 'itemStack' (BASE class member of LevelClass)
					b) print item popped
					c) ask if user wants it y/Y/n/N only (validates)
					d) If doesn't want:
						push onto 'droppedItems' Stack (BASE class member of LevelClass)
					e) If wants:
						enqueue in Jones' 'satchel' (queue)

			4. If no: exit SEARCH

			NOTE* 
				similar structure once you fight the Nazis.
				the foughtItOut flag will be set & .. 
				YOU'LL GET THE NEEDED EXPLOSIVES

Parameters: Character* Jones (Harrison Ford.. YOU in game)

Effect:	
		1. Battle against 3 Nazis
			a) If you win, you get the explosives needed
			b) If not you LOSE GAME.

		If you already fought the Nazis..you can still fight them again
		and get more explosives.

*************************************************************************************/
void Algiers::search(Character* hero)
{
	cout << "\n   Search for weapons, munitions, communications?"
		<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("It is tactically expedient to decide soon, Captain.");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("We really need a supply drop.");
		}
		else
		{
			Item *temp = nullptr;
			temp = itemStack->pop();

			wipeScreen();

			// displays what was found w/ custom dialogue
			this->printNewItemFound(temp, "Sir I discovered this..");

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

/***********************************************************************
PURE VIRTUAL Function Name: action()
Description:

	Algiers, Algeria, North Africa during WWII was the staging point
	for covert missions into Southern France. In this level, you are
	part of a Jedbourgh 3-man special operations team sent to train
	French Partisans. During the process you fight a group of 3 Nazi
	Wehrmacht troopers which, if you live, will give you explosives.
	Those explosives are used in the level above RR_Bridge_Level to
	WIN the GAME!

************************************************************************/
void Algiers::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { introAlgiers(); }
	if(introRead)
	{
		printEnterWipe("You\'ve dropped in the Lazare Dept., a forested mountain region, to escape detection.");
		printEnterWipe("Link up with the French Partisans to pick a target.");

		int roll = hero->rollInitiative();
		if (roll > 3)
		{
			printEnterWipe("You\'ve met up with the Guallists faction. Good.");
		}
		else
		{
			printEnterWipe("You find some of the Partisans... but they\'re Communists.");
			printEnterWipe("Watch yourself.");
		}

		printEnterWipe("Lolly-gaggin\'s over ..");
		printEnterWipe("Company\'s coming.");

		bool foughtItOut = NaziAttack(hero); // THIS IS WHERE WE BATTLE

		if (foughtItOut)
		{
			Item *temp = myInventory->exploders();
			wipeScreen();

			// displays what was found w/ custom dialogue
			this->printNewItemFound(temp, "On the way back I found");

			// Verify keep or drop new item
			char response = checkYesNo("Decisions..decisions.. do I want this?");

			if (toupper(response) == 'N')
			{
				wipeScreen();
				printThenEnter("hmm..   I don't want that.");

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
			// required formatting
			cin.ignore();
			wipeScreen();
		}
	}

}

/************************************************************************************
Function Name: introAlgiers()
Description:

**************************************************************************************/
void Algiers::introAlgiers()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   MISSION OBJECTIVE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("Following the events of the D-Day invasion..");
	printEnterWipe("Army Special Operations groups parachuted into still occupied France..");
	printEnterWipe("behind enemy lines.");
	printEnterWipe("Jedburgh groups consisting of a U.S. Army Officer, a French Officer..");
	printEnterWipe("..and a radio operator, were sent to train French Partisans resistence fighters");
	printEnterWipe("and generally harass the Germans as much as possible.");

	introRead = true;
}

/************************************************************************************
Function Name: NaziAttack() 
Description:
	Level mission for Hills Class. Character* Jones attempts to either:
	1. Roll a chance cube of 5+ and escape -OR-
	2. Successfully beat 3 Nazis

	NaziAttack() will only return true if you beat all three.
	If you do not, you can try again if you are not dead.
	The idea is to fight all of them to get the explosives to
	blow up the Dam and WIN the game

**************************************************************************************/
bool Algiers::NaziAttack(Character *hero)
{
	Nazi *hans = new Nazi;
	hans->setName("Hans");
	Nazi *franz = new Nazi;
	franz->setName("Franz");
	Nazi *rupert = new Nazi;
	rupert->setName("Rupert");

	printEnterWipe("It\'s a Nazi patrol coming by..");

	bool escaped = false;
	bool luckyRoll = false;

	// Check if you escape first
	if (hero->rollInitiative() > 4) 
	{
		printEnterWipe("Looks like they didn\'t see you.");
		hero->setAbility(SNEAKY);
		printEnterWipe("You are SNEAKY now.");
		luckyRoll = true;
	}
	else // Didn't escape. Look for Revolver first, then any other possible weapon from satchel
	{
		printEnterWipe("Better get your Thompson if you have it..");
		printEnterWipe(".. but anything will do");

		hero->addNamedWeaponOrAnyWeaponBonus("\n   Thompson sub-machine-gun");
	}

	if (!luckyRoll)
	{
		int combatResult = 0;

		// 'Sound' Effects (onomatopoeia)
		string K98 = "\n   Bang! * chi-cha-ching Bang! *";
		string MP38 = "\n   bip-bip-bip-bip-bip-bip!!! -bip ~CLICK~";
		string M1911 = "\n   BOOM! BOOM!";
		string Webley = "\n   BANG ~~ .  whiz";
		string PPK = "\n   Pa-pow ! ~ Pa-pow-pow!";
		string MG42 = "\n   ~ -rat-tat-ta ~";
		string german = "\n   Es ist Jones schieBen ihn Sie Idioten !";

		printEnterWipe("There are three Wehrmacht creeps coming at you right now!");
		printEnterWipe("Looks like this first guy wants a piece of you...");

		// fight hans
		combatResult = twoCharactersCombat(hero, Webley, hans, MG42);

		if (combatResult == 1)
		{
			printEnterWipe("Two more of them! Duck !!");
			printEnterWipe("SchieBen ihn Sie Idioten !\n\n   Bang! * chi-cha-ching Bang! *");

			// fight franz
			combatResult = twoCharactersCombat(hero, M1911, franz, PPK);

			if (combatResult == 1)
			{
				printEnterWipe("Fire back!");

				// fight rupert
				combatResult = twoCharactersCombat(hero, Webley, rupert, MP38);

				// finally set escaped as TRUE
				if (combatResult == 1) { escaped = true; }
			}
		}

	}

	if (escaped) { printEnterWipe("You\'re still kickin\' .. that was close."); }

	// printing
	if (!luckyRoll) { hero->postCombatStats(); }

	if (hero->getStrength() < 0)
	{
		printEnterWipe("\n   The Nazis shot you to death!");
		printEnterWipe("GAME OVER!");
		this->setLostGame();
	}

	// heal some
	if(escaped){ hero->health(); }
	
	delete hans; hans = nullptr;
	delete franz; franz = nullptr;
	delete rupert; rupert = nullptr;

	return escaped;
}

//***************************** E O F ********************************
