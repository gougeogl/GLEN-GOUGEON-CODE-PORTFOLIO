/*****************************************************************************
 * File Name: NobleSword.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for NobleSword SUB Class :

 *****************************************************************************/
#include"NobleSword.hpp"

// Constructor
NobleSword::NobleSword()
{
	name = "Nobleman of the Sword";
	ability = TOUGH;
	strength = 8;
	armor = 3;
}

// Destructor
NobleSword::~NobleSword() {}

// ROLL : ATTACK
int NobleSword::rollAttack()
{
	int roll = die.six() + die2.six();

	return  roll;
}

// ROLL : DEFENSE 
int NobleSword::rollDef()
{
	int roll = def.six() + def2.six();

	return roll;
}

// ROLL : INITIATIVE
int NobleSword::rollInitiative()
{
	printEnterWipe("Roll for initiative..");

	// uses 1d8 to roll initiative
	int roll = initiative.eight();
	cout << "\n   " << getName() << " rolled a: " << roll << "\n\n   <ENTER>";
	cin.get();
	wipeScreen();
	return roll;
}

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
int NobleSword::attack(Character *enemy)
{
	int atkRoll = this->rollAttack();

	switch (enemy->getAbility())
	{
		// advantage over the weak minded
		case DUMB: atkRoll += 1; break;
		// penalty for toughness
		case TOUGH: atkRoll -= 1; break;

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
void NobleSword::defend(int harm)
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
		2.	If output of step 1 >= 4 continue
			otherwise print message
		3.  If step 2 successful
				a) roll healing die
				b) restrict maximum outcome to NOBLE_SWORD_MAX_HEALING
				c) add b) to current strength
				d) print previous and current strength

*******************************************************************/
void NobleSword::health()
{
	printEnterWipe("I have combat for the King.. but now I am hurt");
	printEnterWipe("Madamme please help me..");

	if (this->rollInitiative() >= 4)
	{
		int healthBoost = heal.eight();
		if (healthBoost >= NOBLE_SWORD_MAX_HEALING) { healthBoost = NOBLE_SWORD_MAX_HEALING; }

		int prevStrength = this->getStrength();
		int newStrength = (prevStrength + healthBoost);

		this->setStrength(newStrength);

		cout << "\n   " << this->getName() << " received healing:"
			<< "\n   ============================"
			<< "\n   previous strength: " << prevStrength
			<< "\n    current strength: " << this->getStrength();
		 printEnterWipe("============================");
	
	}
	else { printEnterWipe("..\"Huh? she is scared of me.\""); }
}

//***************************** E O F ********************************
