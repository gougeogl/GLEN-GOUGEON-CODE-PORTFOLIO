/*****************************************************************************
 * File Name: Carriage_Vehicle.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for Carriage Vehicle SUB Class

 *****************************************************************************/
#ifndef GOUGEON_CARRIAGE_VEHICLE_CLASS
#define GOUGEON_CARRIAGE_VEHICLE_CLASS

#include "VehicleClass.hpp"
#include "LouisXVI.hpp"
#include "MarieAntoinette.hpp"
#include "PalaceGuards.hpp"
#include "HouseWife.hpp"

enum SEATING {KING, QUEEN, PRINCE, PRINCESS, BODYGUARD1, BODYGUARD2};

class Carriage : public Vehicle
{
	private:
		LouisXVI* king;
		LouisXVI* prince;
		MarieAntoinette* queen;
		MarieAntoinette* princess;

		PalaceGuards* bodyguard1;
		PalaceGuards* bodyguard2;
		HouseWife* escort;

		int passengerSize;

	public:
		Carriage();
		virtual ~Carriage();

		bool isEmpty();
		void seatKing(LouisXVI*, enum SEATING);
		void seatQueen(MarieAntoinette*, enum SEATING);
		void seatGuard(PalaceGuards*, enum SEATING);
		void seatEscort(HouseWife*);

		/******************************************
			GETTERS: *note* enumSeating is
			according to sex. Use remove King for
			male Bourbons. Use remove Queen for
			female Bourbons.
		*******************************************/
		LouisXVI* removeKing(enum SEATING);
		MarieAntoinette* removeQueen(enum SEATING);
		PalaceGuards* removeGuard(enum SEATING);

		// Mme. Tourzel
		HouseWife* removeEscort();

		void printManifest();

		// pure virtual function	
		virtual void showVehicle();

};
#endif 

// ********************************* EOF **************************************
