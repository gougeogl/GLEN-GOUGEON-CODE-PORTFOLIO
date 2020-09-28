/*****************************************************************************
 * File Name: SpiritStLouis.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Spirit of Saint Louis Vehicle SUB Class

 *****************************************************************************/
#include "SpiritStLouis_Vehicle.hpp"

SpiritStLouis::SpiritStLouis()
{
	name = "Spirit of Saint Louis";
	OEM = "Ryan Airlines Corporation";
	model = "Ryan M-2 (modified)";
	engineName = "Wright Whirlwind J-5C";
	hp = 223;
	wingspan = 46;
	length_Ft = 27;
	length_In = 8;
	classification = TRANSPORT;
	crewSize = 1;
	pilot = nullptr;
	color = "Silver";
	speed = 120; /* mph */
	weight = 5135; /* pounds*/
	empty_wgt = 2150;
	fuel = 450;
	value = 6000.00; /* thousand credits*/
	structuralIntegrity = 100;
	engineIsRunning = false;
	crashedPlane = false;
}

SpiritStLouis::~SpiritStLouis() 
{
	if(!compartmentIsEmpty())
	{
		Item* EmerKit = getCompartment();
		delete EmerKit;
		EmerKit = nullptr;
	}
}

/*********************************
  LOCAL MEMBER GETTERS & SETTERS
**********************************/

// COLOR
string SpiritStLouis::getColor() { return this->color; }

// WINGSPAN
int SpiritStLouis::getWingspan() { return wingspan; }

// LENGTH
int SpiritStLouis::getLength_Ft() { return length_Ft; }
int SpiritStLouis::getLength_In() { return length_In; }

// EMPTY WGT
int SpiritStLouis::getEmptyWgt() { return empty_wgt; }

/************************
  STRUCTURAL INTEGRITY
*************************/
// get
int SpiritStLouis::getStructuralIntegrity() { return structuralIntegrity; }
// damage (reduce integrity)
void SpiritStLouis::damageAirframe(int damage) { structuralIntegrity -= damage; }
// repair (increase integrity)
void SpiritStLouis::repairAirframe(int repair) { structuralIntegrity += repair; }

/*************
	ENGINE
**************/

// GET Engine Name
string SpiritStLouis::getEngineName() { return engineName; }

/***********************************************************
(helper) Function: startEngine()

Description:
	Calls getFuelLevel() to check if fuel in plane.
	If not, prints "You're out of fuel!" Otherwise:
		1. call burnFuel() [burns 5 gals.]
		2. print out sound effects
		3. call wipeScreen()
		4. 'crank' engine [roll 1d6]
		5. evaluate int crank
			even: stop
			odd : try again

Effect:
	If an even crank occurred, set 'engineIsRunning'
	member to TRUE. Then return TRUE

	If out of fuel: return FALSE

************************************************************/
bool SpiritStLouis::startEngine()
{
	int crank = 0;

	// must have fuel to crank engine
	if (getFuelLevel() > 0)
	{
		// even rolls start engine
		do
		{
			// subtracts 5 gals. each time
			burnFuel();
			cout << "\n   puh ~ puh ~ puh ~ PUH ~"; 
			wipeScreen();

			crank = starter.six();

		} while (getFuelLevel() > 0 && (crank % 2 != 0));

		// assert was even crank
		if (crank % 2 == 0) { engineIsRunning = true; }

	}
	if(getFuelLevel() < 1)
	{
		printEnterWipe("You\'re out of fuel!");
		engineIsRunning = false;
	}

	return engineIsRunning;
}

/*******************************************************
(helper) Function: stall()

Description: Sets 'engineIsRunning' member to FALSE.

********************************************************/
void SpiritStLouis::stall() { engineIsRunning = false; }

/*******************************************************
(validator) Function: isStalled()

Description: 
	Evaluates the status of 'engineIsRunning' member.

	   Truth Table:
	=================
	running | stalled
	-----------------
	 TRUE   |  FALSE
	-----------------
	 FALSE  |  TRUE

********************************************************/
bool SpiritStLouis::isStalled()
{ 
	bool status = false;
	if(engineIsRunning == false) { status = true; }
	return status;
}

// ENGINE-HORSE-POWER
int SpiritStLouis::getHp() { return hp; }

// damage Engine (decrease horse-power [hp] )
void SpiritStLouis::damageEngine(int damage) { hp -= damage; }

// repair Engine (increase horse-power [hp] )
void SpiritStLouis::repairEngine(int repair) { hp += repair; }

/********
  FUEL
*********/
// GET
int SpiritStLouis::getFuelLevel() { return fuel; }
// DECREMENT BY 5
void SpiritStLouis::burnFuel() { fuel -= 5; }

/************************************************
			STORAGE COMPARTMENT
*************************************************/
// SET
void SpiritStLouis::setInCompartment(Item* stuff)
{
	compartment = stuff;
}
// GET
Item* SpiritStLouis::getCompartment()
{
	return compartment;
}

bool SpiritStLouis::compartmentIsEmpty()
{
	bool result = false;

	if (compartment == nullptr) { result = true; }

	return result;
}

/***********************************
Function: checkAircraft()

Description:
	Prints small table of:
	-engine ON/OFF
	-hp [horse-power]
	-fuel level
	-structural integrity

************************************/
void SpiritStLouis::checkAircraft()
{
	printEnterWipe("Inspect aircraft.");

	cout << "\n   =========="
		<< "\n   | STATUS |"
		<< "\n   ================================";

	switch (isStalled())
	{
	case false: cout << "\n   | engine : ON |"; break;
	case true: cout << "\n   | engine : OFF |"; break;
	}
	cout << "\n   --------------------------------"
		<< "\n   | current hp: " << getHp() << " |"
		<< "\n   --------------------------------"
		<< "\n   | fuel: " << getFuelLevel() << " |"
		<< "\n   --------------------------------"
		<< "\n   | struct. integrity: " << getStructuralIntegrity() << " |";
	printEnterWipe("================================");
}

// PRINT ST. LOUIS STATS
void SpiritStLouis::showVehicle()
{
	cout << "\n\n   ================================================"
		<< "\n   Name: " << this->getVehicleName()
		<< "\n   O.E.M.: " << this->getOEM();

	this->printClassification(this->getClassification());

	cout << "\n   Model: " << this->getModel()
		<< "\n   Color: " << this->getColor()
		<< "\n   Engine: " << getEngineName()
		<< "\n   Horse-power: " << getHp()
		<< "\n   Wingspan: " << getWingspan()
		<< "\n   Length: " << getLength_Ft() << " ft., " << getLength_In() << " in."
		<< "\n   Gross Wgt.: " << this->getWeight()
		<< "\n   Empty Wgt.: " << getEmptyWgt()
		<< "\n   Fuel (gals.): " << getFuelLevel()
		<< "\n   Max Speed (max load & sea-level): " << this->getSpeed();

	// print pilot
	if (this->getPilot() != nullptr) { cout << "\n   Pilot: " << this->getPilot()->getName(); }

	cout << "\n   Crew-size: " << this->getSizeCrew();
	cout << "\n   Cost ($US Dollars): ";
	cout.precision(2);
	cout << std::fixed << this->getValue();
	printEnterWipe("================================================");
}

// ********************************* EOF **************************************
