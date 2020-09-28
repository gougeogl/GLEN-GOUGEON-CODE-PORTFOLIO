/*****************************************************************************
 * File Name: CharacterClass.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Character BASE Class :

 *****************************************************************************/
#include"CharacterClass.hpp"

// Constructor
Character::Character()
{
	name = " ";	
	ability = NONE;
	strength = 0;
	armor = 0;
	lastAttackRoll = 0;
	lastDefenseRoll = 0;
	pocket = nullptr;
	satchel = new Queue;
}

// Destructor
Character::~Character()
{
	delete satchel;
	satchel = nullptr;
}

// NAME
void Character::setName(string title)
{
	this->name = title;
}

string Character::getName()
{
	return this->name;
}

// ABILITY
void Character::setAbility(Ability skill)
{
	this->ability = skill;
}

int Character::getAbility()
{
	return this->ability;	
} 

// STRENGTH
void Character::setStrength(int now)
{
	this->strength = now;
}

int Character::getStrength()
{
	return this->strength;
} 

/*******************
	   ARMOR
********************/

// ADD
void Character::addArmor(int upgrade)
{
	this->armor += upgrade;
}
// DENT
void Character::dentArmor(int ding)
{
	this->armor -= ding;
}
// GET
int Character::getArmor()
{
	return this->armor;
}

// SATCHEL (queue)
Queue* Character::getSatchel()
{
	return satchel;
}

// POCKET (Item*)
void Character::setPocket(Item *found)
{
	this->pocket = found;
}

Item* Character::getPocket()
{
	return this->pocket;
}

/******************************************************************
Function Name: iHaveThis()
Description:
	Search function to find indicated item by string 'wanted'
	in a character's pocket first, then their satchel

Returns: true if found, false otherwise

*******************************************************************/
bool Character::iHaveThis(string wanted)
{
	bool found = false;
	// if you have the blaster you get an armor bonus
	if (!this->getSatchel()->isEmpty())
	{
		if (this->getPocket() != nullptr &&
			this->getPocket()->getTitle() == wanted) {
			found = true;
		}
		// search 'satchel' (queue)
		else
		{
			int counter = this->getSatchel()->getQSize();
			do
			{
				if (this->getSatchel()->peekSatchel() == wanted)
				{
					found = true;
					break;
				}

				else { this->getSatchel()->rotateSatchel(); }
				counter--;

			} while (counter != 0);
		}

	}
	return found;
}

// ROLLING : attack
void Character::setLastAttackRoll(int roll)
{
	this->lastAttackRoll = roll;
}

int Character::getLastAttackRoll()
{
	return this->lastAttackRoll;
}

// ROLLING : defense
void Character::setLastDefenseRoll(int roll)
{
	this->lastDefenseRoll = roll;
}

int Character::getLastDefenseRoll()
{
	return this->lastDefenseRoll;
}

/*****************************************************************************
Function: printStats()

Description:
		Helper. Prints recurring updated Statistics Bar for game.
		[called in runGameShell()] Updated each iteration in shell.

 *****************************************************************************/
void Character::printStats()
{
	cout << "   WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
		<< "\n   STATUS: " << this->getName() << " | ABIL: ";

	this->printAbility();

	cout << " | STR: " << this->getStrength()
		<< " | ARM: " << this->getArmor() << " | LAST: ATTK: " << this->getLastAttackRoll()
		<< " DEF: " << this->getLastDefenseRoll()
		<< "\n   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM";
}

/*****************************************************************************
Function: printAbility()
Description:
		Helper. Prints THIS Character's ability as an all-caps string.

 *****************************************************************************/
void Character::printAbility()
{
	switch (this->getAbility())
	{
		case NONE:	cout << "NONE";	break;
		case TOUGH: cout << "TOUGH"; break;
		case DUMB: cout << "DUMB";	break;
		case PILOT: cout << "PILOT"; break;
		case GERMAN: cout << "GERMAN"; break;
		case FRENCH: cout << "FRENCH"; break;
		case MARKSMANSHIP: cout << "MARKSMANSHIP"; break;
		case HUNCH: cout << "HUNCH"; break;
		case SNEAKY: cout << "SNEAKY"; break;
		case RUTHLESS: cout << "RUTHLESS"; break;
		case CHARISMA: cout << "CHARISMA"; break;
		case PRAYER: cout << "PRAYER"; break;
	}

}

/*****************************************************************************
Function Name: postCombatStats()
Description:
		Helper. Provides formatted output of THIS characters'
			name
			ability
			strength
			armor
			pocket (unless empty)
			last attack roll
			last defense roll

 *****************************************************************************/
void Character::postCombatStats()
{
	cout << "\n\n   POST-COMBAT STATUS:"
		 << "\n   ==================="
		 << "\n   Character: " << this->getName()
		 << "\n   Ability: "; this->printAbility();
	cout << "\n   Strength: " << this->getStrength()
		 << "\n   Armor: " << this->getArmor();

	if (this->getPocket() != nullptr) 
	{ 
		cout << "\n   Pocket Contents:\n\n";
		this->getPocket()->printItem(); 
	}

	cout << "\n   Last Attack  Roll: " << this->getLastAttackRoll()
		 << "\n   Last Defense Roll: " << this->getLastDefenseRoll();
	printEnterWipe("============================");
}

/******************************************************************
Function Name: getNewTrophy()
Description:
	Receive an enemy weapon and gain a bonus from it at the same
	time.

Parameters:
	Character* enemy. The loser of some trophy
	string item. The 'title' of the Item* trophy to receive.

Effect:
	New trophy received to THIS character's satchel(queue),
	and weapon bonus added to THIS character's strength parameter

*******************************************************************/
void Character::getNewTrophy(Character* enemy, string item)
{
	cout << "\n   " << this->getName() << " earned a trophy!\n\n   <ENTER>";
	cin.get();
	wipeScreen();

	Item* trophy = enemy->getSatchel()->searchSatchel(item);
	printEnterWipe(" ");
	this->addWeaponBonus(trophy);
}

/******************************************************************
Function Name: findAnyWeapon()
Description:
	Adds a weapon (armor) bonus for situations in which any weapon
	will do. Calls addWeaponBonus() if a weapon is found in the
	character's queue.

Parameters: none

Returns: true if a weapon found, and bonus added, false otherwise
		 prints the previous & current armor

*******************************************************************/
bool Character::findAnyWeapon()
{
	bool found_a_weapon = false;

	if (!this->getSatchel()->isEmpty())
	{
		Item* someItem = nullptr;

		int counter = this->getSatchel()->getQSize();
		do
		{
			someItem = this->getSatchel()->getFront();
			if (someItem->getType() == WEAPON)
			{
				found_a_weapon = true;
				this->addWeaponBonus(someItem); // add bonus
				break;
			}
			else // keep looking
			{
				this->getSatchel()->enqueue(someItem);
				this->getSatchel()->rotateSatchel();
			}
			counter--;

		} while (!found_a_weapon && counter > 0);

	}
	else // empty 'satchel' (queue)
	{
		printEnterWipe("It\'s worse.. no weapon bonus.");
		if (this->getSatchel()->isEmpty())
		{
			printEnterWipe("..and your satchel was empty.");
		}
	}
	return found_a_weapon;
}

/*******************************************************************
Function Name: addWeaponBonus()
Description:
	Adds the health bonus of an Item* wpn to the character's armor
	value. This will help in defense against enemies.
	Checks that item type is enum value WEAPON (SEE: item.hpp)

Parameters: Item* wpn. The item to use.
			NOTE* wpn MUST be dequeued (getFront()) b/f calling.

Effect: wpn enqueued to this character.
		 Character's armor increased if weapon was found.

*******************************************************************/
void Character::addWeaponBonus(Item* wpn)
{
	if (wpn->getType() == WEAPON)
	{
		int prevArmor = this->getArmor();
		int armorBonus = wpn->getPower(); // get this weapon's armor bonus


		this->addArmor(armorBonus);

		// print what weapon you're using
		cout << "\n   ============================================="
			<< "\n   " << this->getName()
			<< " got an weapon bonus from: "
			<< wpn->getTitle()
			<< "\n   ============================================="
			<< "\n        previous armor: " << prevArmor
			<< "\n        current  armor: " << this->getArmor()
			<< "\n   ============================================="
			<< "\n\n   <ENTER>";
		cin.get();
		wipeScreen();

		// put back in your queue
		this->getSatchel()->enqueue(wpn);
	}
}

/*****************************************************************************
Function Name: addNamedWeaponOrAnyWeaponBonus()

Parameters:	
	string 'weaponName'. The string literal of the weapon you are looking for.

Description:
		Attempts to add a weapon bonus according to 'weaponName'.
		If found, will display statistics for that weapon.
		Searches according to enum WEAPON.. NOT HEAVY_WEAPON.

		STEPS:
			1. calls iHaveThis() on 'weaponName' to see if in 'satchel'
			2. If found:
				2b. call searchSatchel() to retrieve from 'satchel' (queue)
				2c. call addWeaponBonus() on retrieved weapon
				2d. set status TRUE (return value)
			3. If absent:
				3b. call findAnyWeapon()
				3c. If found according to type: WEAPON
					3d. add weapon power to armor value
				3e. If no weapons
					3f. print statements to indicate so
			4. return 'status' (bool)

Returns:
		If any bonus added : TRUE 
		otherwise : FALSE

 *****************************************************************************/
bool Character::addNamedWeaponOrAnyWeaponBonus(string weaponName)
{
	bool status = false;

	if (this->iHaveThis(weaponName))
	{
		Item* myWeapon = this->getSatchel()->searchSatchel(weaponName);
		this->addWeaponBonus(myWeapon);
		status = true;

		printEnterWipe("..AND I FOUND IT !");
	}
	else
	{
		printEnterWipe(".. I CAN\'T FIND IT !!");
		printEnterWipe("Ah.. not good. Look for any weapon!");

		// add bonus of ANY weapon available & return true
		if (this->findAnyWeapon()) { status = true; }
	}
	return status;
}

/*****************************************************************************
Function Name: eat()

Description:

	Handles action EAT in game.
	1. Prompts user for yes or no input
	2. Validates input
	3. If yes:
		3b. calls findFoodInSatchel() to find any type: FOOD
			item there. SEE: findFoodInSatchel() definition for details
	4. If no:
		print statement about not hungry

Effect: 
	If a FOOD found in THIS Character's 'satchel' (queue) will add
	healthBonus of FOOD to THIS Character's strength value

 *****************************************************************************/
void Character::eat()
{
	cout << "\n   Still hungry?\n\n   [enter] YES <y|Y> -or- NO <n|N>\n\n   =>";

	char response = checkYesNoEat("There\'s always room for pie right?"
		"\n   Ready to eat?");

	if (toupper(response) == 'Y') { this->findFoodInSatchel(); }
	else
	{
		wipeScreen();
		cout << "\n   No time to eat anyway.\n\n   <ENTER>";
	}
}

/*****************************************************************************
Function: checkYesNoEat()

Parameters:		string prompt. The input buffer data from keyboard.

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

Returns:	single char y/Y or n/N only

*****************************************************************************/
char Character::checkYesNoEat(string prompt)
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

/******************************************************************
Function Name: findFoodInSatchel()

Description:

	Adds a strength (defense) bonus for situations in which any FOOD
	will do. Calls addFoodBonus() if food is found in the
	character's queue. Evaluation of type : FOOD triggers item found.

Effects:	

	1. If found, Character's strength increased by healthBonus of
		food selected.

	2. Prints the previous & current strength.

Returns: 
	if food found and consumed : TRUE
	otherwise : FALSE	 

*******************************************************************/
bool Character::findFoodInSatchel()
{
	bool found_food = false;
	bool didEat = false;
	
	if (!this->getSatchel()->isEmpty())
	{
		Item* someItem = nullptr;

		int counter = this->getSatchel()->getQSize();
		do
		{
			someItem = this->getSatchel()->getFront();
			if (someItem->getType() == FOOD)
			{
				found_food = true;
				wipeScreen();
				cout << "\n   Here\'s something to eat:"
					<< "\n   ========================="
					<< "\n   " << someItem->getTitle()
					<< "\n   Nutritional value: " << someItem->getHealthBonus()
					<< "\n   ============================"
					<< "\n\n   Are you going to eat that?"
					<< "\n\n   [enter] YES <y|Y> -or- NO <n|N>\n\n   =>";

				char response = checkYesNoEat("Some people haven't eaten all day so..."
					"\n   Are you going to eat this?");

				if (toupper(response) == 'Y')
				{
					// add bonus (will delete someItem within)
					this->addFoodBonus(someItem); 
					didEat = true;
					break;
				}
				// not eating this food
				else if (toupper(response) == 'N')
				{ 
					// put it back in 'satchel' for later
					this->getSatchel()->enqueue(someItem); 
					counter++; // have to increment, b/c didn't want that item
					continue;
				}
			}
			// keep looking
			else { this->getSatchel()->enqueue(someItem); }
			counter--;

		} while (!didEat && counter > 0);

	}
	if(!found_food)
	{
		cout << "\n   There\'s no food in your satchel.\n\n   <ENTER>";
	}
	return found_food;
}

/*******************************************************************
Function Name: addFoodBonus()

Parameters:
	Item* food. The item to use.
	NOTE* food MUST be dequeued (getFront()) b/f calling.

Description:
	Adds the health bonus of an Item* food to the character's 
	strength value, used by the Character::defend() function 

	This will help in defense against enemies.
	Checks that item type is enum value FOOD (SEE: item.hpp)

Effect: 
	If found:
	Charcter's strength increased by healthBonus of item.
	Item* food DESTROYED at end (you ate it)

*******************************************************************/
void Character::addFoodBonus(Item* food)
{
	if (food->getType() == FOOD)
	{
		int prevHealth = this->getStrength();
		int healthBonus = food->getHealthBonus(); // get this food's  bonus
		
		// food increases character strength (used in defend)
		this->setStrength((prevHealth + healthBonus));

		// print what weapon you're using
		wipeScreen();
		cout << "\n   ============================================================"
			 << "\n   " << getName() << " got a strength boost from eating: "
			 << food->getTitle()
			 << "\n   ============================================================"
			 << "\n        previous strength: " << prevHealth
			 << "\n        current  strength: " << this->getStrength()
			 << "\n   ============================================================"
			 << "\n\n   <ENTER>";

		// destroy food
		delete food; 
		food = nullptr;
	}
}

//***************************** E O F ********************************


