/*****************************************************************************
 * File Name: VehicleClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	 Definitions for Vehicle BASE Class :

 *****************************************************************************/
#include "VehicleClass.hpp"

Vehicle::Vehicle()
{
	name = " ";
	OEM = " ";
	model = " ";
	classification = TRANSPORT;
	crewSize = 1;
	pilot = nullptr;
	speed = MIN_SPEED;
	weight = MIN_WEIGHT;
	value = MIN_MSRP;

}

Vehicle::~Vehicle() {}

// VEHICLE NAME
string Vehicle::getVehicleName()
{
	return this->name;
}

void Vehicle::setVehicleName(string callItThis)
{
	this->name = callItThis;
}

// OEM (original equiptment manufacturer)
string Vehicle::getOEM()
{
	return this->OEM; 
}

void Vehicle::setOEM(string brand)
{
	this->OEM = brand;
}

// MODEL
string Vehicle::getModel()
{
	return this->model;
}
void Vehicle::setModel(string name)
{
	this->model = name;
}

/******************
   CLASSIFICATION
*******************/

// GET
VehType Vehicle::getClassification()
{
	return this->classification;
}

// SET
void Vehicle::setClassification(VehType specs)
{
	this->classification = specs;
}

// PRINT 
void Vehicle::printClassification(VehType type)
{
	switch (type)
	{
	case UNKNOWN: cout << "\n   Class: UNKNOWN"; break;

	case TRANSPORT: cout << "\n   Class: TRANSPORT"; break;

	case CARGO: cout << "\n   Class: CARGO"; break;
	}
}

// CREW SIZE
int Vehicle::getSizeCrew()
{
	return this->crewSize;
}
void Vehicle::setSizeCrew(int size) // default 1
{
	this->crewSize = size;
}

// PILOT
Character* Vehicle::getPilot()
{
	return this->pilot;
}
void Vehicle::setPilot(Character* captain)
{
	this->pilot = captain;
}

// SPEED
double Vehicle::getSpeed()
{
	return this->speed;
}
void Vehicle::setSpeed(double howFast)
{
	this->speed = howFast;
}

// WEIGHT
double Vehicle::getWeight()
{
	return this->weight;
}
void Vehicle::setWeight(double mass)
{
	this->weight = mass;
}

// COST
double Vehicle::getValue()
{
	return this->value;
}
void Vehicle::setValue(double price)
{
	this->value = price;
}

//***************************** E O F ********************************
