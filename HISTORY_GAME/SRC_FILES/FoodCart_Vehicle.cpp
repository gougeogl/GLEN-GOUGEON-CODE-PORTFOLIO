/*****************************************************************************
 * File Name: FoodCart_Vehicle.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Food Cart Vehicle SUB Class.

	The 'bed' of the Food Cart is essentially a stack data structure.
	Think of the tailgate as the top of the stack. I chose to implement
	a separate stack here for naming purposes.

 *****************************************************************************/
#include "FoodCart_Vehicle.hpp"

FoodCart::FoodCart()
{
	name = "cart";
	OEM = "His Majesty\'s cart maker.";
	model = "food";
	classification = TRANSPORT;
	crewSize = 1;
	pilot = nullptr;
	speed = MIN_SPEED;
	weight = MIN_WEIGHT;
	value = MIN_MSRP;

	// specific to this cart
	bedCount = 0;
}

FoodCart::~FoodCart() {}

/***************************************************
Function: isBedEmpty()
Description:
	Validates if the bed of the Food Cart is empty.

*****************************************************/
bool FoodCart::isBedEmpty()
{
	bool result = true;

	if (getBedCount() > 0) { result = false; }

	return result;
}

// Returns the number of items in the bed of the cart
int FoodCart::getBedCount()
{
	return bedCount;
}

// Decrements the bedCount then returns
// an item from the back of the bed
Item* FoodCart::popBed()
{
	Item* thing = nullptr;

	bedCount--;
	thing = bed[bedCount];
	bed[bedCount] = nullptr;

	return thing;
}

// Places an item at the back of the bed
// and increments the bedCount
void FoodCart::pushBed(Item* thing)
{
	if (bedCount < BED_SIZE)
	{
		bed[bedCount] = thing;
		bedCount++;
	}
	else
	{
		cout << "\n   Sorry. The bed is already full.\n";
	}
}

// Displays the content of items currently in the bed
void FoodCart::printBed()
{
	cout << "\n   " << this->getModel() << " cart BED CONTENTS:";
	for (int i = 0; i < bedCount; i++)
	{
		bed[i]->printItem();
	}
	printEnterWipe(" ");
}

/***********************************************
Function: dumpBed()

Description:
	Removes and cleans up memory for all Items
	in the bed of carriage. Repeatedly calls
	popBed() to remove each item, and decrements
	the bedCount.

************************************************/
void FoodCart::dumpBed()
{
	Item* junk = nullptr;

	if (getBedCount() > 0)
	{
		junk = popBed();
		delete junk;
		junk = nullptr;
		bedCount--;
	}
}

/****************************************************
(Pure Virtual) Function: showVehicle()

Description:
	Displays statistics for THIS vehicle including:
	-name -model -classification -crew -pilot's name
	-speed -weight -value in U.S. $

*****************************************************/
void FoodCart::showVehicle()
{
	cout << "\n\n   =============================="
		<< "\n   Name: " << getVehicleName()
		<< "\n   Model: " << getModel()
		<< "\n   Class: ";
	printClassification(getClassification());
	cout << "\n   Crew: " << getSizeCrew()
		<< "\n   Pilot: " << getPilot()->getName()
		<< "\n   Speed: " << getSpeed()
		<< "\n   Weight: " << getWeight()
		<< "\n   Value: " << getValue()
		<< "\n   ==============================";
	printEnterWipe(" ");
}

// ********************************* EOF **************************************
