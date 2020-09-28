/*****************************************************************************
 * File Name: GreatFear_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20

 * Description: Function Definitions for Great Fear (Level) Class.

	SEE: Action() below for Level description.

 *****************************************************************************/
#include"GreatFear_Level.hpp"

// Default Constructor
GreatFear::GreatFear()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isUncharted();

	// fill level with items
	itemStack->push(myInventory->staleBread());
	itemStack->push(myInventory->hamburger());
	itemStack->push(myInventory->DeBouilleMap());
	itemStack->push(myInventory->torch());
	itemStack->push(myInventory->bugs());

	// Apocalypse Now
	this->setLocationName("\n   The Great Fear : French countryside. \n");
	this->setDate("July", 0, 1789);
	this->setDescription("\n   Peasants attack chateaux, burning houses and destroying registries of obligation.");

	string explanation = "\n   Famine led wandering beggars into the French countryside to steal or beg"
		"\n   from the peasant class. Rumors circulated of a vagrant army sent to destroy peasant crops."
		"\n   Combined with the events of the Estates General and the storming of the Bastille,"
		"\n   the French peasantry began burning down their noble landlords\' manor homes and the registries"
		"\n   of obligation. The Great Fear provided an impetus to the National Assembly to codify what"
		"\n   their violence had begun.";

	this->setSignificance(explanation);

	torchLit = false;
	readIntro = false;
	burnedChateau = false;
	robbedFoodCart = false;
}

// Destructor
GreatFear::~GreatFear() {}

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
void GreatFear::search(Character* hero)
{
	cout << "\n   I\'ve heard the rumors.. an army of brigands. Should I still look around?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("No time to waste.. what\' your decision?");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("Looks like the beggars picked everything clean already.");
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
	Character* rioter. The user's player.

Items required: concordance, & torch

Description:
	Handles ACTION command menu functionality.
	This is the Great Fear.. so rioter is appropriate.
	Peasants are burning and looting their Landlords
	homes.

	2 OBJECTIVES POSSIBLE:

	1. Gain an important document:
		(Declaration of Rights of Man and of the Citizen). 

		There are 2 ways to do this:
			1. A good initiative roll
			2. Fight 4 peasants. If rioter survives, gain
			   the document.

	2. Rob a food cart for the items inside

***********************************************************/
void GreatFear::action(Character* rioter)
{
	// plotline
	cin.ignore();
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// challenge
	if (!readIntro) { intro(); }
	
	// locate concordance
	if (rioter->iHaveThis("\n   English-to-French Concordance"))
	{
		if (!burnedChateau)
		{
			printEnterWipe("wait.. okay.. he said:");
			printEnterWipe("(Get a torch and come with us.)");
			printEnterWipe("Ah.. let me see..");
			if (rioter->iHaveThis("\n   torch"))
			{
				dialogue();

				int roll = rioter->rollInitiative();
				if (roll > 2)
				{
					printEnterWipe("It\'s lit.. nice job.");
					torchLit = true;
				}

				printEnterWipe("You follow the mob up to a Chateau..");
				printEnterWipe("..but someone is coming outside.");
				printEnterWipe("Bonjour? qui est là?");
				printEnterWipe("(Hello? who is there?)");
				printEnterWipe("See if you can avoid them..");

				roll = rioter->rollInitiative();

				if (roll > 4 || (rioter->getAbility() == SNEAKY))
				{
					printEnterWipe("Good. They didn\'t see you.");
					if (checkTorch(rioter, &torchLit))
					{
						printEnterWipe("Go burn down the Chateau!");
						roll = rioter->rollInitiative(); 

						if (roll > 3)
						{
							printEnterWipe("See if there\'s anything inside first.. you never know.");
							roll = rioter->rollInitiative();

							if(roll > 4)
							{
								// gained the DECLARATION
								declarationFound(rioter);
							}
							else { printEnterWipe("Nothing here."); }
							
							printEnterWipe("it\'s lit..but");
							printEnterWipe("Now you\'re a criminal!");
							burnedChateau = true;
						}
						else
						{
							fightFourPeasants(rioter);
						}
					}
				}
				// you're not sneaky FIGHT a Noble of the Sword
				else { fightNoble(rioter); }
			}
			// no torch
			else { printEnterWipe("You\'ll need to get a torch!"); }
		}
		// Chateau is already burned down
		else if(!robbedFoodCart)
		{
			printEnterWipe("The Chateau is toast.. what more do you want? -eh?");
			attackFoodCart(rioter);
		}
		else
		{
			printEnterWipe("\n   Go home before you get into more trouble!");
		}
	}
	// no concordance
	else { printEnterWipe("It\'s all Greek to me.. go get the concordance"); }
}

/********************************************************************************
Description:
	Introductory dialogue for this level is controlled by level member variable:
	readIntro. Executes once.

Effect:	readIntro set TRUE

*********************************************************************************/
void GreatFear::intro()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	printEnterWipe("It is late July - early August of 1789.");
	printEnterWipe("Looking down.. you notice that your cloth is poor..");
	printEnterWipe(".. you are dirty and thread-bare at best.");
	printEnterWipe("You are most likely one of the feudal peasantry.");
	printEnterWipe("It is dusk.. and you hear the rustling of grass..");
	printEnterWipe(".. and some heavy breathing.");
	printEnterWipe("It looks like a small mob almost.");
	printEnterWipe("Obtenez une torche et venez avec nous.");
	printEnterWipe("Huh? .. you need that concordance again..");
	printEnterWipe("Where is it?");

	readIntro = true;
}

/********************************************************************************
Description:
	Dialogue that executes after user's player has: Concordance & Torch.

*********************************************************************************/
void GreatFear::dialogue()
{
	printEnterWipe("Ok .. je l'ai ici maintenant quoi?");
	printEnterWipe("(Ok .. I have it here now what?)");

	printEnterWipe("The other peasants inform you..");
	printEnterWipe("(as if it were obvious) that a Vagrant Army is coming!");
	printEnterWipe("Wait what??");
	printEnterWipe("You seem to recollect from a previous lecture class..");
	printEnterWipe(".. that following the events of the Storming of the Bastille..");
	printEnterWipe(".. that French peasants rose up against the landed nobility.");
	printEnterWipe("..the peasants\' landlords basically.");
	printEnterWipe("They believed that magistrates had conscribed a vagrant army..");
	printEnterWipe("..to steal their food.");
	printEnterWipe("The peasants sought to get rid of..");
	printEnterWipe("the Special Privelages while they were at it.");
	printEnterWipe("Seriously weirdo dream I\'m having here.");

	printEnterWipe("Try to light that torch.");

}

/***********************************************************
(helper) Function: checkTorch()

Paramters:
	Character* rioter. The user's player.
	bool* burning. A member of GreatFear_Level.
				   It indicates if your torch is lit.

Description:
	Checks if rioter's torch is lit already. If not, then
	a relighting process occurs.

Effect:
	Rioter's torch should be lit, & burning set TRUE

************************************************************/
bool GreatFear::checkTorch(Character* rioter, bool* burning)
{
	int roll = 0;

	printEnterWipe("Is it lit?");
	if (!*burning)
	{
		do
		{
			printEnterWipe("It\'s out.. try to light it.");
			roll = rioter->rollInitiative();
			if (roll > 2)
			{
				*burning = true;
				printEnterWipe("Okay.. it\'s lit now.");
			}

		} while (!burning);
	}
	else
	{
		printEnterWipe("It\'s already lit.");
	}

	return *burning;

}

/****************************************************************
(helper) Function: fightNoble()

Parameters:
	Character* rioter. The user's player.

Description:
	Triggered if first initiative roll < 5. Rioter fights this
	nobleman, who is armed with a rifle.

Effect:
	Typical combat consequences..
		-reduced strength
		-if healing available, recovery of strength
		-death/defeat .. LOSE GAME

*****************************************************************/
void GreatFear::fightNoble(Character* rioter)
{
	printEnterWipe("You\'ve just been spotted by a Nobleman!");

	// setup noble
	NobleSword* noble = new NobleSword();
	noble->setName("Lord Chevalie");
	cout << "\n   Looks like " << noble->getName() << " is pulling out a rifle!"
		<< "\n\n   <ENTER>";
	cin.get();
	wipeScreen();

	noble->addWeaponBonus(myInventory->charleville());

	// give yourself a weapons bonus if you have one
	printEnterWipe("You better have a weapon too..");
	rioter->findAnyWeapon();

	// fight noble
	if (twoCharactersCombat(rioter, "Pa-craCK!", noble, "SH-eu-POP!") == 1)
	{
		printEnterWipe("HE MISSED! you ARE sneaky now.");
		rioter->setAbility(SNEAKY);
	}
	else
	{
		printEnterWipe("THUD! YOU\'VE BEEN HIT!");
		printEnterWipe("You\'re still alive for now.. but that was DUMB!");
		rioter->setAbility(DUMB);
	}
	noble->postCombatStats();
	rioter->postCombatStats();

	if (rioter->getStrength() < 0)
	{
		cout << "\n   " << noble->getName() << " shot you dead";
		printEnterWipe("GAME OVER!");
		this->setLostGame();
	}
	else
	{
		// heal some
		rioter->health();
	}

	delete noble; noble = nullptr;

}

/****************************************************************
(helper) Function: fightFourPeasants()

Parameters:
	Character* rioter. The user's player.

Description:
	Triggered when rioter's initiative roll < 4 at a specific
	point in the game. Rioter fights 4 peasants (3 have wpns).
	Rioter has torch stolen, and can regain if 1st peasant is
	defeated. Trophies for wins from peasants 2 & 3. If peasant
	4 defeated, then call to declarationFound() earns the 
	Declaration of Rights document.

Effect
	Possibilities:
		1. gain weapons (trophies)
		2. healing (strength increased) 
		3. gain Declaration of Rights Item
		4. death and LOSE GAME

*****************************************************************/
void GreatFear::fightFourPeasants(Character* rioter)
{
	bool survived = false;
	printEnterWipe("On second thought.. this is wrong!");
	printEnterWipe("One of the other peasants steals your torch..");

	Peasant* peasant1 = new Peasant();
	peasant1->setName("Jaques");
	peasant1->getSatchel()->enqueue(myInventory->pike());

	// give your torch to the 1st Peasant
	Item* torch = rioter->getSatchel()->searchSatchel("\n   torch");
	peasant1->getSatchel()->enqueue(torch);
	printEnterWipe(" ");

	printEnterWipe("..and lights up the Chateau anyway..");
	printEnterWipe("Now they want to kill you too!");

	Peasant* peasant2 = new Peasant();
	peasant2->setName("Gaston");
	peasant2->getSatchel()->enqueue(myInventory->sabre());

	Peasant* peasant3 = new Peasant();
	peasant3->setName("Louis");
	peasant3->getSatchel()->enqueue(myInventory->pike());

	Peasant* peasant4 = new Peasant();
	peasant4->setName("Edmond");

	int combatResults = 0;

	combatResults = twoCharactersCombat(rioter, "Eerg-ah!", peasant1, "Voosh-whoosh!");

	if (combatResults == 1)
	{
		// TROPHY
		rioter->getNewTrophy(peasant1, "\n   Revolutionary Pike");

		// get torch back
		printEnterWipe("You also got your torch back!");
		Item* torch = peasant1->getSatchel()->searchSatchel("\n   torch");
		rioter->getSatchel()->enqueue(torch);
		printEnterWipe(" ");

		combatResults = twoCharactersCombat(rioter, "Voosh-whoosh!", peasant2, "swing-slash!");

		if (combatResults == 1)
		{
			// TROPHY
			rioter->getNewTrophy(peasant2, "\n   French Sabre");

			combatResults = twoCharactersCombat(rioter, "Ching-Ching!", peasant3, "Voosh-whoosh!");

			if (combatResults == 1)
			{
				// TROPHY
				rioter->getNewTrophy(peasant3, "\n   Revolutionary Pike");

				combatResults = twoCharactersCombat(rioter, "Ching-Ching!", peasant4, "Voosh-whoosh!");

				if (combatResults == 1)
				{
					printEnterWipe("The last guy had something different..");

					// gained the DECLARATION
					declarationFound(rioter);
					survived = true;
				}

			}

		}
	}

	if (survived) { printEnterWipe("Those guys were NOT good neighbors."); }

	// printing
	rioter->postCombatStats();

	if (rioter->getStrength() < 0)
	{
		printEnterWipe("\n   Peasants shot you to death!");
		printEnterWipe("GAME OVER!");
		this->setLostGame();
	}
	else
	{
		// heal some
		rioter->health();
	}

	delete peasant1; peasant1 = nullptr;
	delete peasant2; peasant2 = nullptr;
	delete peasant3; peasant3 = nullptr;
	delete peasant4; peasant4 = nullptr;

}

/**************************************************************
(helper) Function: declarationFound()

Parameters:
	Character* rioter. The user's player.

Description:
	Provides dialogue for gameplay for 2 conditions in which
	rioter earns the Declaration of Rights document.

Effect:
	Declaration of Rights Item document in rioter's satchel
	(queue)

***************************************************************/
void GreatFear::declarationFound(Character* rioter)
{
	// FOUND DECLARATION OF RIGHTS
	printEnterWipe("You notice some type of document.");
	printEnterWipe("It is a copy of the Declaration of Rights!");
	printEnterWipe("You heard of others speaking about this..");
	printEnterWipe("Too bad it is not official.");
	printEnterWipe("It would need the King\'s seal to be authentic.");
	printEnterWipe("You better hold on to this!");
	rioter->getSatchel()->enqueue(myInventory->DeclarationRights());

}


/******************************************************
Function: attackFoodCart()

Parameters:
	Character* villain. The user's player.

Description:
	Performs vile action of theft against a food cart.
	Coachman may attack you outright, or you get a
	'slap on the wrist'. (decrease strength by 1)

Effect:
	EITHER:
		-gain cart contents
		-combat consequences as previously described.

*******************************************************/
void GreatFear::attackFoodCart(Character* villain)
{
	printEnterWipe("There\'s no more food here but..");
	printEnterWipe(".. I do see something.. it\'s..");
	FoodCart* cart = new FoodCart();
	cart->pushBed(myInventory->hamburger());
	cart->pushBed(myInventory->croutons());
	cart->pushBed(myInventory->hamburger());
	cart->pushBed(myInventory->bugs());
	cart->pushBed(myInventory->hamburger());

	Bourgeoisie* bourg = new Bourgeoisie();
	bourg->setName("Pierre Gougeon");
	bourg->getSatchel()->enqueue(myInventory->whiskey());

	cart->setPilot(bourg);
	cart->showVehicle();
	
	printEnterWipe("Famine began to plague France because of the economy"
		"\n   and the price of bread inflated greatly.");
	printEnterWipe("See if you can ransack this cart and steal the food..");
	if(villain->rollInitiative() < 4) 
	{
		printEnterWipe("The coachman isn\'t having your foolishness..");
		int combatResult = twoCharactersCombat(villain, "THUD-THUD-punch", bourg, "COUNTER-hit-Jab");
		if (combatResult == 1)
		{
			printEnterWipe("He went down like a sack of potatoes!");
			printEnterWipe("Let\'s see what was in his cart shall we?");
			cart->printBed();
			while (!cart->isBedEmpty())
			{
				villain->getSatchel()->enqueue(cart->popBed());
			}
			wipeScreen();
			printEnterWipe("Here\'s what you have now:");
			villain->getSatchel()->printQueue();
			printEnterWipe(" ");
			robbedFoodCart = true;
		}
	}
	else
	{
		villain->defend(1);
		printEnterWipe("You got slapped. That\'s what you get for theft.");
	}

	if (villain->getStrength() < 0)
	{
		cout << "\n   " << bourg->getName() << " killed you.";
		printEnterWipe("GAME OVER!");
		this->setLostGame();
	}

	cart->setPilot(nullptr);
	cart->dumpBed();
	delete cart; cart = nullptr;
	delete bourg; bourg = nullptr;

}

//***************************** E O F ********************************
