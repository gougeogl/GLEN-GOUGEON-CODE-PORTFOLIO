/*****************************************************************************
 * File Name: Carriage_Vehicle.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Carriage Vehicle SUB Class

 *****************************************************************************/
#include "Carriage_Vehicle.hpp"

Carriage::Carriage()
{
	name = "Royal Carriage";
	classification = TRANSPORT;
	crewSize = 4;

	pilot = nullptr;
	king = nullptr;
	queen = nullptr;
	prince = nullptr;
	princess = nullptr;
	bodyguard1 = nullptr;
	bodyguard2 = nullptr;
	escort = nullptr;

	speed = 25.00;
	weight = 1500;
	value = 5000;

	// specific to this cart
	passengerSize = 4;
}

Carriage::~Carriage() {}

/**************************************************
Function: isEmpty()

Description:
	Evaluates if the carriage is empty by checking
	the pointers of each game Character type is
	nullptr or not.

***************************************************/
bool Carriage::isEmpty()
{
	bool status = false;

	if (
		king == nullptr &&
		queen == nullptr &&
		prince == nullptr &&
		princess == nullptr &&
		bodyguard1 == nullptr &&
		bodyguard2 == nullptr &&
		escort == nullptr)
	{
		status = true;
	}

	return status;
}

/***********************************
  SEATING CHARACTERS INTO CARRIAGE
************************************/

/**********************************************************
Function: seatKing()

Paramters:
	LouisXVI* louis. The male Bourbon royal to place in
	the appropriate seat (pointer to LouisXVI)

	enum SEATING seat. The seat to place 'louis' in.

Description:
	Places 'louis' according to 'seat'. Think of the
	Prince as a little louis. 

	Seating Options: [king, prince]

***********************************************************/
void Carriage::seatKing(LouisXVI* louis, enum SEATING seat) 
{ 
	if (seat == KING) { king = louis; }
	else if (seat == PRINCE) { prince = louis; }
}

/***************************************************************
Function: seatQueen()

Paramters:
	MarieAntoinette* marie. The female Bourbon royal to 
	place in the appropriate seat (pointer to MarieAntoinette)

	enum SEATING seat. The seat to place 'marie' in.

Description:
	Places 'marie' according to 'seat'. Think of the
	Princess as a little marie.

	Seating Options: [queen, princess]

****************************************************************/
void Carriage::seatQueen(MarieAntoinette* marie, enum SEATING seat) 
{ 
	if (seat == QUEEN) { queen = marie; }
	else if (seat == PRINCESS) { princess = marie; }
}

/***************************************************************
Function: seatGuard()

Paramters:
	PalaceGuards* guard. The guard Character
	place in the appropriate seat (pointer to PalaceGuards)

	enum SEATING seat. The seat to place 'marie' in.

Description:
	Places 'guard' according to 'seat'. 

	Seating Options: [bodyguard1, bodyguard2]

****************************************************************/
void Carriage::seatGuard(PalaceGuards* guard, enum SEATING seat)
{
	switch (seat)
	{
		case BODYGUARD1: bodyguard1 = guard; break;
		case BODYGUARD2: bodyguard2 = guard; break;
		default: break;
	}
}

/****************************************************************
Function: seatEscort()

Description:
	Places the HouseWife* nanny in the escort's seat in THIS
	carriage. She escorts the Royal children from place to place.

*****************************************************************/
void Carriage::seatEscort(HouseWife* nanny) { escort = nanny; }

/*****************************************************
REMOVING CHARACTERS INTO CARRIAGE PERTAINING TO:

Functions:
	removeKing()
	removeQueen()
	removeGuard()
	removeEscort()

Description:
  The following 4 functions operate similarly.
  an enum SEATING param. 'seat' will match
  the appropriate seat desired and return a 
  pointer to that game Character. The escort
  requires no parameter.

Returns:
	Pointer to the appropriate game Character.

******************************************************/

LouisXVI*::Carriage::removeKing(enum SEATING seat) 
{ 
	LouisXVI* maleToReturn = nullptr;

	switch (seat)
	{
		case KING: maleToReturn = king; break;
		case PRINCE: maleToReturn = prince; break;
		default: break;
	}
	return maleToReturn;
}

MarieAntoinette*::Carriage::removeQueen(enum SEATING seat)
{
	MarieAntoinette* femaleToReturn = nullptr;

	switch (seat)
	{
		case QUEEN: femaleToReturn = queen; break;
		case PRINCESS: femaleToReturn = princess; break;
		default: break;
	}
	return femaleToReturn;
}

PalaceGuards*::Carriage::removeGuard(enum SEATING seat)
{
	PalaceGuards* guardToSend = nullptr;

	switch (seat)
	{
		case BODYGUARD1: guardToSend = bodyguard1; break;
		case BODYGUARD2: guardToSend = bodyguard2; break;
		default: break;
	}

	return guardToSend;
}

HouseWife*::Carriage::removeEscort() { return escort; }

/*****************************************************************
(helper) Function: printManifest()

Description:
	Provides a list of seated game Characters in THIS Carriage.
	Checks that a seat is not empty before attempting to print
	their name.

******************************************************************/
void Carriage::printManifest()
{
	cout << "\n   Now loaded into the Carriage are:"
		 << "\n   =================================="
		 << "\n   Passenger Manifest:";
	if (king != nullptr) { cout << "\n   " << king->getName(); }
	if (queen != nullptr) { cout << "\n   " << queen->getName(); }
	if (prince != nullptr) { cout << "\n   " << prince->getName(); }
	if (princess != nullptr) { cout << "\n   " << princess->getName(); }
	if (bodyguard1 != nullptr) { cout << "\n   " << bodyguard1->getName(); }
	if (bodyguard2 != nullptr) { cout << "\n   " << bodyguard2->getName(); }
	if (escort != nullptr) { cout << "\n   " << escort->getName(); }

	cout << "\n   ==================================\n";
}

/******************************************************************************
(Pure Virtual) Function: showVehicle()

Description:
	Displays THIS vehicle's stats including:
		-vehicle name -classification -crew size -pilot's name 
		-# of passengers -manifest of passengers -speed -weight -value

*******************************************************************************/
void Carriage::showVehicle()
{
	cout << "\n\n   =============================="
		<< "\n   Name: " << getVehicleName();

	printClassification(getClassification());

	cout << "\n   Crew: " << getSizeCrew();

	if (pilot != nullptr) { cout << "\n   Driver: " << getPilot()->getName(); }

	cout << "\n   Max. Passengers: " << passengerSize;

	// print passengers if carriage not empty
	if (!isEmpty()) { printManifest(); }

	cout << "\n   Speed: " << getSpeed()
		<< "\n   Weight: " << getWeight()
		<< "\n   Value: " << getValue();
	printEnterWipe("==============================");

}

// ********************************* EOF **************************************
