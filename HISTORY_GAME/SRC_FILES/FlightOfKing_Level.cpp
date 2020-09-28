/*****************************************************************************
 * File Name: FlightOfKing_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Flight of the King (SUB) Level.

	SEE: FlightOfKing_Level.hpp -OR- action() below for further details.

 *****************************************************************************/
#include"FlightOfKing_Level.hpp"

// Default Constructor
KingFlees::KingFlees()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->fedora());
	itemStack->push(myInventory->ChangeOfClothes());
	itemStack->push(myInventory->phonyPassports());

	// Apocalypse Now
	this->setLocationName("\n   On route to Varennes, France.\n");
	this->setDate("June", 21, 1791);
	this->setDescription("\n   The royal family flees Paris for Montmedy.\n");

	string explanation = "\n   On the night of June 21st, 1791, Louis XVI, the last Bourbon Monarch,"
		"\n   fled with his family from the Tuilieres Palace in Paris for the Fortress of Montmedy."
		"\n   The escape was a failure, that permanently tarnished his reputation with the people.";

	this->setSignificance(explanation);

	soundTally = 0;
	recognizedTally = 0;

	introComplete = false;
	hasEscapeProvisions = false;
	womenReady = false;
	loadedSilently = false;
	departedTuileries = false;

	mainMissionComplete = false;

	gendarmeCount = 0;
	dragoonCount = 0;
}

// Destructor
KingFlees::~KingFlees() {}

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
void KingFlees::search(Character* hero)
{
	cout << "\n   It\'s aweful busy here.. Should bother looking for something here?"
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

/**************************************************************************
Function: action(). ACTION cmd option.

Description:
	This mission has hero escort the French Royal Family to the Fortress
	of Montmedy. Characters consist of four royals, a madamme helper, a
	couple of bodyguards, peasants, villagers, and a small mob. There
	are multiple stops required which closely reflect the route taken
	according to the diary account of the Princess Marie-Therese from 1791.

Effect:
	Possible to WIN the GAME if successful in passing all stops along the 
	way. Also possible to LOSE by Dying

***************************************************************************/
void KingFlees::action(Character* hero)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	if (!introComplete) { kingFleesIntroSequence(); }

	// GET ITEMS:
	if (!hasEscapeProvisions) 
	{ 
		printEnterWipe("The arrangements have all been made..");
		printEnterWipe("You have but to deliver the royals to the Fortress of Montmedy..");
		printEnterWipe(".. and you will get the agreed upon price.");
		printEnterWipe("Keep a low profile, and stick to the plan..");
		printEnterWipe("First.. you need some supplies:");
		printEnterWipe("De Bouille\'s escape route, some phony passports, and changes of clothes.");
		printEnterWipe("Do you have them?");

		hasEscapeProvisions = checkSupplies(hero); 
	}

	if(hasEscapeProvisions)
	{
		if (!mainMissionComplete) { startMissionSequence(hero); }
	}

}

/*************************************************************************
Description:
	Introductory information and dialogue about the Flight to Varennes 
	by the royal family. Executes once (introComplete = TRUE).

**************************************************************************/
void KingFlees::kingFleesIntroSequence()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("By June of 1791, Louis and the royal family was effectively under house arrest.");
	printEnterWipe("The violence of the Great Fear, and the October days forced the royals to stay in Paris.");
	printEnterWipe("At the Palace of Tuileries the King took the advisement of"
		"\n   a moderate named Honore Mirabeau (born Gabriel Riqueti).");
	printEnterWipe("Under Mirabeau\'s advisement and with a plan concocted by"
		"\n   the Swedish aristocrat Alex von Ferson (the Queen\'s rumored lover),"
		"\n   the royal family accepted a plan to abscond to the Fortress of Montmedy.");
	printEnterWipe("They would have the support of loyalist soldiers, and foreign powers once they arrived.");
	printEnterWipe("The trip was some 200 miles from Paris.");

	introComplete = true;
}


/**************************************************************************
Function: startMissionSequence()

Parameters:
	Character* hero. The user's player.

Description:
	Primary wrapper for mission sequence. Instantiates:

	[1 VEHICLE]: Carriage

	[7 CHARACTERS]:
		-King -Queen -Prince -Princess -Mme. Tourzel
		-(2) Palace Guards 

	Calls:
		prepMmeQueen()
		silentlyLoadCarriage()
		waitingForKing()

	Frees:[1 VEHICLE] & [7 CHARACTERS]

**************************************************************************/
void KingFlees::startMissionSequence(Character* hero)
{
	/*MMMMMMMMMMMMMMMM
		  VEHICLE
	MMMMMMMMMMMMMMMMMM*/
	Carriage* carriage = new Carriage();

	/*MMMMMMMMMMMMMMMM
		CHARACTERS
	MMMMMMMMMMMMMMMMMM*/

	// KING
	LouisXVI* louis = new LouisXVI();
	// give him the Royal Seal
	louis->getSatchel()->enqueue(myInventory->royalSeal());
	louis->getSatchel()->enqueue(myInventory->AugustDecrees());
	louis->getSatchel()->enqueue(myInventory->DeclarationRights());

	// QUEEN
	MarieAntoinette* marie = new MarieAntoinette();

	// PRINCE
	LouisXVI* prince = new LouisXVI();
	prince->setName("Louis-Charles");
	prince->setStrength(3);

	// PRINCESS
	MarieAntoinette* princess = new MarieAntoinette();
	princess->setName("Princess Marie - Therese");
	princess->setStrength(3);

	// CHILD-CARE/ESCORT
	HouseWife* tourzel = new HouseWife();
	tourzel->setName("Madamme Tourzel");

	// SECURITY
	PalaceGuards* guard1 = new PalaceGuards();
	guard1->setName("Fritz");
	PalaceGuards* guard2 = new PalaceGuards();
	guard2->setName("Ernest");

	//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM

	printEnterWipe("It\'s night at the Tuileries palace in Paris.");
	printEnterWipe("There\'s some ambient light.. enough to make out a:");
	// display carriage
	carriage->showVehicle();

	printEnterWipe("Hearing footsteps you twist around quickly and see two figures:");

	// ROYALS ARRIVE
	cout << "\n   It\'s..\n\n";
	louis->printStats();

	cout << "\n\n   AND\n\n";

	marie->printStats();
	printEnterWipe(" ");

	// prepare women for trip
	if (!womenReady) { womenReady = prepMmeQueen(hero, tourzel, marie); }
	if (womenReady)
	{
		// leave through unguarded doors
		if (!loadedSilently) { silentlyLoadCarriage(carriage, prince, princess, tourzel, marie, guard1); }

		if (loadedSilently)
		{
			// carriage goes round and round to avoid being followed
			printEnterWipe("Now circle round in case you\'re being followed.");
			printEnterWipe("The Queen will decide when to stop.");

			// Marquis de Lafayette & Jean-Sylvain Bailly detain the king (talking) @ King's apartment

			printEnterWipe("The hour is late.. but the King is delayed.");
			printEnterWipe("Marquis de Lafayette and Jean-Sylvain Bailly are still talking to the King.");
			printEnterWipe("See if King can cut it short");

			// carriage waits for King
			waitingForKing(louis);

			printEnterWipe("Load the King! .. and another body guard.");
			// load king
			carriage->seatKing(louis, KING);
			// give sabre to loaded guard
			carriage->removeGuard(BODYGUARD1)->addWeaponBonus(myInventory->sabre());
			// give sabre to 2nd guard then load
			guard2->addWeaponBonus(myInventory->sabre());
			carriage->seatGuard(guard2, BODYGUARD2);

			// dawn
			printEnterWipe("The royal family finally departs for Montmedy.");
			printEnterWipe("It is the morning of the 21st.");

			// Stop at Etoges
			bool stillUnrecognized = Etoges(carriage);

			// Chalons-sur-Marne (large town) [4pm]
			Chalons_sur_Marne(carriage);

			// Clermont (sundown)
			stillUnrecognized = Clermont(carriage);

			// Varennes (village of 100 houses)
			mainMissionComplete = Varennes(carriage, stillUnrecognized);

			if (mainMissionComplete)
			{
				printEnterWipe("They finally made it to Montmedy!!");
				printEnterWipe("You collect your fee, and leave the continent.");
				printEnterWipe("In other words... YOU WON THE GAME !!");
				this->setWonGame();
			}
			else
			{
				printEnterWipe("The royal family was taken back to the Tuileries Palace in Paris.");
				printEnterWipe("Many were jaded by the failed escape attempts.");
				printEnterWipe("On the one side.. loyalists felt the King had abandoned them to the Revolution.");
				printEnterWipe("On the other hand.. Louis was en route to bring back an armed force.");
				printEnterWipe("He may have intended to use it against anyone in his way.");
				printEnterWipe("Louis XVI was executed by Guillotine in the Place de la Revolution..");
				printEnterWipe("January 21, 1793.");
				printEnterWipe("Mission failed.");
				printEnterWipe("YOU LOST THE GAME !!");
				this->setLostGame();
			}
		}
	}
	else
	{
		printEnterWipe("The women are not ready to go.."
			"\n   You\'ll have to try again later.");
	}

	// free royalty
	delete louis; louis = nullptr;
	delete marie; marie = nullptr;
	delete prince; prince = nullptr;
	delete princess; princess = nullptr;

	// free help
	delete tourzel; tourzel = nullptr;
	delete guard1; guard1 = nullptr;
	delete guard2; guard2 = nullptr;
	
	// carriage
	delete carriage; carriage = nullptr;

}

/**************************************************************************
(helper) Function: prepMmeQueen()

Parameters:
	Character* hero. The user's player (character)
	Character* helper. Mme. Tourzel ONLY
	Character* queenie. The Queen Marie Antoinette ONLY

	-------------------------------------------------------
SPECIAL REQUIREMENTS:
	* 'hero' MUST have:
		a) De Bouille's map
		b) Phony Passports
		c) set of clothes

	* helper checkSupplies() should ensure 'hero' has these
	  BEFORE calling THIS function.
------------------------------------------------------------

Description:
	Specialized function for FlightOfKing_Level ONLY.
	Transfers items from 'hero' to 'helper', and prints an err if 'helper'
	doesn't have them in her satchel(queue). Calls sub-helpers:
		-transferItems()
		-printEnterWipe() [screenFunctions.cpp]

Effect:
	'helper' [Mme. Tourzel] should have:
		1. De Bouille's map
		2. Phony Passport documents

************************************************************************************/
bool KingFlees::prepMmeQueen(Character* hero, Character* helper, Character* queenie)
{
	bool result = false;

	// children w/ Madamme Tourzel conducted to carriage at 10:30 pm 6-20-1791
	printEnterWipe("It is almost 10:30 p.m.");
	printEnterWipe("The children will go with Madamme Tourzel"
		"\n   and circle round the Tuileries until the King can join them.");
	printEnterWipe("Now give the supplies to her for safe keeping.");

	const int qtySupplies = 2;
	string supplies[qtySupplies] = {
		"\n   De Bouille\'s Route",
		"\n   Phony passport documents"
	};

	string queensDress = "\n   a set of clothes";

	// give Tourzel the supplies
	if (transferItems(hero, supplies, qtySupplies, helper))
	{
		// display what tourzel has
		printUpdatedQueue(helper);
		wipeScreen();

		// give the Queen her dress
		printEnterWipe(".. and the Queen will need to change.");
		if (transferItems(hero, &queensDress, 1, queenie))
		{
			printUpdatedQueue(queenie);
			wipeScreen();
			result = true;
		}
		else
		{
			printEnterWipe("The Queen couldn\'t change clothes.");
		}
	}
	else
	{
		printEnterWipe("Mme. Tourzel didn\'t get everything.");
	}

	printEnterWipe("Both women are ready now.");
	return result;
}

/**************************************************************************
(helper) Function: silentlyLoadCarriage()

**note** [CHARACTER param name] ==> [SEAT IN CARRIAGE]

Parameters:
	
	Carriage* ride. The Carriage vehicle to use.
	LouixXVI* prince. ==> prince (seat in carriage)
	MarieAntoinette* princess. ==> princess (seat)
	HouseWife* nanny. ==> escort (seat)
	MarieAntoinette* queen. ==> queen (seat)
	PalaceGuards* musle. ==> bodyguard1 (seat)

Description:
	Handles sequence of loading 'ride' [a carriage] with included param 
	list of Characters. Each character being loaded may make noise and 
	increment soundTally per checkMadeNoise() call on EACH param Character.
	Prints carriage manifest at end

Effect:
	Possible:
		-noises by a certain Character will..
			1. increment soundTally++ member
			2. print who made a sound
			3. spec. abil. of noise character
			   becomes DUMB

		-no noise made

**************************************************************************/
bool KingFlees::silentlyLoadCarriage(
	Carriage* ride,
	LouisXVI* prince,
	MarieAntoinette* princess,
	HouseWife* nanny,
	MarieAntoinette* queen,
	PalaceGuards* muscle)
{
	// leave through unguarded doors
	printEnterWipe("Okay.. now.."); 
	printEnterWipe("Tourzel");
	printEnterWipe("Prince Louis-Charles");
	printEnterWipe("and the Princess Marie-Therese..");
	printEnterWipe("will have to sneak through an unguarded door, and into the carriage"
		"\n   WITHOUT making a sound.");

	printEnterWipe("Have the Prince go first.");
	if (checkMadeNoise(prince)) { soundTally++; }
	ride->seatKing(prince, PRINCE);

	printEnterWipe("Now the princess.");
	if (checkMadeNoise(princess)) { soundTally++; }
	ride->seatQueen(princess, PRINCESS);

	printEnterWipe("Next goes Tourzel..");
	if (checkMadeNoise(nanny)) { soundTally++; }
	ride->seatEscort(nanny);

	printEnterWipe("Then the Queen..");
	if (checkMadeNoise(queen)) { soundTally++; }
	ride->seatQueen(queen, QUEEN);

	printEnterWipe(".. and One body guard.");
	ride->seatGuard(muscle, BODYGUARD1);
	//-----------------------------------------------------

	if (soundTally < 4) 
	{ 
		// print carriage manifest
		cout << "\n";
		ride->printManifest();
		printEnterWipe(" ");

		printEnterWipe("The plan was to use multiple carriages, but the Queen insisted on this ONE carriage.");
		printEnterWipe("Using 6 horses.. it is a little conspicuous.");

		loadedSilently = true; 
	}
	else
	{
		printEnterWipe("They made too much noise loading the carriage.");
		printEnterWipe("You\'ll have to try again tomorrow night.");
		soundTally = 0;
	}

	return loadedSilently;
}


/**************************************************************************
Function: Etoges()

Parameters: 
	Carriage* carriage. The loaded carriage being used.

Description:
	First stop on route to Montmedy. Calls groupIsIncognito to
	determine if the group has been recognized yet.

Effect:
	Failure to stay incognito will increment the recognizedTally
	member and possibly stop the carriage.

**************************************************************************/
bool KingFlees::Etoges(Carriage* carriage)
{
	bool passed = false;

	// Etoges stop (maybe known?)
	printEnterWipe("You are approaching Etoges, the first stop.");
	printEnterWipe("Avoid being seen.");

	// avoid being recognized
	passed = groupIsIncognito(carriage);

	return passed;
}

/**************************************************************************
Function: Chalons_sur_Marne()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	Second stop on route to Montmedy. Calls groupIsIncognito to
	determine if the group talks to peasant in the village.

Effect:
	Failure to stay incognito will increment the recognizedTally
	member and possibly stop the carriage.

**************************************************************************/
void KingFlees::Chalons_sur_Marne(Carriage* carriage)
{
	printEnterWipe("It is now 4 p.m.");
	printEnterWipe("You are approaching a large town called,"
		"\n   Chalons-sur-Marne");
	printEnterWipe("You have to change horses, so there is no choice but to stop.");
	printEnterWipe("A peasant comes close to the carriage.");
	printEnterWipe("See if he recognizes you.");

	// if recognized.. get fed by peasant !
	if (!groupIsIncognito(carriage)) { feedCarriage(carriage); }
}

/**************************************************************************
Function: Clermont()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	Third stop on route to Montmedy. Calls stayIncognito on the King only. 
	If spotted, an Officer informs the King that they have been betrayed. 
	If this happens, and you still have De Bouille's route, you may be able 
	to avert future harm.

Effect:
	If recognized & have DeBouille's route, you can avert future
	harm [possibly]. If recognized the recognizedTally increments.

**************************************************************************/
bool KingFlees::Clermont(Carriage* carriage)
{
	bool passed = false;

	printEnterWipe("Sundown approaches and you approach Clermont.");
	printEnterWipe("Whilst there, an officer comes close to the carriage.");
	printEnterWipe("See if he recognizes the King.");

	passed = stayIncognito(carriage->removeKing(KING));
	if (!passed)
	{
		// Officer recognized king. comes close. whispers King is betrayed.
		printEnterWipe("Being recognized is not so bad in this case.");
		printEnterWipe("The officer informs you that they have been BETRAYED!");
		printEnterWipe("Check to see you you still have De Bouille\'s map.");
		if (carriage->removeEscort()->iHaveThis("\n   De Bouille\'s Route"))
		{
			printEnterWipe("Good.. you didn\'t lose it!");
			printEnterWipe("The mood of the carriage is sober though.");
			printEnterWipe("Armed with this knowledge.. you decide on an alternate route.");

			// make all passengers SNEAKY
			carriage->removeKing(KING)->setAbility(SNEAKY);
			carriage->removeKing(PRINCE)->setAbility(SNEAKY);
			carriage->removeQueen(QUEEN)->setAbility(SNEAKY);
			carriage->removeQueen(PRINCESS)->setAbility(SNEAKY);
			carriage->removeEscort()->setAbility(SNEAKY);
			carriage->removeGuard(BODYGUARD1)->setAbility(SNEAKY);
			carriage->removeGuard(BODYGUARD2)->setAbility(SNEAKY);

			printEnterWipe("Everybody in the carriage is now SNEAKY.");
			recognizedTally /= 2;
			passed = true;
		}
		else
		{
			printEnterWipe("It\'s not here.");
			printEnterWipe("We will not alter our course then.");
			printEnterWipe("There\'s no alternative.");
		}
	}
	else
	{
		printEnterWipe("He didn\'t recognize the King.");
	}

	return passed;
}

/**************************************************************************
Function: Varennes()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	Final stop on path to Varennes. This is the town where the
	royal family was actually captured, but in the game there
	are other possibilities. 

Effect:
	Success: WIN GAME
	Failure: LOSE GAME

**************************************************************************/
bool KingFlees::Varennes(Carriage* carriage, bool unrecognized)
{
	bool passed = false;

	// Varennes (village of 100 houses)
	printEnterWipe("It is night again.");
	printEnterWipe("Just ahead is Varennes, a village with about 100 houses.");
	printEnterWipe("It would be unwise to stop now..");
	printEnterWipe("But you might have no choice.. the horses could need changing ..");

	Bourgeoisie* postmaster = new Bourgeoisie();
	postmaster->setName("Jean-Baptiste Drouet");

	// STOP STOP
	int roll = postmaster->rollInitiative();
	if (roll > 4 || !unrecognized)
	{
		cout << "\n   " << postmaster->getName() << " recognized the King from his coin!"
			<< "\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		if (inspectCarriage(carriage)) 
		{ 
			printEnterWipe("You pass though Varennes.. en route to the Fortress!");
			passed = true; 
		}
	}
	else
	{
		printEnterWipe("And you don\'t have to!");
		passed = true;
	}

	// free postmaster
	delete postmaster; postmaster = nullptr;

	return passed;
}

/***************
	HELPERS
****************/

/**************************************************************************
(helper) Function: checkSupplies()

Parameters:
	Character* royal. Usu. user's player.

Description:

	'royal' is supposed to have:
		1. De Bouille's route,
		2. Phony Passport documents,
		3. Set of clothes.

	Validates prior to startMissionSequence() call

Returns:
	if all items present: TRUE
	otherwise: FALSE

**************************************************************************/
bool KingFlees::checkSupplies(Character* royal)
{
	bool result = false;

	string id = "\n   Phony passport documents";
	string costumes = "\n   a set of clothes";
	string map = "\n   De Bouille\'s Route";

	if (royal->iHaveThis(id))
	{
		if (royal->iHaveThis(costumes))
		{
			if (royal->iHaveThis(map))
			{
				printEnterWipe("Nice work.. you brought everything we\'ll need.");
				result = true;
			}
			else
			{
				cout << "\n   " << royal->getName() << " doesn\'t have De Bouille\'s escape route.";
				printEnterWipe(" ");
			}

		}
		else
		{
			cout << "\n   " << royal->getName() << " forgot the disguises.";
			printEnterWipe(" ");
		}
	}
	else
	{
		cout << "\n   " << royal->getName() << " forgot the phony passports.";
		printEnterWipe(" ");
	}

	return result;
}

/**************************************************************************
(helper) Function: transferItems()

Parameters:
	1. Character* giver. The Character with the items BEFORE the call.
	2. string* items. A string array with names of items to give.
	3. int numItems. The size of the string array. (not an imm. index)
	4. Character* receiver. The Character with the items AFTER the call.

Description:
	-Prints statement about who is giving what to whom.
	-Iterates until < numItems.
	-Calls searchSatchel() [queue class helper] to locate item by string.
	-If not nullptr, the returned item is enqueued into receiver's satchel
		[queue class helpers here too].
	-Each iter:
		prints whether receiver received 'item'

Effect:
	All 'items' go from giver --to--> receiver.
	Print statements when:
		items found with giver
		items received
		items not received

Returns:
	If all items transferred : TRUE
	Otherwise: FALSE

***************************************************************************/
bool KingFlees::transferItems(Character* giver, string* items, int numItems, Character* receiver)
{
	bool hasNewItems = false;
	int itemsReceivedTally = 0;

	for (int i = 0; i < numItems; i++)
	{
		cout << "\n   " << giver->getName() << " wants to give " << items[i] << " to " << receiver->getName() << "."
			<< "\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		Item* something = giver->getSatchel()->searchSatchel(items[i]);
		printEnterWipe(" ");

		if (something != nullptr)
		{
			receiver->getSatchel()->enqueue(something);
			if (receiver->iHaveThis(items[i]))
			{
				cout << "\n   " << receiver->getName() << " now has " << items[i] << ".";
				printEnterWipe(" ");

				itemsReceivedTally++;
			}
			else
			{
				cout << "\n   " << receiver->getName() << " didn\'t receive: " << items[i] << ".";
				printEnterWipe(" ");
			}
		}
	}
	if (itemsReceivedTally == numItems) { hasNewItems = true; }

	return hasNewItems;
}

/**************************************************************************
(helper) Function: checkMadeNoise()

Parameters:
	Character* royal. Some game character

Description:
	Evaluates if 'royal' made a noise by comparing to their
	initiative roll. A low roll is desired.

Effect:
	Conditions:
		initiative roll < 3: SNEAKY ability + returns FALSE
		initiative roll > 5: gain DUMB + returns TRUE

***************************************************************************/
bool KingFlees::checkMadeNoise(Character* royal)
{
	bool madeSound = false;

	if (royal->getAbility() != SNEAKY)
	{
		// low roll makes royal SNEAKY
		int roll = royal->rollInitiative();

		// SILENT
		if (roll < 3) { royal->setAbility(SNEAKY); }

		// MADE NOISE!
		else if (roll > 5) 
		{ 
			madeSound = true;
			royal->setAbility(DUMB);	
			cout << "\n   " << royal->getName() << " made some noise!\n\n   <ENTER>";
			cin.get();
			wipeScreen();
		}
	}

	return madeSound;
}

/*******************************************************************************************
(helper) Function: waitingForKing()

Parameters:
	Character* king. The Louis XVI Character.

Description:
	Marquis de Lafayette & Jean-Sylvain Bailly detain the king (by talking).
	This acts as an abstraction of waiting for them to stop talking so the king can
	leave with his family.

Effect:
	Every initiative roll < 5 increments the soundTally member variable of the level.
	Prints game dialogue.

*******************************************************************************************/
void KingFlees::waitingForKing(Character* king)
{
	int roll = 0;
	do
	{
		roll = king->rollInitiative();
		if (roll > 4)
		{
			printEnterWipe("Good! They\'re done");
		}
		else
		{
			printEnterWipe("You\'ll have to circle the block.. they\'re not done.");
			soundTally++;
		}

	} while (roll < 5);
}

/**************************************************************************
(helper) Function: groupIsIncognito()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	Calls stayIncognito on every present member of carriage.
	Recognized passengers increment the recognizedTally per each offender.
	If more than 3 are recognized, the entire carriage is recognized.

Effect:
	If more than 3 are recognized, this returns TRUE, and the stopCarriage
	Level member will be set.

***************************************************************************/
bool KingFlees::groupIsIncognito(Carriage* carriage)
{
	bool remainedAnon = false;

	LouisXVI* theKing = carriage->removeKing(KING);
	LouisXVI* thePrince = carriage->removeKing(PRINCE);
	MarieAntoinette* theQueen = carriage->removeQueen(QUEEN);
	MarieAntoinette* thePrincess = carriage->removeQueen(PRINCESS);
	HouseWife* theHelp = carriage->removeEscort();

	if (theKing != nullptr) { if (!stayIncognito(theKing)) { recognizedTally++; } }
	if (thePrince != nullptr) { if (!stayIncognito(thePrince)) { recognizedTally++; } }
	if (theQueen != nullptr) { if (!stayIncognito(theQueen)) { recognizedTally++; } }
	if (thePrincess != nullptr) { if (!stayIncognito(thePrincess)) { recognizedTally++; } }
	if (theHelp != nullptr) { if (!stayIncognito(theHelp)) { recognizedTally++; } }

	if(recognizedTally >= 3)
	{
		printEnterWipe("The carriage has been recognized!");
		printEnterWipe("Be ready for anything.");
	}
	else
	{
		printEnterWipe("Don\'t worry so much.. not enough of you were recognized you this time.");
		remainedAnon = true;
	}
	return remainedAnon;
}

/**************************************************************************
(helper) Function: stayIncognito()

Parameters:
	Character* whom. Some Character in the game.

Description:
	Determines if 'whom' was recognized by comparing their intiative roll. 

Effect:
	If 'whom' rolls > 5, and their ability was SNEAKY, it is now DUMB. 
	A print will also trigger if recognized.

Returns:
	int incognito score.

***************************************************************************/
bool KingFlees::stayIncognito(Character* whom)
{
	bool notRecognizedYet = true;

	int incognitoScore = whom->rollInitiative();
	if (incognitoScore >= 5)
	{
		cout << "\n   " << whom->getName() << " was recognized!\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		if (whom->getAbility() == SNEAKY) { whom->setAbility(DUMB); }
		notRecognizedYet = false;
	}
	return notRecognizedYet;
}

/**************************************************************************
(helper) Function: feedCarriage()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	Called by Chalons_sur_Marne() to let a peasant feed the carriage
	company. [Character_Class] addFoodBonus() helper & [Inventory_Class]
	Item* constructors accomplish that task.

Effect:
	Every existing Character in carriage is fed. The health-bonus of the
	food item is added to the Character's strength value.

***************************************************************************/
void KingFlees::feedCarriage(Carriage* carriage)
{
	Peasant* peasant = new Peasant();
	peasant->setName("Francois");
	peasant->setAbility(PRAYER);

	// attempt to talk??
	printEnterWipe("One of the locals approaches the carriage..");
	peasant->printStats();
	cout << "\n\n   <ENTER>";
	cin.get();
	wipeScreen();

	printEnterWipe("Your majesty.. God Bless You. We pray for your safety.");
	printEnterWipe("Well.. at least someone likes them.");
	printEnterWipe("Francois has food for everyone..");

	if (carriage->removeKing(KING) != nullptr)
	{
		carriage->removeKing(KING)->addFoodBonus(myInventory->wineGlass());
		cin.get(); 
		wipeScreen();
	}
	if (carriage->removeQueen(QUEEN) != nullptr)
	{
		carriage->removeQueen(QUEEN)->addFoodBonus(myInventory->wineGlass());
		cin.get();
		wipeScreen();
	}
	if (carriage->removeKing(PRINCE) != nullptr)
	{
		carriage->removeKing(PRINCE)->addFoodBonus(myInventory->staleBread());
		cin.get();
		wipeScreen();
	}
	if (carriage->removeQueen(PRINCESS) != nullptr)
	{
		carriage->removeQueen(PRINCESS)->addFoodBonus(myInventory->staleBread());
		cin.get();
		wipeScreen();
	}
	if (carriage->removeEscort() != nullptr)
	{
		carriage->removeEscort()->addFoodBonus(myInventory->hamburger());
		cin.get();
		wipeScreen();
	}
	if (carriage->removeGuard(BODYGUARD1) != nullptr)
	{
		carriage->removeGuard(BODYGUARD1)->addFoodBonus(myInventory->croutons());
		cin.get();
		wipeScreen();
	}
	if (carriage->removeGuard(BODYGUARD2) != nullptr)
	{
		carriage->removeGuard(BODYGUARD2)->addFoodBonus(myInventory->croutons());
		cin.get();
		wipeScreen();
	}

	printEnterWipe("Merci Monsieur!");

	delete peasant; peasant = nullptr;
}

/**************************************************************************
(helper) Function: checkPapers() : 

Parameters:
	Character* detainee. The Character with the Phony passports.

Description:
	HELPER for : inspectCarriage().

	A gendarme checks to see if detainee has an id, and asks them who
	they are. If they answer correctly, they get their fake id back.
	
Returns:
	If answered correctly [answerDamage > 0] : TRUE
	Otherwise : FALSE

***************************************************************************/
bool KingFlees::checkPapers(Character* detainee)
{
	bool passed = false;

	cout << "\n   Have " << detainee->getName() << " show them the papers.";
	printEnterWipe(" ");

	if (detainee->iHaveThis("\n   Phony passport documents"))
	{
		Item* id = detainee->getSatchel()->searchSatchel("\n   Phony passport documents");
		printEnterWipe(" ");

		string responses[4] = {
			"Mme. Karst and her family",
			"Mme. Korff and her family",
			"Mme. Tourzel and her family",
			"Mlle. Touissant and her family"
		};

		// get a gendarme
		hireGendarme(gendarmeCount);

		cout << "\n   " << Gendarmarie[gendarmeCount-1]->getName() << " asks you: \"Who are you?\""
			<< "\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		// get rid of gendarme
		fireGendarme(gendarmeCount - 1);

		int answerDamage = chooseResponse(responses, 4, 2);
		speechConsequences(detainee, answerDamage);

		// give passport back to tourzel
		detainee->getSatchel()->enqueue(id);

		// detainee answered correctly
		if (answerDamage > 0)
		{
			wipeScreen();
			cin.ignore();
			printEnterWipe("Everything appears to be in order here. Just a routine check.");
			printEnterWipe("Good evening Mme. Korff");
			passed = true;
		}
	}
	else
	{
		printEnterWipe("You\'re missing documents!");
	}

	return passed;
}

/**************************************************************************
(helper) Function: inspectCarriage()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	
***************************************************************************/
bool KingFlees::inspectCarriage(Carriage* carriage)
{
	printEnterWipe("STOP! STOP! .. you are commanded");
	printEnterWipe("They are asking for papers.");
	
	// check Mme. Tourzel's papers
	bool passedInspection = checkPapers(carriage->removeEscort());

	if (!passedInspection)
	{
		wipeScreen();
		cin.ignore();
		printEnterWipe("What she said doesn\'t match her papers..");
		printEnterWipe("They ask everyone to get out..");
		printEnterWipe("See if one of the royals can convince him");
		printEnterWipe("Gendarme: \"If you cannot be honest.. I WILL arrest you!");
		
		passedInspection = checkPapers(carriage->removeEscort());

		// Didn't pass = INSPECTION!
		if (!passedInspection) { passedInspection = stoppedByGendarmes(carriage); }
	}
	if (passedInspection)
	{
		printEnterWipe("WHEY! ~ ~ ~ that was CLOSE!");
	}
	return passedInspection;
}

/**************************************************************************
(helper) Function: stoppedByGendarmes()

Parameters:
	Carriage* carriage. The loaded carriage being used.

Description:
	In the event that Tourzel doesn\'t answer correctly.. She is
	asked again. If she fails the second time:
		-Gendarme asks them to GET OUT
		-Princess tries to persuade him to let them go
			-if fail: they are captured [LOSE GAME]
			-if succeed: Dragoons show up
				-King attempts to get their attention
					if succeed: 
						The Gendarmes & Dragoons argue
						-King rolls to see what happens
							if roll [1..3] 
								THEY FIGHT
							else
								They go free! [YOU WIN]

***************************************************************************/
bool KingFlees::stoppedByGendarmes(Carriage* carriage)
{
	bool passed = false;

	wipeScreen();
	cin.ignore();
	printEnterWipe("Gendarmes: \"GET OUT NOW!\"");
	printEnterWipe("The Princess tries to sway them..");

	int roll = carriage->removeQueen(PRINCESS)->rollInitiative();

	if (roll < 3)
	{
		cout << "\n   " << carriage->removeQueen(PRINCESS)->getName() << " says.."
			<< "\n   Messieurs! we are just travellers! Please let us pass!"
			<< "\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		printEnterWipe("Your MAJESTY! I am sorry.. but you must come back with us!");
	}
	else
	{
		printEnterWipe("Suddenly there is a unit of Mounted Dragoons that pass by!");
		printEnterWipe("There doesn\'t appear to be an officer with them so..");
		printEnterWipe(".. try to get them to help you.");

		roll = carriage->removeKing(KING)->rollInitiative();

		if (roll > 3)
		{
			printEnterWipe("GOOD! They\'re going to help you!");
			printEnterWipe("Dragoon: \"What goes on here?\"");
			printEnterWipe("Gendarme: \"Their papers are not in order.\"");
			printEnterWipe("Gendarme: \"Besides you have no authority here!\"");
			printEnterWipe("A fight could be stirring.. roll to see what happens.");

			roll = carriage->removeKing(KING)->rollInitiative();

			if (roll > 3)
			{
				printEnterWipe("The Gendarmes back down.. and the Dragoons let you go!");
				printEnterWipe("That was close!");
				passed = true;
			}
			else
			{
				// GENDARMES
				callGendarmarie();
				armGendarmarie();

				printEnterWipe("The Gendarmes lower their rifles at the carriage forcing you out!");

				// MOUNTED DRAGOON SOLDIERS
				callDragoons();
				armDragoons();

				printEnterWipe("The Dragoons point theirs at the Gendarmes..");

				printEnterWipe(".. and it turns into a shooting gallery!");
				brawl();

				if(dragoonCount > 0)
				{
					printEnterWipe("The Dragoons Saved the Day!");
					passed = true;
				}
				else
				{
					printEnterWipe("The Dragoons were all killed..");
					printEnterWipe("And you are ALL under arrest!");
				}

				// free Dragoons
				disbandDragoons();

				// free Gendarmarie
				closeGendarmarie();
			}
		}
		else
		{
			printEnterWipe("..but they just pass you by.");

			// two carriage guards get out and fight!
			bool guardsSucceeded = guardsDefendCarriage(carriage);

			if (guardsSucceeded)
			{
				printEnterWipe("Your Palace Bodyguards rescued you from capture!");
				printEnterWipe("You giddy-up the horses and take off!");
				passed = true;
			}
			else
			{
				printEnterWipe("The gendarmes lower their weapons.. a bit shocked.");
				printEnterWipe("Your MAJESTIES! I am sorry.. but you must come back with us!");
			}

		}

	}
	return passed;
}

/*MMMMMMMMMMMMMMMMM
	GENDARMARIE
MMMMMMMMMMMMMMMMM*/

/***************************************************************************
Function: hireGendarme()

Description:
	Calls PalaceGuards() constructor to instantiate a new Gendarme on the
	heap, and give them a generic name. Checks Gendarmarie[rank] is nullptr
	before proceeding.

Effect:
	New Gendarme in Gendarmarie[rank]. gendarmeCount incremented.

***************************************************************************/
void KingFlees::hireGendarme(int rank)
{
	const size_t nameLength = 15 * sizeof(char);
	char name[nameLength];
	memset(name, '\0', nameLength);
	int trueNum = rank + 1;
	snprintf(name, nameLength, "Gendarme #%d", trueNum);

	Gendarmarie[rank] = nullptr;
	Gendarmarie[rank] = new PalaceGuards();
	Gendarmarie[rank]->setName(name);
	gendarmeCount++;
}

/***************************************************************************
Function: callGendarmarie()

Description:
	Calls hireGendarme() to instantiate a new Gendarme policeman,
	name them, and place them into the Gendarmarie[] array (Level member).

***************************************************************************/
void KingFlees::callGendarmarie()
{

	// MOUNTED DRAGOON SOLDIERS
	for (int i = 0; i < MAX_GENDARMARIE; i++)
	{
		hireGendarme(i);
	}
}

/***************************************************************************
Function: armGendarmarie()

Description:
	Calls addWeaponBonus() helper to add a rifle to every
	available Gendarme. Bonus is printed by the addWeaponBonus() call.

***************************************************************************/
void KingFlees::armGendarmarie()
{
	printEnterWipe("The Gendarmarie are being armed.");
	for (int i = 0; i < gendarmeCount; i++)
	{
		Gendarmarie[i]->addWeaponBonus(myInventory->charleville());
	}
}

/***************************************************************************
Function: fireGendarme()

Parameters:
	int rank. The array index where the gendarme lives.

Description:
	Frees memory for ONE Gendarme in the Gendarmarie[rank] array.

Effect:
	Gendarme is de-allocated at end of call.

***************************************************************************/
void KingFlees::fireGendarme(int rank)
{
	if (Gendarmarie[rank] != nullptr)
	{
		delete Gendarmarie[rank];
		Gendarmarie[rank] = nullptr;
		gendarmeCount--;
	}
}

/***************************************************************************
Function: closeGendarmarie()

Description:
	Calls fireGendarme() to free all memory for each gendarme in the
	Gendarmarie[] array. (member of THIS Level).

Effect:
	All Gendarmes de-allocated at end of call.

***************************************************************************/
void KingFlees::closeGendarmarie()
{
	for (int i = 0; i < MAX_GENDARMARIE; i++)
	{
		fireGendarme(i);
	}
}

/*MMMMMMMMMMMMMMMMMMMM
	MOUNTED DRAGOONS
MMMMMMMMMMMMMMMMMMMMMM*/

/***************************************************************************
Function: recruitDragoon()

Description:
	Calls PalaceGuards() constructor to instantiate a new Dragoon on the
	heap, and give them a generic name. Checks Dragoon[rank] is nullptr
	before proceeding.

***************************************************************************/
void KingFlees::recruitDragoon(int rank)
{
	const size_t nameLength = 15 * sizeof(char);
	char name[nameLength];
	memset(name, '\0', nameLength);
	int trueNum = rank + 1;
	snprintf(name, nameLength, "Dragoon #%d", trueNum);

	Dragoons[rank] = nullptr;
	Dragoons[rank] = new PalaceGuards();
	Dragoons[rank]->setName(name);
	dragoonCount++;
}

/***************************************************************************
Function: callDragoons()

Description:
	Calls recruitDragoon() to instantiate a new Dragoon troop
	name them, and place them into the Dragoons[] array (Level member).

***************************************************************************/
void KingFlees::callDragoons()
{
	for (int i = 0; i < MAX_DRAGOONS; i++)
	{
		recruitDragoon(i);
	}
}

/***************************************************************************
Function: armDragoon()

Description:
	Calls addWeaponBonus() helper to add a sabre, and rifle to every
	available Dragoon. Bonus is printed by the addWeaponBonus() call.

***************************************************************************/
void KingFlees::armDragoons()
{
	printEnterWipe("The Dragoons are making ready their weapons.");
	for (int i = 0; i < dragoonCount; i++)
	{
		if (Dragoons[i] != nullptr)
		{
			Dragoons[i]->addWeaponBonus(myInventory->sabre());
			Dragoons[i]->addWeaponBonus(myInventory->charleville());
		}
	}
}

/***************************************************************************
Function: dischargeDragoon()

Parameters:
	int rank. The array index where the dragoon lives.

Description:
	Frees memory for ONE Dragoon in the Dragoons[rank] array.

Effect:
	Dragoon is de-allocated at end of call.

***************************************************************************/
void KingFlees::dischargeDragoon(int rank)
{
	if (Dragoons[rank] != nullptr)
	{
		delete Dragoons[rank];
		Dragoons[rank] = nullptr;
		dragoonCount--;
	}
}

/***************************************************************************
Function: disbandDragoons()

Description:
	Calls dischargeDragoon() to free all memory for each dragoon in the
	Dragoons[] array. (member of THIS Level).

Effect:
	All Dragoons de-allocated at end of call.

***************************************************************************/
void KingFlees::disbandDragoons()
{
	for (int i = 0; i < MAX_DRAGOONS; i++)
	{
		dischargeDragoon(i);
	}
}

/***************************************************************************
Function: brawl()

Description:
	Calls gendarmeVsDragoon() to fight Gendarme police vs Dragoon soldiers.
	Even parity index i gives the Genarmarie the initiative. Brawl continues
	until one side is dead. 
	
	SEE gendarmeVsDragoon for more details

Effect:
	One of the two teams will be destroyed, and each member deallocated
	as required.

***************************************************************************/
void KingFlees::brawl()
{
	int maxToUse = 0;
	if (MAX_DRAGOONS <= MAX_GENDARMARIE) { maxToUse = MAX_DRAGOONS; }
	else if (MAX_GENDARMARIE < MAX_DRAGOONS) { maxToUse = MAX_GENDARMARIE; }

	for(int i = 0; i < maxToUse; i++)
	{
		if (i % 2 == 0)
		{
			gendarmareVsDragoon(Gendarmarie[i], i, Dragoons[i], i, DRAGOON);
		}
		else
		{
			gendarmareVsDragoon(Gendarmarie[i], i, Dragoons[i], i, GENDARMARIE);
		}
		i++;
	} 

}

/***************************************************************************
(helper) Function: gendarmareVsDragoon()

Parameters:
	Character* gendarme. The policeman.
	int gRank. Their index position in the Gendarmarie[].

	Character* dragoon. The soldier.
	int dRank. Thier index position in the Dragoons[] array.

Description:
	Performs one-on-one combat of a Gendarme vs. a Dragoon. Whichever
	happens to lose is de-allocated by calls to:
		-fireGendarme() -OR-
		-dischargeDragoon()

	Aggressor CANNOT attack with negative strength.

Effect:
	Loser dies & is de-allocated.

***************************************************************************/
void KingFlees::gendarmareVsDragoon(
	// parameters:
	Character* gendarme, 
	int gRank, 
	Character* dragoon, 
	int dRank, 
	enum FirstAttack aggressor)
	//--------------------------
{
	string bang = "BANG ~~ Poof";
	string pop = "Ch-clink ** BOOM";
	int combatResult = 0;

	// Gendarmarie has the initiative
	if (aggressor == GENDARMARIE)
	{
		combatResult = twoCharactersCombat(gendarme, pop, dragoon, bang);
		if (combatResult == 1) { dischargeDragoon(dRank); }
		else { fireGendarme(gRank); }
	}
	// Dragoons have the initiative
	else if (aggressor == DRAGOON)
	{
		combatResult = twoCharactersCombat(dragoon, bang, gendarme, pop);
		if (combatResult == 1) { fireGendarme(gRank); }
		else { dischargeDragoon(dRank); }
	}
}

/***************************************************************************
Function: guardsDefendCarriage()

Helpers:
	twoCharacterCombat()
	callGendarmarie()
	closeGendarmarie()

Paramters:
	Carriage* carriage. The loaded carriage with 2 Palace Guards inside.

Description:
	Performs combat between the 'carriage' body-guards and the Gendarmes.
	The guards only have to defeat 3 Gendarmes.

Returns:
	If Guards win: TRUE (Royals escape)
	Otherwise: FALSE (Royals are captured)

***************************************************************************/
bool KingFlees::guardsDefendCarriage(Carriage* carriage)
{
	bool guardsWon = false;

	string bang = "BANG ~~ Poof";
	string pop = "Ch-clink ** BOOM";

	PalaceGuards* g1 = carriage->removeGuard(BODYGUARD1);
	PalaceGuards* g2 = carriage->removeGuard(BODYGUARD2);

	printEnterWipe("The Gendarmes un-sling their muskets.");

	// GENDARMES
	callGendarmarie();
	armGendarmarie();

	printEnterWipe("The Gendarmes lower their rifles at the carriage forcing you out!");
	printEnterWipe("Your Royal Guards will have to defend you instead!");
	printEnterWipe("They start attacking the Gendarmes..\n   hoping for a chance to escape!");

	int gendarmesDefeatedTally = 0;

	// fight
	for (int i = 0; i < MAX_GENDARMARIE; i++)
	{
		int combatResult = twoCharactersCombat(g1, bang, Gendarmarie[i], pop);
		if (combatResult == 1) { gendarmesDefeatedTally++; }

		combatResult = twoCharactersCombat(g2, bang, Gendarmarie[i], pop);
		if (combatResult == 1) { gendarmesDefeatedTally++; }

		if (gendarmesDefeatedTally >= 2) { guardsWon = true; break; }
	}

	// free Gendarmes
	closeGendarmarie();

	return guardsWon;
}

//***************************** E O F ********************************
