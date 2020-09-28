/*****************************************************************************
 * File Name: PalaceGuards.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Palace Guards SUB Class :

 *****************************************************************************/
#include "PalaceGuards.hpp"

// Constructor
PalaceGuards::PalaceGuards()
{
	name = "Guard";
	ability = DUMB;
	strength = 9;
	armor = 4;
}

// Destructor
PalaceGuards::~PalaceGuards() {}

// ROLL : ATTACK
int PalaceGuards::rollAttack()
{
	int roll = die.eight();

	return  roll;
}

// ROLL : DEFENSE
int PalaceGuards::rollDef()
{
	int roll = def.six();

	return roll;
}

// NO initiative
int PalaceGuards::rollInitiative() { return 0; }

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
int PalaceGuards::attack(Character *enemy)
{
	int atkRoll = this->rollAttack();

	switch (enemy->getAbility())
	{
		// penalty for toughness
		case TOUGH: atkRoll -= 1; break;
		// penalty against SITH

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

 *******************************************************************/
void PalaceGuards::defend(int harm)
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

// NO health
void PalaceGuards::health() {}

//***************************** E O F ********************************
