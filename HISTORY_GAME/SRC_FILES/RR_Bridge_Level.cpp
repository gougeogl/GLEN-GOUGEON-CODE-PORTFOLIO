/*****************************************************************************
 * File Name: RR_Bridge_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for RR_Bridge_Level Class (SUB) Level

	SEE: RR_Bridge_Level.hpp for detailed description of this Level.

 *****************************************************************************/
#include"RR_Bridge_Level.hpp"

// default Constructor
Bridge::Bridge()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->hamburger());
	itemStack->push(myInventory->bugs());

	// Rhone Valley, France.
	this->setLocationName("\n   Rhone Valley, France.\n");
	this->setDate("August", 12, 1944);
	this->setDescription("\n   Team Packard assists French Partisans to harass the Germans.\n");

	string explanation = "\n   Operation Dragoon August 15, 1944 sought the allied invasion of southeastern France."
		"\n   On August 19, U.S. Army Lt. General Lt. Gen. Alexander M. Patch broke away from the Anvil Beach-Head."
		"\n   The actions of special operations units and french resistance fighters, prepared the way for the"
		"\n   liberation of southeastern France during the Second World War, by reaking havoc ahead of the allied landings.";

	this->setSignificance(explanation);

	introRead = false;
}

// Destructor
Bridge::~Bridge() {}

/**********************************************************************************
Function Name: search()
Description:
		Performs action SEARCH option on a Level

		STEPS:
			1. Prompt yes or no input to search
			2. Validates step 1 input
			3. If yes:
				AND itemStack is empty:
					enters covertMission() function sequence
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
		1. Attempt to blow up the Bridge with the explosives you have
			a) If you succeed you WIN THE GAME
			b) If not you LOSE GAME.

*************************************************************************************/
void Bridge::search(Character* hero)
{
	cout << "\n   You need a specific item..want to look nearby?"
		<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("Need to search for it or not?");

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
			this->printNewItemFound(temp, "Capitan, I discovered this..");

			// validate
			char response = checkYesNo("Monsieur? .. your orders?");

			// leaving behind at this location
			if (toupper(response) == 'N')
			{
				printThenEnter("I\'ll come back for it.");

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
	Attempt to blow up a RR Bridge and win the game. If you don't have the 
	suitcase of explosives, you'll have to get them and return. Once you attempt
	to blow up the bridge, there is no going back. 

Effect:
	If successful : WIN game
	If failure : LOSE game

**********************************************************************************/
void Bridge::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { bridgeIntroDialogue(); }
	covertMission(hero);

}

// Introductory history and other dialogue about this level
void Bridge::bridgeIntroDialogue()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("You and the other Jedburghs have spent several weeks in training the French Partisans.");
	printEnterWipe("The different teams are even in competition against one-another.");
	printEnterWipe("This time.. it\'s a railroad bridge.");
	printEnterWipe("Knock it out.. and it\'ll mess with Jerry\'s supply lines for weeks!");

	introRead = true;
}

/************************************************************
Function Name: covertMission()
Description:
	RR_Bridge_Level mission. Initiates game sequence where
	you attempt to use explosives to blow up a RR Bridge.

	STEPS:
		1. call covertMissionCutScene()
		2. explain mission objective in cut scene
		3. determine choice yes or no
		4. validate choice

		5. If accept:
			validate hero has explosives:

				a) If hero doesn't:
					prompt he needs to find them
				b) If hero has them:
					print them
					call plantBridgeExplosives()
					call stripWires()

		6. If accept but:
				doesn't have them (lie/mistake)
					prompt to go get them and come back

*************************************************************/
void Bridge::covertMission(Character *hero)
{
	bool missionSuccess = false;

	// dialogue for mission
	covertMissionCutScene(hero);

	// validate
	char choice = checkYesNo("Well did you bring them with you or not?");

	if (toupper(choice) == 'N')
	{
		wipeScreen();
		cin.ignore();
		cout << "\n   Then you'll need those explosives."
			<< "\n   Your radio operator says they made a supply drop back in the Lazare District."
			<< "\n\n   You better find it before the Nazis do.";
		printEnterWipe("How else would you expect to wake up?");
	}
	else if (toupper(choice) == 'Y' && !hero->getSatchel()->isEmpty())
	{
		// check if you have the explosives
		if (hero->iHaveThis("\n   Suitcase of Explosives"))
		{
			wipeScreen();
			cin.ignore();

			Item* bridgePilings = hero->getSatchel()->searchSatchel("\n   Suitcase of Explosives");
			printEnterWipe(" ");

			// deletes Item* baseOfDam
			plantBridgeExplosives(bridgePilings);

			// final process of mission
			missionSuccess = stripWires();

			// set WIN
			if (missionSuccess) { this->setWonGame(); }
			// set LOSE
			else { this->setLostGame(); }
		}
		// you don't have them
		else
		{
			wipeScreen();
			cin.ignore();
			cout << "\n   You need those explosives!";
			printEnterWipe("don't come back until you\'ve got \'em!");
		}
	}
	else
	{
		wipeScreen();
		cin.ignore();
		cout << "\n   Nice try. You need that suitcase."
			<< "\n   Your radio operator says they made a supply drop back in the Lazare District."
			<< "\n   You better find it before the Nazis do.";
		printEnterWipe("How else would you expect to wake up?");
	}

}

// Prints some dialogue pertaining to mission
void Bridge::covertMissionCutScene(Character* hero)
{
	printEnterWipe("You've just arrived near your mission objective: The Bridge."
		"\n   Your French counterpart begins talking:");

	// dialogue??
	if (hero->getAbility() != FRENCH)
	{
		printEnterWipe("Il y a un train qui arrive ... il vaut mieux se depecher!");
		printEnterWipe("C\'mon! English!");
		printEnterWipe("Whatever.");
	}
	else
	{
		printEnterWipe("There\'s a train coming... better hurry!");
	}

	printEnterWipe("Locate the explosives to blow up the bridge, and escape!!");

	cout << "\n   Do you have them on you?"
		<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";
}

/***********************************************************
Function Name: plantBridgeExplosives()
Description:
	Helper to covertMission() function.
	Deletes Item* input explosives as part of the game.
	Prints two statements before and after deletion.

************************************************************/
void Bridge::plantBridgeExplosives(Item* explosives)
{
	printEnterWipe("Now disperse those around the pilings like I showed \'ya..");

	delete explosives; 
	explosives = nullptr;

	printEnterWipe("Got the explosives set? Huh? Great.");
}

/***********************************************************
Function Name: stripWires()
Description:
	Helper to covertMission() function.

	STEPS:
		1.	roll a 1d12 twelve-sided die
		2.	if output of step 1 was > 5
			then roll again and call blowBridge()
		3.	otherwise keep 're-wiring'
		4.	evaluate the return value of blowBridge()
		5.	set game WON/LOST based on step 4 result

************************************************************/
bool Bridge::stripWires()
{
	bool outcome = false;

	Die myRoller;
	int result = 0;

	printEnterWipe("Now try to wire the leads.");

	do
	{
		printEnterWipe("Start stripping wires.");

		result = myRoller.twelve();

		cout << "\n   PLASTIC EXPLOSIVE WIRED ! ~"
			<< "\n   wiring quality:" << result << "\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		if (result > 5)
		{
			printEnterWipe("Now that it's wired...be careful."
				"\n   if we screw this up, we won't get another chance.");

			result = myRoller.twelve();

			cout << "\n   DETONATOR SET ! ~ click ~ "
				<< "\n   eta min. safe distance: " << result << "\n\n   <ENTER>";
			cin.get();
			wipeScreen();

			// outcome later determines WIN/LOSE game
			outcome = blowBridge(result);
			break;

		}
		else
		{
			printEnterWipe("Not like that! try again.");
		}

	} while (result < 6);

	return outcome;
}

/***********************************************************
Function Name: blowBridge()
Description:
	Helper to stripWires() function .. which is a helper
	to covertMission()

	STEPS:
		1.	evaluate int input
		2.	If >= 8 you succeed
			and call successEffects()
		3.	Otherwise you fail and call
			failEffects()
		4.	If success, the result is TRUE
		5.	return result

************************************************************/
bool Bridge::blowBridge(int input)
{
	bool result = false;

	if (input >= 8)
	{
		successEffects();

		// YOU WON
		result = true;

		wipeScreen();
		printEnterWipe("YOU DID IT! ! !!");
		printEnterWipe("TO WAKE UP PRESS");

	}
	// You Blew yourself up!
	else { failEffects(); }

	return result;
}

// Prints 'special effects' if successfully blow up the Bridge
void Bridge::successEffects()
{
	printThenEnter("\n   Blow that Bridge! NOW !!");
	for (int i = 0; i < 15; i++)
	{
		wipeScreen();
		cout << "\n   POP-POP-POP! BAM!";
	}
	for (int i = 0; i < 20; i++)
	{
		wipeScreen();
		cout << "\n ~*^^*******#  ##  #***^^****^^^*~~~~~~~"
			<< "\n   KA-BOOOM!! BAM!"
			<< "\n ~*^^*******#  ##  #***^^****^^^*~~~~~~~";
	}
	for (int i = 0; i < 15; i++)
	{
		wipeScreen();
		cout << "\n ~*^^*******#  ##  #***^^****^^^*~~~~~~~"
			<< "\n   KA-BOOOM!! BOOM! BOOM! ...BOOM!"
			<< "\n ~*^^*******#  ##  #***^^****^^^*~~~~~~~";
	}
	wipeScreen();
}

// Prints alternate 'special effects' if hero gets blown up
void Bridge::failEffects()
{
	for (int i = 0; i < 20; i++)
	{
		wipeScreen();
		cout << "\n ~*^^*******#  ##  #***^^****^^^*~~~~~~~"
			<< "\n   BA-BOOOOOOOOOOOOOOM!! ! ~ ~~ ~ .. ..."
			<< "\n ~*^^*******#  ##  #***^^****^^^*~~~~~~~";
	}
	wipeScreen();
	printEnterWipe("MISSION FAILED:");
	printEnterWipe("You just blew Yourself up!");
	printEnterWipe("GAME OVER !!");

}

//***************************** E O F ********************************
