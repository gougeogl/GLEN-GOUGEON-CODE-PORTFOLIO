/*****************************************************************************
 * File Name: FishingBoat_Vehicle.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for Fishing Boat Vehicle SUB Class

 *****************************************************************************/
#ifndef GOUGEON_FISHING_BOAT_CLASS
#define GOUGEON_FISHING_BOAT_CLASS

#include "VehicleClass.hpp"

class FishingBoat : public Vehicle
{
	private:
		string color;
		int passengers;
	public:
		FishingBoat();
		virtual ~FishingBoat();

		string getColor();

		// PRINT BOAT STATS
		virtual void showVehicle() override;
};
#endif

// ********************************* EOF **************************************
