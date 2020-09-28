/*****************************************************************************
 * File Name: EstatesGeneral_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Estates General (Level) Class.

	SEE: action() below for Level description.

 *****************************************************************************/
#include"EstatesGeneral_Level.hpp"

// Default Constructor
EstatesGeneral::EstatesGeneral()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->royalSummons());
	itemStack->push(myInventory->wineGlass());
	itemStack->push(myInventory->royalSummons());
	itemStack->push(myInventory->royalSummons());
	itemStack->push(myInventory->Artillery());
	itemStack->push(myInventory->croutons());
	itemStack->push(myInventory->concordance());
	itemStack->push(myInventory->charleville());

	// Estates General
	this->setLocationName("\n   Estates General : Versailles, France.\n");
	this->setDate("May", 5, 1789);
	this->setDescription("\n   Estates General meets to discuss political reforms.\n");

	string explanation = "\n   The convening of the Estates General laid the foundation"
		" for the French Revolution of 1789.";

	this->setSignificance(explanation);

	introRead = false;
	middleRead = false;
	summonsTally = 0;
	allSummonsIn = false;
	invites = PRIEST;
	convincedOthers = false;
	startedInviting = false;
	firstContactComplete = false;
}

// Destructor
EstatesGeneral::~EstatesGeneral() {}

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
void EstatesGeneral::search(Character* hero)
{
	cout << "\n   It\'s aweful busy here.. Should I bother looking for something here?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("Why can't I make up my mind?..a simple");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("There\'s no way I won\'t get caught..forget it.");
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
	Executes ACTION cmd option for this level. Player must deliver summons
	to members of 3 different estates to complete this mission. A call to 
	firstContact() acts as a way to teach the user the format for talking to 
	other game Characters when prompted. The process of delivering said summons
	then takes place.

**********************************************************************************/
void EstatesGeneral::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { estatesIntroDialogue(); }

	// FIRST CONTACT (talking)
	if (!firstContactComplete) { firstContact(hero); }

	if (this->isGameLost() == false)
	{
		learnFrenchFromConcordance(hero);

		// MISSION
		if (!convincedOthers) { callEstatesGeneral(hero); }
		if (convincedOthers) { endingDialogue(); }

	}

}

// Opening dialogue for the ACTION command option.
void EstatesGeneral::estatesIntroDialogue()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("You\'ve landed in France.. and you thought you were near Paris but..");
	printEnterWipe(".. now you appear before a simply massive complex. Definately still French.");
	printEnterWipe("There are divided gardens, ornate pavers, and hundres of windows.");
	printEnterWipe("This can\'t be the palace of Versailles? can it??");
	printEnterWipe("Your French is pretty rusty but you manage the words.. GATHER, KING, and ESTATE.");
	printEnterWipe("From the dress of the people you realize you\'ve either landed at a re-enactment..");
	printEnterWipe("or you\'re in the 18th century French countryside.");
	printEnterWipe("As you draw closer to the palace, you observe a bit of cliquish behavior..");
	printEnterWipe(".. based on dress? mannerisms?? and alot of lace?");

	introRead = true;
}

/*********************************************************************************
Function Name: firstContact()

Helpers:
	chooseResponse()
	speechConsequences()
	fightTwoGuards()

Parameters:
	Character* newbie. The player's main Character. The Hero.

Description:
	Introduces user to 'talking' to another game character by presenting a
	selection of options to say. Calls chooseResponse() to do so.

Consequences :
	(a positive/negative return value from chooseResponse() function)

	[+] A good response. Will print a happy statement.
	[-] A poor response, will call fightTwoGuards() function.

**********************************************************************************/
void EstatesGeneral::firstContact(Character* newbie)
{
	printEnterWipe("Someone is staring at you.");
	printEnterWipe("Attempt to say ..ah.. something so as not to look suspicious.");

	// The wrong response will sap some of your strength
	string sayings[3] = { "Sacre bleu", "Bonjoir", "Bon Appetite" };
	int score = chooseResponse(sayings, 3, 2);

	// how you respond has consequences to your health!
	speechConsequences(newbie, score);
	wipeScreen();
	cin.ignore();

	if (score > 0) { printEnterWipe("Not bad.. but try to avoid interactions for now."); }
	else { fightTwoGuards(newbie); }

	firstContactComplete = true;
}

/*********************************************************************************
Function Name: fightTwoGuards()

Helpers:
	addWeaponBonus() [Character_Class]
	twoCharactersCombat() [#included]
	postCombatStats() [Character_Class]

Parameters:
	Character* newb. The player's main Character. The Hero.

Description:
	Performs combat on two Guards instantiated by this function.

Consequences :
	If newb succeeds, he earns their weapons as trophies.
	If newb fails: Death [LOSE GAME]

**********************************************************************************/
void EstatesGeneral::fightTwoGuards(Character* newb)
{
	printEnterWipe("That got you into trouble.. watch out!");
	printEnterWipe("The Palace Guard you were just talking to..");
	printEnterWipe(".. are on the defensive..");
	printEnterWipe("You\'ll have to fight the two of them.");

	// setup guards
	PalaceGuards* g1 = new PalaceGuards();
	g1->setName("Breve");
	cout << "\n   Looks like " << g1->getName() << " has a pike aimed at your gut!"
		<< "\n\n   <ENTER>";
	cin.get();
	wipeScreen();

	// give Breve a wpns bonus
	g1->addWeaponBonus(myInventory->pike());

	PalaceGuards* g2 = new PalaceGuards();
	g2->setName("Dijon");
	cout << "\n   .. and so does " << g2->getName() << "!"
		<< "\n\n   <ENTER>";
	cin.get();
	wipeScreen();

	// give Dijon a wpns bonus
	g2->addWeaponBonus(myInventory->pike());


	// give yourself a weapons bonus if you have one
	printEnterWipe("You better have a weapon too..");
	newb->findAnyWeapon();

	int combatStatus = twoCharactersCombat(newb, "BLOCK-shuffle!", g1, "SWASH-SWISH-FOO!");

	if (combatStatus == 1)
	{
		printEnterWipe("You\'re still alive for now.. but that was DUMB!");
		newb->setAbility(DUMB);

		combatStatus = twoCharactersCombat(newb, "roll-JUMP-block!", g2, "SWASH-SWISH-CHING!");

		if (combatStatus == 1)
		{
			printEnterWipe("HE MISSED! you ARE tough now.");
			newb->setAbility(TOUGH);

			printEnterWipe("They had their fun.. and you are still alive but..");
			printEnterWipe("..remember the French take language SERIOUSLY!");
		}

	}

	g1->postCombatStats();
	g2->postCombatStats();
	newb->postCombatStats();

	if (newb->getStrength() < 0)
	{
		printEnterWipe("You were skewered with a pike!");
		printEnterWipe("GAME OVER !!");
		this->setLostGame();
	}
	else
	{
		// heal some
		newb->health();
	}

	delete g1; g1 = nullptr;
	delete g2; g2 = nullptr;

}

/*********************************************************************************
Function Name: callEstatesGeneral()

Description:
	Performs ACTION mission for this level. The player must deliver 3 royal
	summons to a member of each estate. Summons must be collected as part of
	a search on this and other levels if available.

Returns:
	If all 3 summons were delivered: TRUE
	otherwise: FALSE

**********************************************************************************/
bool EstatesGeneral::callEstatesGeneral(Character* hero)
{
	// DIALOGUE
	if(!middleRead){ estatesMiddleDialogue(); }

	if (!attemptDeliveryToAllEstates(hero)) { convincedOthers = false; }
	// if you convinced all 3 estates success
	else 
	{ 
		burnAllSummons();
		convincedOthers = true; 
	}
	return convincedOthers;
}

// Dialogue found in the middle of the estatesGeneral() function.
void EstatesGeneral::estatesMiddleDialogue()
{
	printEnterWipe("Suddenly you notice that you are dressed in the garb of a royal messenger..");
	printEnterWipe("and there is something in your hand. Some sort of announcement perhaps.");
	printEnterWipe("If this IS Versailles, and your suspicions are correct..");
	printEnterWipe("You should be summoning people to convene for the Estates General");
	printEnterWipe("You\'ll need the official documents (all 3) for this to work..");
	printEnterWipe("..and mastery of French.");

	middleRead = true;
}


/*********************************************************************************
Function Name: attemptDeliveryToAllEstates()
Description:
	Determines of servant has the required 3 Royal Summons required to begin the
	Estates General.

	It does this by calling helper functions:
		iHaveThis() [Character Class helper]
		attemptDeliveryToAllEstates()
		stashInvite()

	to find the summons and then place them into an Item* array which is a
	private member of this class. The purpose of this is to allow the player
	to leave the level and return to fill the array.

	If the array is full, summonsTally will equal 3, and the function will return
	the member variable allSummonsIn as true

Returns:
	If allSummonsIn is true : TRUE
	Otherwise :	FALSE

**********************************************************************************/
bool EstatesGeneral::attemptDeliveryToAllEstates(Character* servant)
{
	if (summonsTally < 3)
	{
		for (int i = summonsTally; i < 3; i++)
		{
			// You must possess a summons in your satchel
			if (servant->iHaveThis("\n   Royal Summons"))
			{
				bool succeeded = false;

				// see which estate needs and invitation
				switch (invites)
				{
					case PRIEST:	succeeded = convinceAndDeliverDelegate(servant, PRIEST);
									break;

					case NOBLE:		succeeded = convinceAndDeliverDelegate(servant, NOBLE);
									break;

					case COMMONER:	succeeded = convinceAndDeliverDelegate(servant, COMMONER);
									break;
				}
				if (succeeded)
				{
					// places summons into boxOfSummons array
					stashInvite(servant);
				}
				else
				{
					printEnterWipe("Not very convincing..");

					int triesLeft = (2 - i);
					if (triesLeft > 0)
					{
						cout << "\n   You have " << triesLeft << " more tries.";
						printEnterWipe(".. if you fail again, you can come back later.");
					}
				}

			}
			else
			{
				printEnterWipe("You are low on Royal Summons. Go get some more.");
				break;
			}
		}
		if (summonsTally == 3) { allSummonsIn = true; }
	}
	return allSummonsIn;
}

/***********************************************************************************************
Helper Function Name: convinceAndDeliverDelegate()

Parameters:
	Character* convicer. THIS Character.
	enum DelegatType delegate. The enum associated with PRIEST, NOBLE, & COMMONER

Description:
	This is a wrapper with a switch (by enum DelegateType) that calls helper
	talkToDelegate() which calls chooseResponse(). Altogether it simplifies
	the conditional calls of the attemptDeliveryToAllEstates() switch cases.

Returns:
	if successful: TRUE
	otherwise : FALSE

************************************************************************************************/
bool EstatesGeneral::convinceAndDeliverDelegate(Character* convincer, enum DelegateType delegate)
{
	bool succeeded = false;

	Priest* priest = nullptr;
	NobleSword* noble = nullptr;
	Bourgeoisie* bourg = nullptr;
	Character* person = nullptr;

	cout << "\n   You just met:\n\n";
	switch (delegate)
	{
		case PRIEST:	priest = new Priest(); 
						priest->printStats();
						person = priest;
						break;

		case NOBLE:		noble = new NobleSword(); 
						noble->printStats();
						person = noble;
						break;

		case COMMONER: 	bourg = new Bourgeoisie(); 
						bourg->printStats();
						person = bourg;
						break;
	}
	
	printThenEnter("Now try to convince him.");
	succeeded = talkToDelegate(convincer, person);

	// FREE MEMORY
	switch (delegate)
	{
		case PRIEST:	if(succeeded) {invites = NOBLE;}
						delete priest; priest = nullptr;
						break;

		case NOBLE:		if (succeeded) { invites = COMMONER; }
						delete noble; noble = nullptr;
						break;

		case COMMONER: 	delete bourg; bourg = nullptr;
						break;
	}
	return succeeded;
}

/*************************************************************************
Function Name: talkToDelegate()

Parameters:
	Character* talker. THIS Character.
	Character* deleget. The delegate to talk to (convince)

Description:
	Wrapper that calls chooseResponse based on talker's Ability. 
	If talker has FRENCH as an ability, the options will be in English,
	and in French otherwise (to mimick being able to translate). Saying
	the wrong thing will affect talker's strength.

Effect:
	Talker's strength affected according what was choosen as the correct
	thing to say.

**************************************************************************/
bool EstatesGeneral::talkToDelegate(Character* talker, Character* delegate)
{
	bool convinced = false;
	int score = 0;

	string frenchPersuasions[3] = {
	"Si vous ne nous rencontrez pas, nous nous rencontrerons quand même.",
	"Ta mère a le visage d'un singe!",
	"Il y a une réunion des États généraux. Sa majesté demande votre présence."
	};

	string englishPersuasions[3] = {
		"If you don't meet us, we'll meet anyway.",
		"Your mother has the face of a monkey!",
		"There is a meeting of the Estates General. His majesty requests your presence."
	};

	if (talker->getAbility() == FRENCH)
	{
		score = chooseResponse(englishPersuasions, 3, 3);
	}
	else
	{
		score = chooseResponse(frenchPersuasions, 3, 3);
	}

	// how you respond has consequences to your health!
	speechConsequences(talker, score);

	if (score < 0)
	{
		wipeScreen();
		cin.ignore();
		printEnterWipe("That didn\'t go well.. them\'s fightin\' words!");
		twoCharactersCombat(talker, "WHAM-POW", delegate, "WOP-POOM");
	}
	else
	{
		convinced = true;
	}

	return convinced;
}


/*********************************************************************************
Helper Function Name: stashInvite()
Description:
	If messenger has a Royal Summons in their satchel(queue) then it is placed
	into the boxOfSummons[] array as part of gameplay for the Estate General
	Level. The boxOfSummons[] will be freed when mission completed.

**********************************************************************************/
void EstatesGeneral::stashInvite(Character* messenger)
{
	wipeScreen();
	Item* scroll = messenger->getSatchel()->searchSatchel("\n   Royal Summons");
	cout << "\n\n   <ENTER>";
	cin.ignore();
	cin.get();
	wipeScreen();

	printEnterWipe("Voila. Royal Summons delivered");
	boxOfSummons[summonsTally] = nullptr;
	boxOfSummons[summonsTally] = scroll;
	this->summonsTally++;

}

/*********************************************************************************
Function Name: burnAllSummons()
Description:
	Deletes all Royal Summons placed in the boxOfSummons array.
	Calls burnLastSummons() until summonsTally is zero.

**********************************************************************************/
void EstatesGeneral::burnAllSummons()
{
	while (summonsTally > 0)
	{
		burnLastSummons();
	}
}

/*********************************************************************************
Function Name: burnLastSummons()
Description:
	Deletes the Royal Summons placed in last position of boxOfSummons array.

	Where is it called? :
		1. This is called in case condition 5, followed by an initiative roll of 4+
		2. Also called by a 'destructor' for the array.
		3. The actual destructor calls this function during cleanup.

	Decrements summonsTally member variable

Effect: Last Royal Summons in boxOfSummons array member
		is freed and set to to nullptr

**********************************************************************************/
void EstatesGeneral::burnLastSummons()
{
	if (summonsTally > 0)
	{
		if (boxOfSummons[summonsTally - 1] != nullptr)
		{
			summonsTally--;
			delete boxOfSummons[summonsTally];
			boxOfSummons[summonsTally] = nullptr;
		}
	}
}

// Dialogue found at the end of the ACTION command option for this level.
void EstatesGeneral::endingDialogue()
{
	printEnterWipe("The events following the invitation to attend quickly ramped up..");
	printEnterWipe("..into a revolution, culminating in the creation of the National Assembly..");
	printEnterWipe("..and the creation of a Constitution.");
	printEnterWipe("Here is the timeline..");
	printEnterWipe("June 10, 1789. The Third Estate invites the other estates to join them as a unified body.");
	printEnterWipe("June 17, 1789. Declaration of the \'National Assembly\' by members of the Third Estate.");
	printEnterWipe("June 20, 1789. The Tennis Court Oath. The Third Estate being locked out of their customary room"
		"\n   at the Versailles palace, meet in a nearby tennis court. They vow not to leave until"
		"\n   the National Assembly creates a Constitution limiting the King\'s power");
	printEnterWipe("June 27, 1789. King Louis XVI attempts to disband the Assembly but fails.");

}

//***************************** E O F ********************************
