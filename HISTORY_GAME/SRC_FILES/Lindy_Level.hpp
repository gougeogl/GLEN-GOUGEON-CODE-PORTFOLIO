/*****************************************************************************
 * File Name: Lindy_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Lindy (SUB) Level.

	This re-enacts Charles Lindbergh's historic solo flight from NYC-->Paris
	in 1927. There are 21 checkPoint() functions for each entry in the flight
	log of "Lucky Lindy" as he was called. Successful landing in Paris earns
	food, a precious document, a torch, and potential healing. 

 *****************************************************************************/
#ifndef GGOUGEON_LINDY_LEVEL_HPP
#define GGOUGEON_LINDY_LEVEL_HPP

#include"LevelClass.hpp"

#define MAX_HOURS 34
#define MAX_WIND_SPEED 125
#define MAX_ALTITUDE 11000
#define MIN_ALTITUDE 1

enum Direction {N, NW, NE, E, SE, S, SW, W};
enum AM_PM {AM, PM};

class Lindy : public Level
{
	private:
		bool introRead;
		bool hasSupplies;
		
		struct Time
		{
			int hours;
			int minutes;
			enum AM_PM zone;

			Time(int hrs, int mins, enum AM_PM zn) { hours = hrs; minutes = mins; zone = zn; }
			~Time() {}
		};

		//Time* now;

		struct Coordinates
		{
			int flightHrs;
			Time* now = nullptr;
			string landmass1;
			string landmass2;
			string waterBody;

			int altitude;
			int airspeed;
			int windspeed;
			Direction direction;
			string notes;

			Coordinates(
				int flyHrs,
				int hr, int min, AM_PM zn,
				string land1, 
				string land2, 
				string water, 
				int alt, 
				int speed, 
				int wind, 
				Direction way, 
				string scribble)
			{
				flightHrs = flyHrs;
				now = new Time(hr, min, zn);
				landmass1 = land1;
				landmass2 = land2;
				waterBody = water;
				altitude = alt;
				airspeed = speed;
				windspeed = wind;
				direction = way;
				notes = scribble;
			}

			~Coordinates(){ delete now; now = nullptr;}
		};

		Coordinates* location;


	public:
		Lindy();
		virtual ~Lindy();

		Lindy::Coordinates* getLocation();

		// FLIGHT HOURS
		int getFlightHours();

		// LAND
		string getLandMass(int);

		// WATER
		string getWaterBody();

		// ALTITUDE
		int getAltitude();

		// AIR SPEED
		int getAirSpeed();

		// WIND
		int getWindSpeed();

		// DIRECTION
		string getDirection();

		// NOTES
		void printNotes();

		// TIME
		int getHour();
		int getMin();
		string getZone();
		void printTime();

		// FLIGHT LOG
		void updateLog(
			int newHrs,
			int hr, int min, AM_PM zn,
			string land1,
			string land2,
			string water,
			int alt,
			int speed,
			int wind,
			Direction,
			string myNotes
		);

		void printLog();

		// virtual function	
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void lindyIntro();
		bool isPrepared(Character*);
		bool hasHat(Character*);
		bool hasGoggles(Character*);
		bool hasEmergencyGear(Character*);

		// wrapper for checkPoints()
		void flyToParis(SpiritStLouis*);

		// CHECK-POINTS FOR NYC - PARIS
		void checkPoint1(SpiritStLouis*);
		void checkPoint2(SpiritStLouis*);
		bool checkPoint3(SpiritStLouis*);
		bool checkPoint4(SpiritStLouis*);
		bool checkPoint5(SpiritStLouis*);
		bool checkPoint6(SpiritStLouis*);
		bool checkPoint7(SpiritStLouis*);
		bool checkPoint8(SpiritStLouis*);
		bool checkPoint9(SpiritStLouis*);
		bool checkPoint10(SpiritStLouis*);
		bool checkPoint11(SpiritStLouis*);
		void checkPoint12(SpiritStLouis*);
		void checkPoint13(SpiritStLouis*);
		bool checkPoint14(SpiritStLouis*);
		bool checkPoint15(SpiritStLouis*);
		void checkPoint16(SpiritStLouis*);
		bool checkPoint17(SpiritStLouis*);
		bool checkPoint18(SpiritStLouis*);
		bool checkPoint19(SpiritStLouis*);
		void checkPoint20(SpiritStLouis*);
		bool checkPoint21(SpiritStLouis*);

		// CHECKPOINT HELPERS

		// validator for checkPoint() functions
		bool stillFlying(SpiritStLouis*);

		void makeRepairs(SpiritStLouis*);

		// checkPoint10() helper
		bool survivedElectrocution(SpiritStLouis*);

		// checkPoint21() helper
		bool landPlane(SpiritStLouis*);

};
#endif 

//***************************** E O F ********************************
