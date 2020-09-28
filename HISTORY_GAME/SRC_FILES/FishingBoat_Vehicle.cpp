/*****************************************************************************
 * File Name: FishingBoat.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Fishing Boat Vehicle SUB Class

 *****************************************************************************/
#include "FishingBoat_Vehicle.hpp"

FishingBoat::FishingBoat()
{
	name = "Fishing Boat";
	OEM = "England";
	model = "Eastern-Rig";
	classification = TRANSPORT;
	crewSize = 6;
	passengers = 4;
	pilot = nullptr;
	color = "White";
	speed = 10; /* mph */
	weight = 2500; /* pounds*/
	value = 12500.00; /* $US dollars*/

}

FishingBoat::~FishingBoat() {}

// no color in BASE Vehicle Class
string FishingBoat::getColor() { return this->color; }

/***************************************************************
(Pure Virtual) Function: showVehicle()

Description:
	Displays information for THIS vehicle's
	-name -OEM -model -classification -crew size -pilot's name
	-boat's color -speed -weight -value in U.S. $

****************************************************************/
void FishingBoat::showVehicle()
{
	cout << "\n\n   ============================================"
		<< "\n   Name: " << this->getVehicleName()
		<< "\n   O.E.M.: " << this->getOEM()
		<< "\n   Model: " << this->getModel();

	this->printClassification(this->getClassification());

	cout << "\n   Crew: " << this->getSizeCrew();

	// print pilot
	if (this->getPilot() != nullptr) { cout << "\n   Pilot: " << this->getPilot()->getName(); }
	cout << "\n   Color: " << this->getColor();
	cout << "\n\n   Max Speed: " << this->getSpeed()
		<< "\n   Weight: " << this->getWeight()
		<< "\n   Cost ($US Dollars): ";
	cout.precision(2);
	cout << std::fixed << this->getValue();
	printEnterWipe("============================================");
}

// ********************************* EOF **************************************
