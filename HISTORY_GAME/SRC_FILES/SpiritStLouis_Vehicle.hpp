/*****************************************************************************
 * File Name: SpiritStLouis_Vehicle.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for Spirit of Saint Louis Vehicle SUB Class

 *****************************************************************************/
#ifndef GOUGEON_SPIRIT_SAINT_LOUIS_CLASS
#define GOUGEON_SPIRIT_SAINT_LOUIS_CLASS

#include "VehicleClass.hpp"

class SpiritStLouis : public Vehicle
{
private:
	bool engineIsRunning;
	Die starter;
	bool crashedPlane;
	int structuralIntegrity;
	string color;
	string engineName;
	int hp; // horsepower
	int wingspan; // ft.
	int length_Ft; // length in feet [27]
	int length_In; // length in inches [8]

	// regular wgt [Vehicle BASE CLASS] = Gross wgt
	int empty_wgt; 

	int fuel; // gals.
	Item* compartment;

public:
	SpiritStLouis();
	virtual ~SpiritStLouis();

	string getColor();
	int getWingspan();
	int getLength_Ft();
	int getLength_In();
	int getEmptyWgt();

	// STRUCTURAL INTEGRITY
	int getStructuralIntegrity();
	void damageAirframe(int);
	void repairAirframe(int);

	// ENGINE
	string getEngineName();
	bool startEngine();
	void stall();
	bool isStalled();
	int getHp();
	void damageEngine(int);
	void repairEngine(int);

	// FUEL
	int getFuelLevel();
	void burnFuel();

	// STORAGE COMPARTMENT
	void setInCompartment(Item*);
	Item* getCompartment();
	bool compartmentIsEmpty();

	// PRINTING
	void checkAircraft();
	virtual void showVehicle() override;

};
#endif 

// ********************************* EOF **************************************
