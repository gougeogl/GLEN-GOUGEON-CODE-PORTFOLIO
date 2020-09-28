/*****************************************************************************
 * File Name: OctoberDays_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Function Definitions for October Days (Level) Class.

	SEE: OctoberDays_Level.hpp for full Level description.

 *****************************************************************************/
#include"OctoberDays_Level.hpp"

// Default Constructor
OctoberDays::OctoberDays()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->fedora());
	itemStack->push(myInventory->shotgun());
	itemStack->push(myInventory->hamburger());

	// Apocalypse Now
	this->setLocationName("\n   The October Days : Versailles, France.\n");
	this->setDate("October", 5, 1789);
	this->setDescription("\n   Parisian housewives march 12 miles to Versaille to protest a lack of bread.\n");

	string explanation = "\n   Twenty thousand Parisian guards also travelled to Versailles in support"
		"\n   of the housewives. The move essentially forced the King to return to Paris, where he signed"
		"\n   the \'Declaration of the Rights of Man and of the Citizen\', and approved the August decrees."
		"\n   It was the final act in removing the last remnants of the old regime.";

	this->setSignificance(explanation);

	introRead = false;
	convincedRoyalsCount = 0;
	missionAccomplished = false;
}

// Destructor
OctoberDays::~OctoberDays() {}

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
void OctoberDays::search(Character* hero)
{
	cout << "\n   Want to look for bread.. or anything really?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("Why can't I make up my mind?..a simple");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("Right. Food shortages. Nothing here.");
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

/**********************************************************
(Pure Virtual) Function: action()

Parameters:
	Character* hero. The user's player.

Description:
	Handles the ACTION menu command for THIS level.
	This mission has you take part in demanding the King
	and Queen sign the August Decrees and the Declaration
	of Rights. This is abstracted by having them 'battle'
	[argue]. The twoCharacterCombat() function is at the 
	heart of this. 

	When convincedRoyalsCount >=2, the mission is a success
	and hero gains the Royal Seal. This combined with 2
	other documents will help you WIN the game!

***********************************************************/
void OctoberDays::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { octDaysIntroDialogue(); }

	if (!missionAccomplished)
	{
		// Validate begin mission
		cout << "\n   Want to go too ? (it WILL sap your strength)?"
			<< "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

		char response = checkYesNo("C\'mon! where\'s your chivalry? they might be in distress?");

		if (toupper(response) == 'Y')
		{
			// The trip reduces your strength in HALF
			sapStrength(hero);

			// CHALLENGE SEQUENCE 1
			talkToWoman(hero);

			if (this->isGameLost() == false)
			{
				// CHALLENGE SEQUENCE 2
				talkToGuard(hero);

				if (this->isGameLost() == false)
				{
					// DIALOGUE
					printEnterWipe("Introductions aside.. you find yourself in the thick of things.");
					printEnterWipe("You found the house-wives in an uproar, and a small army for backup.");
					printEnterWipe("The question is what will the royals do?");
					printEnterWipe("Les femmes have been at it awhile. Screaming. Shouting.");

					// MAIN MISSION SEQUENCE
					convinceRoyalsSequence(hero);
				}
			}
		}
		else
		{
			wipeScreen();
			cin.ignore();
			printEnterWipe("Maybe next time.");
		}
	}
	// Mission Completed
	else 
	{ 
		printEnterWipe("You already helped at Versailles."); 
		printEnterWipe("but the August Decrees..");
		printEnterWipe("and the Declaration of Rights MUST be signed by the King..");
		printEnterWipe("..and imprinted with his SEAL.");
		printEnterWipe("Did you have the documents?");
		signDecreesAndRight(hero);
	}
}

/**********************************************************
Description:
	Introductory dialogue only executes ONE TIME. This
	is controlled by OctoberDays_Level member: introRead.

***********************************************************/
void OctoberDays::octDaysIntroDialogue()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("There is a major bread shortage in Paris..");
	printEnterWipe("Parisian House-Wives fed-up with the lack.. went to the Palace..");
	printEnterWipe("..to Versailles..");
	printEnterWipe("..twelve miles away.");

	introRead = true;
}

/**********************************************************
Function: sapStrength()

Parameters:
	Character* victim. The user's player.

Description:
	Deciding to take the journey to Versailles is abstracted
	by taking half of victim's strength points

Effects:
	Victim's strength now half of previous

***********************************************************/
void OctoberDays::sapStrength(Character* victim)
{
	int tempStrength = 0;

	// divide strength in half
	tempStrength = victim->getStrength() / 2;
	if (tempStrength < 0) { tempStrength = 0; }

	victim->setStrength(tempStrength);
}

/****************************
 TALK/FIGHT GAME CHARACTERS
*****************************/

/**********************************************************
Function: talkToWoman()

Parameters:
	Character* hero. The user's player.

Description:
	LEVEL CHALLENGE 1. Essentially hero attempts to talk
	to one of the women who went to Versailles. If it goes
	poorly you have to 'fight' her. If it goes well, she
	asks hero to help them convince the royals. If FRENCH
	is hero's special ability, it will be in English, &
	French otherwise.

Effects:
	Either gameplay information, or combat penalties may
	occur, though the heal() call if you survive may be
	a boost to your strength after the long journey!

***********************************************************/
void OctoberDays::talkToWoman(Character* hero)
{
	cin.ignore();
	wipeScreen();

	printEnterWipe("You finally made it..");
	printEnterWipe(".. and see the women.");

	// YOU NEED FRENCH AGAIN
	learnFrenchFromConcordance(hero);

	printEnterWipe("Try asking one of them how it\'s going.");

	// The wrong response will make you regret it
	string askFrench[3] = {
		"Comment ca se passe ici?",
		"Tu as l\'air fatigue.",
		"N\'avez-vous pas faim? Ce fut une longue randonnee!"
	};

	string askEnglish[3] = { 
		"How is it going here?", 
		"You look tired", 
		"Aren\'t you hungry? That was a long hike!" 
	};

	int score = 0;
	if (hero->getAbility() == FRENCH)
	{
		score = chooseResponse(askEnglish, 3, 1);
	}
	else
	{
		score = chooseResponse(askFrench, 3, 1);
	}

	// how you respond has consequences to your health!
	speechConsequences(hero, score);
	wipeScreen();
	cin.ignore();

	if (score > 0) 
	{ 
		if (hero->getAbility() == FRENCH)
		{
			printEnterWipe("Sir.. we need help convincing the King."
				"\n   He won't sign the August Decrees or the Declaration.."
				"\n   And our families are starving! please help us!");
		}
		else
		{
			printEnterWipe("Monsieur ... nous avons besoin d\'aide pour convaincre le roi."
				"\n   Il ne signera ni les decrets d\'aout ni la déclaration.."
				"\n   Et nos familles meurent de faim!"
				"\n   Aidez nous s\'il vous plait!");
		}
	}
	else { fightWoman(hero); }
}

/**********************************************************
(helper) Function: fightWoman()

Parameters:
	Character* hero. The user's player.

Description:
	You said the wrong thing to Claire, and you two are
	going to 'fight'. She will slap you, and you will
	be DUMB. If your ability was FRENCH.. it will have
	to be regained if possible (must have concordance).

Effects:
	Increased strength if you win [after call to heal()]
	Possible to die and LOSE GAME

***********************************************************/
void OctoberDays::fightWoman(Character* hero)
{
	HouseWife* woman = new HouseWife();
	woman->setName("Claire");

	printEnterWipe("That\'s NOT what she wanted to hear!");
	printEnterWipe("You better take it like a man..");

	// fight noble
	if (twoCharactersCombat(hero, "I\'m sorry okay? Don\'t hit me!", woman, "SLAP!") == 1)
	{
		printEnterWipe("You said the wrong thing, but she forgave you I guess.");
	}
	else
	{
		printEnterWipe("SLAP! YOU\'VE BEEN HIT!");
		printEnterWipe("Apparently that was a DUMB thing to say!");
		hero->setAbility(DUMB);
	}
	woman->postCombatStats();
	hero->postCombatStats();

	if (hero->getStrength() < 0)
	{
		cout << "\n   " << woman->getName() << " slapped you to death!"
			<< "\n   GAME OVER!\n\n   <ENTER>";
		cin.get();
		wipeScreen();
		this->setLostGame();
	}
	else
	{
		// heal some
		hero->health();
	}

	// free woman
	delete woman; woman = nullptr;
}

/**********************************************************
Function: talkToGuard()

Parameters:
	Character* hero. The user's player.

Description:
	CHALLENGE 2. Analagous to 'talkToWoman' CHALLENGE above.
	Replace woman with guard instead.

Effects:
	EITHER:
		1. if NO comat
			gameplay information is displayed..
			in English (if ability is FRENCH) or
			French otherwise.

		2. if combat happens..

			[WIN]
				If hero has healing ability, may increase health if
				successfully combad guard.

			[LOSE]
				Can LOSE THE GAME!

***********************************************************/
void OctoberDays::talkToGuard(Character* hero)
{
	printEnterWipe("You\'re at the Palace of Versailles .. and so ..");
	printEnterWipe("Palacial guards seem appropriate.");
	printEnterWipe("But these ones are from Paris!");
	printEnterWipe("And.. there are thousands of them!");

	if (hero->getAbility() == FRENCH) { printEnterWipe("Who are you?"); }
	else { printEnterWipe("One of the asks, \"Qui êtes vous?\""); }

	// The wrong response will make you regret it
	string myResponse[3] = {
		hero->getName(),
		"Montesquieu RobesPierre.",
		"Bonaparte David"
	};

	int score = chooseResponse(myResponse, 3, 1);

	// how you respond has consequences to your health!
	speechConsequences(hero, score);
	wipeScreen();
	cin.ignore();

	if (score > 0)
	{
		if (hero->getAbility() == FRENCH)
		{
			printEnterWipe("Sir.. if you have come in the aid of these wives"
			"\n   then we have no quarrel.");
		}
		else
		{
			printEnterWipe("Monsieur ... si vous etes venu en aide a ces epouses,"
				"\n   nous n'avons aucune dispute.");
		}
	}
	else { fightOneGuard(hero); }
}

/**********************************************************
(helper) Function: fightOneGuard()

Parameters: 
	Character* hero. The user's player.

Description:
	Also analagous to 'fightWoman()' function described
	above. Handles situation in which 'talkToGuard'
	dissolves into combat.

Effects:
		EITHER:

		1. if NO comat
			gameplay information is displayed..
			in English (if ability is FRENCH) or
			French otherwise.

		2. if combat happens..

			[WIN]
				If hero has healing ability, may increase health if
				successfully combad guard.

			[LOSE]
				Can LOSE THE GAME!

***********************************************************/
void OctoberDays::fightOneGuard(Character* hero)
{
	PalaceGuards* guard = new PalaceGuards();
	guard->setName("Beau");
	guard->addWeaponBonus(myInventory->sabre());

	// give yourself a weapons bonus if you have one
	printEnterWipe("Look for a weapon to use..");
	hero->findAnyWeapon();

	// fight noble
	twoCharactersCombat(hero, "POW-THWAP!", guard, "BOOM-POP!");

	guard->postCombatStats();
	hero->postCombatStats();

	if (hero->getStrength() < 0)
	{
		cout << "\n   " << guard->getName() << " killed " << hero->getName()
			<< "\n   GAME OVER!\n\n   <ENTER>";
		cin.get();
		wipeScreen();
		this->setLostGame();
	}
	else
	{
		// heal some
		hero->health();
	}

	// free guard
	delete guard; guard = nullptr;

}

/**********************************************************
Function: convinceRoyalsSequence()

Parameters:
	Character* hero. The user's player.

Description:
	Main mission wrapper function. Handles sequence of
	calls to helpers and dialogue.

	Two woman attempt to convince the royals:
		- Louis XVI
		- Marie Antoinette
	.. to 'sign:
		1.	The August Decrees (The Great Fear)
		2.	The Declaration of the Right of Man 
			and of the Citizen

	Then hero attempts to convince. If convincedRoyalsCount >= 2,
	the mission succeeds. If not, it CAN dissolve into the 
	guards killing both royals and YOU LOSE.

	The women may/may not convince the guards to attack if
	they fail to convince the royals. If they cannot 
	convince the guards, hero can return and TRY AGAIN

Effects:
	Either succeed and gain Royal Seal [used to WIN]
	or possible RETRY
	or possible DEATH and LOSE GAME!

***********************************************************/
void OctoberDays::convinceRoyalsSequence(Character* hero)
{
	HouseWife* woman1 = new HouseWife();
	woman1->setName("Claire");

	HouseWife* woman2 = new HouseWife();
	woman2->setName("Monique");

	LouisXVI* louis = new LouisXVI();
	// give him the Royal Seal
	louis->getSatchel()->enqueue(myInventory->royalSeal());

	MarieAntoinette* marie = new MarieAntoinette();
	marie->getSatchel()->enqueue(myInventory->royalSeal());

	// ROYALS ARRIVE
	printEnterWipe("You see someone looking out of a window.");
	cout << "\n   It\'s..\n\n";

	louis->printStats();

	cout << "\n\n   AND\n\n";

	marie->printStats();
	printEnterWipe(" ");

	// DIALOGUE
	printEnterWipe("Finally.. they\'ve shown themselves..");
	printEnterWipe("The house-wives start to demand they be listened to.");
	printEnterWipe("They have demanded it of the National Assembly..");
	printEnterWipe("but the King must sign the August Decrees..");
	printEnterWipe("..and the Declaration of Rights.");
	printEnterWipe("You watch it all unfold.");

	// Women fight with Royals
	demandRoyals(woman1, marie);
	demandRoyals(woman2, louis);

	// hero fights with Royals
	printEnterWipe("They could use your help now.. try convincing the royals!");
	if (louis->getStrength() >= 0) { demandRoyals(hero, louis); }
	else if (marie->getStrength() >= 0) { demandRoyals(hero, marie); }

	if (convincedRoyalsCount >= 2)
	{
		printEnterWipe("They\'ve been convinced!");
		printEnterWipe("Since you helped convince them.. you can carry the documents.");

		// get Royal Seal of King
		Item* seal = louis->getSatchel()->searchSatchel("\n   Royal Seal of Approval");
		printEnterWipe(" ");

		// place in hero's satchel(queue)
		hero->getSatchel()->enqueue(seal);

		// set level status
		missionAccomplished = true;

		printUpdatedQueue(hero);
		cout << "\n";
		wipeScreen();
	}
	else
	{
		printEnterWipe("They WILL NOT YIELD!");
		printEnterWipe("The stubborness of the royals..");
		printEnterWipe("..is not lost on the Parisian Guards.");
		printEnterWipe("See if one of the women convince them to attack the palace.");

		int roll = woman1->rollInitiative();

		if (roll > 5)
		{
			guardsKillOneRoyal(louis, marie);
		}
		else
		{
			printEnterWipe("They guards are unwilling to FORCE them..");
			printEnterWipe("..to sign the Decrees and the Declaration.");
			printEnterWipe("Try again later.");
		}

	}

	// free memory
	delete marie; marie = nullptr;
	delete louis; louis = nullptr;
	delete woman2; woman2 = nullptr;
	delete woman1; woman1 = nullptr;
}

/**********************************************************
(helper) Function: demandRoyals()

Parameters:
	Character* complainer. The user's player.
	Character* king. A LouisXVI character.
	Character* queen. A MarieAntoinette character.

Description:
	Complainer challenges each royal to a 'combat' [argument]
	by calling twoCharacter() combat function. Success by
	complainer increments the convincedRoyalsCount member
	of this level. A tally of 5 will accomplish the mission.

Effects:
	Level member: convincedRoyalsCount incremented by
	maximum of 2 for each call to demandRoyals()

***********************************************************/
bool OctoberDays::demandRoyals(Character* complainer, Character* royal)
{
	bool result = false;
	string complainerTalk = " ";
	string royalTalk = " ";

	// if royal is QUEEN
	if (royal->getName() == "Marie Anoinette Queen of France")
	{
		complainerTalk.clear();
		complainerTalk = "We DEMAND bread! Our families are starving!";

		royalTalk.clear();
		royalTalk = "Let them eat Brioche! (cake)";
	}
	// if royal is KING
	else if (royal->getName() == "King Louis (Auguste) XVI of France")
	{
		complainerTalk.clear();
		complainerTalk = "The King must approve the Decrees and the Rights!";

		royalTalk.clear();
		royalTalk = "I will NOT yield to you!";
	}

	int argumentResult = twoCharactersCombat(
		complainer,
		complainerTalk,
		royal,
		royalTalk
	);

	if (argumentResult == 1)
	{
		convincedRoyalsCount++;
		cout << "\n   " << royal->getName() << " is more sympathetic now."
			<< "\n\n   <ENTER>";
		cin.get();
		wipeScreen();
		result = true;
	}

	return result;
}

/**********************************************************
Function: guardsKillOneRoyal()

Description:
	Mission Failure.

	Handles situation in which:
		1. Women and hero DID NOT convince the royals.
		2. Women persuaded the guards to attack the royals.

Effects:
	Either the King or Queen die, and you LOSE THE GAME!

***********************************************************/
void OctoberDays::guardsKillOneRoyal(LouisXVI* king, MarieAntoinette* queen)
{
	PalaceGuards* guard = new PalaceGuards();
	guard->setName("Charles");
	guard->addWeaponBonus(myInventory->charleville());

	do
	{
		twoCharactersCombat(guard, "click-BANG", king, "I am the King you idiot!");
		twoCharactersCombat(guard, "click-BANG", queen, "AWWWWW!");

	} while ((king->getStrength() >= 0) && (queen->getStrength() >= 0));

	// Print who died.. GAME OVER
	if (king->getStrength() < 0)
	{
		cout << "\n   " << king->getName() << " was assasinated by " << guard->getName() << "\n";
		printEnterWipe("The King is dead, and the mission a failure!");
		printEnterWipe("GAME OVER !!");
		this->setLostGame();
	}
	else if (queen->getStrength() < 0)
	{
		cout << "\n   " << queen->getName() << " was assasinated by " << guard->getName() << "\n";
		printEnterWipe("The Queen is dead, and the mission a failure!");
		printEnterWipe("GAME OVER !!");
		this->setLostGame();
	}

	// free memory
	delete guard; guard = nullptr;
}

/**********************************************************
(helper) Function: signDecreesAndRight()

Parameters:
	Character* hero. The user's player.

Description:
	Allows hero to WIN THE GAME by having 3 things:
		1. Royal Seal
		2. August Decrees
		3. Declaration of Rights of Man
		   and of the Citizen (Fr. Dec. of Rights)
Effects:
	Prints statements to indicate lack of items, or the
	winning of the game.

***********************************************************/
void OctoberDays::signDecreesAndRight(Character* hero)
{
	if (missionAccomplished)
	{
		int specialDocsTally = 0;

		// SIGNATURE OF ROYALTY
		if (hero->iHaveThis("\n   Royal Seal of Approval")) { specialDocsTally++; }
		else { printEnterWipe("You don\'t have the Royal Seal!"); }
		// AUGUST DECREES
		if (hero->iHaveThis("\n   The August Decrees")) { specialDocsTally++; }
		else { printEnterWipe("You don\'t have the Decrees!"); }

		// DECLARATION OF RIGHTS
		if (hero->iHaveThis("\n   Declaration of the Rights of Man, and of the Citizen")) { specialDocsTally++; }
		else { printEnterWipe("You don\'t have the Declaration either!"); }

		// If you have all 3 YOU WIN THE GAME!!
		if (specialDocsTally == 3) 
		{ 
			printEnterWipe("You delivered all of them!");
			printEnterWipe("YOU WON THE GAME !!!");
			this->setWonGame(); 
		}
	}
}


//***************************** E O F ********************************
