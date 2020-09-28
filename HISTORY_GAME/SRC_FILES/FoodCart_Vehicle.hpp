/*****************************************************************************
 * File Name: FoodCart_Vehicle.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for Food Cart Vehicle SUB Class

 *****************************************************************************/
#ifndef GOUGEON_FOOD_CART_VEHICLE_CLASS
#define GOUGEON_FOOD_CART_VEHICLE_CLASS

#include "VehicleClass.hpp"
#define BED_SIZE 10

class FoodCart : public Vehicle
{
	private:
		Item* bed[10];
		int bedCount;

	public:
		FoodCart();
		virtual ~FoodCart();

		bool isBedEmpty();
		int getBedCount();
		Item* popBed();
		void pushBed(Item*);
		void printBed();
		void dumpBed();

		// pure virtual functions	
		virtual void showVehicle();

};
#endif 

// ********************************* EOF **************************************
