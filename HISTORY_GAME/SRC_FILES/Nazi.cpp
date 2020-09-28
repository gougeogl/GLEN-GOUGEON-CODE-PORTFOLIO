/*****************************************************************************
 * File Name: Nazi.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Nazi SUB Class :

 *****************************************************************************/
#include<iostream>

#include"DieClass.hpp"
#include"CharacterClass.hpp"
#include"Nazi.hpp"

// Constructor
Nazi::Nazi()
{
	ability = DUMB;	
	strength = 8;
	armor = 0;
}

// Destructor
Nazi::~Nazi(){}

// ROLL : ATTACK
int Nazi::rollAttack()
{
	int roll = die.six() + die2.six();

	return  roll;
}

// ROLL : DEFENSE
int Nazi::rollDef()
{
	int roll = def.six() + def2.six();

	return roll;
}

// NO initiative
int Nazi::rollInitiative() { return 0; }

/********************************************************************
Function Name: attack()
Description:
	Performs an attack against an enemy Character 'enemy'

	STEPS:
		1. roll attack
		2. add ability bonus/penalty to atkRoll
		3. call setLastAttackRoll() with output of step 2
		4. call getLastAttackRoll() and return it

 * *****************************************************************/
int Nazi::attack(Character *enemy)
{
	int atkRoll = this->rollAttack();

	switch (enemy->getAbility())
	{
		// penalty for toughness
		case TOUGH: atkRoll -= 2; break;

		// penalty against SITH
		case SNEAKY: atkRoll -= 3; break;

		default: break;
	}

	// restrict negatives
	if (atkRoll < 0) { atkRoll = 0; }

	// set result
	this->setLastAttackRoll(atkRoll);

	// = int harm input to defend()
	return this->getLastAttackRoll();
}

/********************************************************************
Function Name: defend()
Description:
	Defends against 'harm' from an enemy attack

	STEPS:
		1. roll defense
		2. call setLastDefenseRoll() with output of step 1
		3. subtract harm from defRoll
		4. if (harm > 0) then get and store:
			current strength
			current armor
				IF (currentArmor > 0)
						if(harm > currentArmor)
							subtract currentArmor from harm
							(currentArmor - harm)
						call dentArmor() with input harm
				IF (excessHarm > 0)
						if(currentStrength > excessharm)
								subtract excessHarm from currentStrength
								(currentStrength - excessHarm)
						else if(currentStrenght < excessharm)
								set newStrength to -1

				ELSE subtract harm DIRECTLY from currentStrength

				call setStrength() with stored newStrength variable

 * *****************************************************************/
void Nazi::defend(int harm)
{
	// roll defense & record
	int defRoll = this->rollDef();
	this->setLastDefenseRoll(defRoll);

	// harm goes to defensive die first
	harm -= defRoll;

	if (harm > 0) 
	{
		int currentStrength = this->getStrength();
		int currentArmor = this->getArmor();
		int newStrength = 0;
		int excessHarm = 0;

		// damage goes to armor next
		if (currentArmor > 0)
		{
			if (harm > currentArmor)
			{
				excessHarm = harm - currentArmor;
			}
			this->dentArmor(harm);
		}

		// harm goes to strength last
		if (excessHarm > 0)
		{
			// subtraction results in positive
			if (currentStrength > excessHarm)
			{
				newStrength = (currentStrength - excessHarm);
			}
			// subtraction results in negative
			else if ((currentStrength - excessHarm) < 0)
			{
				newStrength = (-1);
			}
		}
		else
		{
			newStrength = currentStrength - harm;
		}

		// update strength
		this->setStrength(newStrength);
	}

}

/******************************************************************
Function Name: health()
Description:
	Potential increase to THIS Character's strength (health)

	STEPS:
		1.	Roll for initiative
		2.	If output of step 1 equals TWO continue
		3.  If step 2 successful
				a) print dialogue
				b) increment current strength by TWO

*******************************************************************/
void Nazi::health()
{
	if (die.six() == 2)
	{
		cout << "\n   Trooper: " << this->getName() << " aww MEDIC!!";
		this->setStrength((this->getStrength() + 2));
	}
}

//***************************** E O F ********************************
