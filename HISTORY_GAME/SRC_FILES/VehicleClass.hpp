/*****************************************************************************
 * File Name: VehicleClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	 Specification for Vehicle BASE Class :

	The Vehicle BASE Class has four SUB classes in this game:
		-FoodCart	-Carriage	-SpiritStLouis	-FishingBoat

	Some are used only for printing their statistics for gameplay, but
	most have some special functionality. The FlightOfKing_Level for 
	example carries 7 passengers, which after loading are passed around as
	parameters to functions that mimick transportation. 

	Each Vehicle includes:
		name. The name of the vehicle
		OEM. The manufacturer (original equiptment manufacturer)
		model. The specific model of vehicle
		classification. The class type if existing
		crewsize. A tracker used for a tally and printing (depending)
		pilot. The driver/captain/pilot of the vehicle [Character*]
		speed. Used for printing only
		weight. Used for printing only
		value. Used for printing only

		* Certain members, as with Item* objects, are used only for
		  printing purposes, though they may be used in further development

 *****************************************************************************/
#ifndef GOUGEON_VEHICLE_BASE_CLASS
#define GOUGEON_VEHICLE_BASE_CLASS

#include<iostream>
using std::cout;
using std::endl;

#include<string>
using std::string;

#include"item.hpp"
#include"stack.hpp"
#include"DieClass.hpp"

//#include "LevelClass.hpp"
#include "CharacterClass.hpp"
#include "DieClass.hpp"

#define MIN_WEIGHT 0.1

#define MAX_STR_INTEGRITY 100
#define MIN_STR_INTEGRITY 1

#define MIN_MSRP 1.00 // $US
#define MIN_SPEED 1.00 // mph

enum VehType {UNKNOWN = 1, TRANSPORT, CARGO};

class Vehicle
{
protected:
	string name;
	string OEM;
	string model;
	VehType classification;
	int crewSize;
	Character* pilot;
	double speed;
	double weight;
	double value;

public:
	Vehicle();
	~Vehicle();

	// VEHICLE NAME
	string getVehicleName();
	void setVehicleName(string);

	// OEM
	string getOEM();
	void setOEM(string);

	// MODEL
	string getModel();
	void setModel(string);

	// CLASSIFICATION
	VehType getClassification();
	void setClassification(VehType);
	void printClassification(VehType);

	// CREW SIZE
	int getSizeCrew();
	void setSizeCrew(int = 1); // default 1

	// PILOT
	Character* getPilot();
	void setPilot(Character*);

	// SPEED
	double getSpeed();
	void setSpeed(double);

	// WEIGHT
	double getWeight();
	void setWeight(double);

	// COST
	double getValue();
	void setValue(double);

	// PRINT VEHICLE
	virtual void showVehicle() = 0;

};
#endif 

//***************************** E O F ********************************
