/*****************************************************************************
 * File Name: Lindy_Level.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Lindy (SUB) Level.

	SEE: Lindy_Level.hpp for Level description.

 *****************************************************************************/
#include"Lindy_Level.hpp"

// Default Constructor
Lindy::Lindy()
{
	this->up = nullptr;
	this->down = nullptr;
	this->left = nullptr;
	this->right = nullptr;

	this->isDiscovered();

	// fill level with items
	itemStack->push(myInventory->whiskey());
	itemStack->push(myInventory->bugs());
	itemStack->push(myInventory->aviatorHat());
	itemStack->push(myInventory->goggles());
	itemStack->push(myInventory->emergencyKit());

	// Untouchables Office
	this->setLocationName("\n   Roosevelt Field. Long Island, New York\n");
	this->setDate("May", 20, 1927);
	this->setDescription("\n   Dirt airstrip. Location of Charles Lindbergh\'s solo flight across the Atlantic.\n");

	string explanation =
		"\n   Raymond Orteig solicited a re-newed prize of $25,000 to any pilot who could travel from"
		"\n   Paris to New-York non-stop or vice-versa. Charles Lindbergh was an experienced Barn-Stormer"
		"\n   and Air-Mail pilot by that time, with some 2,000 hours under his belt. Pilots around the globe"
		"\n   were in competition to be the first to cross the Atlantic, write their name in the history books,"
		"\n   and claim the prize. Lindbergh was the first, he did it solo, and in an experimental aircraft.";

	this->setSignificance(explanation);

	introRead = false;
	hasSupplies = false;
	location = nullptr;
}

// Destructor
Lindy::~Lindy() 
{ 
	// Coordinate
	if(location != nullptr)
	{
		delete location; 
		location = nullptr; 
	}
}

// COORDINATES -- LOCATION
Lindy::Coordinates*::Lindy::getLocation() { return location; }

// FLIGHT HOURS
int Lindy::getFlightHours() { return location->flightHrs; }

// LAND
string Lindy::getLandMass(int num)
{
	string landToReturn = " ";

	switch (num)
	{
	case 1: landToReturn = location->landmass1; break;
	case 2: landToReturn = location->landmass2; break;
	}

	return landToReturn;
}

// WATER
string Lindy::getWaterBody() { return location->waterBody; }

// ALTITUDE
int Lindy::getAltitude() { return location->altitude; }

// AIR SPEED
int Lindy::getAirSpeed() { return location->airspeed; }

// WIND SPEED
int Lindy::getWindSpeed() { return location->windspeed; }

// DIRECTION
string Lindy::getDirection()
{
	string directionToReturn = " ";

	switch (location->direction)
	{
	case N: directionToReturn = "N"; break;
	case NW: directionToReturn = "NW"; break;
	case NE: directionToReturn = "NE"; break;
	case E: directionToReturn = "E"; break;
	case SE: directionToReturn = "SE"; break;
	case S: directionToReturn = "S"; break;
	case SW: directionToReturn = "SW"; break;
	case W: directionToReturn = "W"; break;
	}

	return directionToReturn;
}

// NOTES ABOUT FLIGHT LOG STATUS
void Lindy::printNotes() { cout << "\n   " << location->notes; }

/************
  GET TIME
*************/

// HR
int Lindy::getHour() { return location->now->hours; }

// MIN
int Lindy::getMin() { return location->now->minutes; }

// a.m. or p.m.
string Lindy::getZone()
{
	string str_to_ret = " ";

	switch (location->now->zone)
	{
	case AM: str_to_ret = "a.m."; break;
	case PM: str_to_ret = "p.m."; break;
	}
	return str_to_ret;
}

void Lindy::printTime()
{
	cout << "\n   TIME: " << getHour() << ":" << getMin() << " " << getZone();
}

/***********************************************
Function: updateLog()

Parameters:
	int newHrs:		The total flight hrs.
	int hr:			The current hour.
	int min:		The current minute.
	AM_PM zn:		Either a.m. or p.m.
	string land1:	Landmark 1.
	string land2:	Landmark 2.
	string water:	Closest body of water.
	int alt:		Altitude.
	int speed:		Airspeed of plane.
	int wind:		Current windspeed.
	Direction way:	North, South, etc.
	string myNotes:	Details about checkpoint.

Description:
	Updates members of the Coordinates struct
	part of THIS Level class. Used for printing
	the flight log as part of game play for
	Lindy_Level. Deletes previous contents then
	calls Coordinates constructor to fill with
	the new function arguments.

************************************************/
void Lindy::updateLog(
	int newHrs,
	int hr, int min, AM_PM zn,
	string land1,
	string land2,
	string water,
	int alt,
	int speed,
	int wind,
	Direction way,
	string myNotes
)
{
	delete location;

	location = new Coordinates(
		newHrs,
		hr, min, zn,
		land1,
		land2,
		water,
		alt,
		speed,
		wind,
		way,
		myNotes
	);
}

// Prints currect Coordinate struct contents
void Lindy::printLog()
{
	cout << "\n   ====================================================="
		<< "\n   FLIGHT LOG:";
	printTime();
	cout << "\n   Hrs. Logged: " << getFlightHours()
		<< "\n   Landmass 1: " << getLandMass(1)
		<< "\n   Landmass 2: " << getLandMass(2)
		<< "\n   Water: " << getWaterBody()
		<< "\n   Altitude: " << getAltitude()
		<< "\n   Airspeed: " << getAirSpeed()
		<< "\n   Windspeed: " << getWindSpeed()
		<< "\n   Direction: " << getDirection()
		<< "\n   NOTES:"
		<< "\n   ------";
	printNotes();
	printEnterWipe("=====================================================");

}

/**********************************************************************************
Function Name: search()
Description:
		Performs action SEARCH option on a Level

		STEPS:
			1. Prompt yes or no input to search
			2. Validates step 1 input
			3. If yes:
				AND itemStack is empty:
					prints indicate so
				AND itemStack is NOT empty:

					a) pop item off 'itemStack' (BASE class member of LevelClass)
					b) print item popped
					c) ask if user wants it y/Y/n/N only (validates)
					d) If doesn't want:
						push onto 'droppedItems' Stack (BASE class member of LevelClass)
					e) If wants:
						enqueue in hero's 'satchel' (queue)

			4. If no: exit SEARCH

Parameters: Character* hero (YOU in game)

Effect:
		Gain item popped or it is left for recovery later

*************************************************************************************/
void Lindy::search(Character* chuck)
{
	cout << "\n   Should I be looking for something here?"
		 << "\n\n   [enter] YES <y|Y> -or -NO <n|N>\n\n   => ";

	// validate
	char password = checkYesNo("I shouldn\'t add any extra weight. Should I get this?");

	if (toupper(password) == 'Y')
	{
		if (itemStack->isEmpty())
		{
			printThenEnter("It\'s not worth the risk. Nevermind.");
		}
		else
		{
			Item *temp = nullptr;
			temp = itemStack->pop();

			wipeScreen();

			// displays what was found w/ custom dialogue
			this->printNewItemFound(temp, "what\'s this here? looks like");

			// validate
			char response = checkYesNo("Hey Chuck.. how \'bout it?");

			// leaving behind at this location
			if (toupper(response) == 'N')
			{
				printThenEnter("hmm..   I don't want that now.");

				// this will place it in a queue at this location
				this->droppedItems->enqueue(temp);
			}
			else
			{
				// place in satchel
				chuck->getSatchel()->enqueue(temp);
				// show effect
				this->printUpdatedQueue(chuck);
			}

		}
	}

}

/*********************************************************************************
PURE VIRTUAL Function Name: action()
Description:
	Executes ACTION cmd option for this level. Perform Charles Lindbergh's 
	epic flight from New York to Paris without crashing!

**********************************************************************************/
void Lindy::action(Character* chuck)
{
	cin.ignore();

	// plotline
	cout << "\n   LOCATION:";
	cout << this->getLocationName();

	// INTRODUCTION
	if (!introRead) { lindyIntro(); }
	if (introRead)
	{
		printEnterWipe("You are near a dirt airstrip.");
		printEnterWipe("Nearby you see a small single-prop.");
		printEnterWipe("You notice that you are wearing a cotton-canvas flight suit.");
		printEnterWipe("Definately pre-World-War-II you think.");
		printEnterWipe("I must be flying somewhere..");
		printEnterWipe("You\'ve got a flight suit.. but you\'ll need more than that.");
		printEnterWipe("Check for supplies..");

		if (!hasSupplies) { hasSupplies = isPrepared(chuck); }
		if (hasSupplies)
		{
			SpiritStLouis* plane = new SpiritStLouis();

			// show Spirit
			printEnterWipe("Walking closer to the plane you start to get a good look..");
			printEnterWipe("It\'s a small silver monoplane.");
			printEnterWipe("NO WAY! It\'s it\'s");

			plane->showVehicle();

			printEnterWipe("Get in.");
			plane->setPilot(chuck);
			plane->checkAircraft();

			// get inside
			printEnterWipe("Sitting in the whicker chair, and no windows, you realize..");
			printEnterWipe("\"I\'m actually sitting in the \'Spirit of Saint Louis\'!\"");
			printEnterWipe("This is so cool..");

			// place emergency kit in compartment
			printEnterWipe("First you need to stash that emergency kit.");
			printEnterWipe("Stow it.");
			Item* kit = plane->getPilot()->getSatchel()->searchSatchel("\n   Emergency Kit");
			plane->setInCompartment(kit);
			cout << "\n   <ENTER>";
			cin.get();
			wipeScreen();

			// start engine
			printEnterWipe("Start Engine.");
			if (plane->startEngine())
			{
				flyToParis(plane);
			}
			else
			{
				printEnterWipe("Couldn\'t start the engine.");
			}

			// put Spirit of Saint Louis in Smithsonian
			delete plane; plane = nullptr;
		}
	}
}

// Intro Historical Info & Dialogue
void Lindy::lindyIntro()
{
	printDate();
	cout << "\n   DESCRIPTION:";
	cout << this->getDescription();
	cout << "\n\n   SIGNIFICANCE:";
	cout << this->getSignificance();
	printEnterWipe(" ");

	introRead = true;
}

/*************************************************************
Function: isPrepared()

Parameters:
	Character* charles. The user's main game Character.

Description:
	Checks if 3 items are in charles' satchel before 
	entering flyToParis() function. If an item is not
	in charles' satchel (queue) then a message informs
	the user about it. 
	
	Makes calls to validator functions:
	-----------------------------------
		hasHat()	hasGoggles()	hasEmergencyKit()

	Req'd. Items:
	-------------
		-Aviator Cap	-Goggles	-Emergency Kit

Returns:
	If even 1/3 items missing: FALSE
	If 3/3 items in charles' satchel(queue): TRUE

**************************************************************/
bool Lindy::isPrepared(Character* charles)
{
	bool hasEverything = false;

	// check for hat
	if (!hasHat(charles)) 
	{ 
		printEnterWipe("You forgot your Aviator Cap!"); 
	}
	else
	{
		printEnterWipe("Hat.. check.");

		// check for goggles
		if (!hasGoggles(charles)) 
		{ 
			printEnterWipe("You don\'t have your goggles either."); 
		}
		else
		{
			printEnterWipe("Goggles.. check.");

			// check for emergency kit
			if (!hasEmergencyGear(charles)) 
			{ 
				printEnterWipe("You\'ll also need an emergency kit.. just in case."); 
			}
			else
			{
				printEnterWipe("Emergency kit.. check.");
				hasEverything = true;
			}
		}
	}
	return hasEverything;
}

// assert lindy has his hat
bool Lindy::hasHat(Character* lindy)
{
	bool result = false;
	string hat = "\n   Aviator Cap";

	if (lindy->iHaveThis(hat)) { result = true; }
	return result;
}

// assert lindy has his goggles
bool Lindy::hasGoggles(Character* lindy)
{
	bool result = false;
	string goggles = "\n   Aviator Goggles";

	if (lindy->iHaveThis(goggles)) { result = true; }
	return result;
}

// assert lindy has his emergency supplies
bool Lindy::hasEmergencyGear(Character* lindy)
{
	bool result = false;
	string gear = "\n   Emergency Kit";

	if (lindy->iHaveThis(gear)) { result = true; }
	return result;
}

/***************
  CHECKPOINTS
****************/
void Lindy::flyToParis(SpiritStLouis* plane)
{
	checkPoint1(plane);
	checkPoint2(plane);
	checkPoint3(plane);
	if (!isGameLost()) { checkPoint4(plane); }
	if (!isGameLost()) { checkPoint5(plane); }
	if (!isGameLost()) { checkPoint6(plane); }
	if (!isGameLost()) { checkPoint7(plane); } 
	if (!isGameLost()) { checkPoint8(plane); }
	if (!isGameLost()) { checkPoint9(plane); } 
	if (!isGameLost()) { checkPoint10(plane); } 
	if (!isGameLost()) { checkPoint11(plane); } 
	if (!isGameLost()) { checkPoint12(plane); } 
	if (!isGameLost()) { checkPoint13(plane); } 
	if (!isGameLost()) { checkPoint14(plane); } 
	if (!isGameLost()) { checkPoint15(plane); } 
	if (!isGameLost()) { checkPoint16(plane); } 
	if (!isGameLost()) { checkPoint17(plane); } 
	if (!isGameLost()) { checkPoint18(plane); } 
	if (!isGameLost()) { checkPoint19(plane); } 
	if (!isGameLost()) { checkPoint20(plane); } 
	if (!isGameLost()) { checkPoint21(plane); } 
}

/*MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
ALL CHECKPOINT FUNCTIONS:

Parameter: SpiritStLouis* plane. The Spirit of Saint Louis
	plane that 'chuck' input is flying.

General Description: 
	The following 21 checkpoints reflect the flight log of
	Charles Lindbergh during his solo trip NY-->Paris 1927.

MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM*/

/****************************************************************
Description:						#1 
	Time: 8:52 a.m. 
	Flight over RI, LongIslandSound, Connecticut

****************************************************************/
void Lindy::checkPoint1(SpiritStLouis* plane)
{
	plane->burnFuel();
	plane->checkAircraft();

	string newNotes = "Currently over Rhode Island."
		"\n   Except for some turbulence, the flight over"
		"\n   Long Island Sound and Connecticut uneventful.";

	// 8:52am flight over RI, LongIslandSound, Conn.
	location = new Coordinates(1, 8, 52, AM, "Rhode Island", "Connecticut", "Long Island Sound", 500, 0, 0, NW, newNotes);
	printLog();
}

/****************************************************************
Description:						#2
	Time: 9:52 a.m. Boston & Cape Cod.

****************************************************************/
void Lindy::checkPoint2(SpiritStLouis* plane)
{
	plane->burnFuel();
	plane->checkAircraft();

	string newNotes = "Boston lies behind the plane;\n   Cape Cod to the right.";
	updateLog(2, 9, 52, AM, "Boston", "Cape Cod", " ", 150, 107, 0, NW, newNotes);
	printLog();
}

/****************************************************************
Description:						#3
	Time: 10:52 a.m. 
	1st time tired. 4hrs. from NY. Flies 10 ft. from h2o

****************************************************************/
bool Lindy::checkPoint3(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("Descend to 10 ft.");
	int roll = plane->getPilot()->rollInitiative();
	if (roll > 2) { printEnterWipe("Good. Altitude is 10 ft."); }
	else
	{
		plane->damageEngine(roll);
		printEnterWipe("SPLASH!");
		printEnterWipe("That was too low. You got salt water in the engine.");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Feeling tired. Flew near water to stay awake.";
		updateLog(3, 10, 52, AM, "New York", " ", " ", 10, 107, 10, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#4
	Time: 11:52 a.m. Altitude Change. Over Gulf of Maine.

****************************************************************/
bool Lindy::checkPoint4(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("Ascend to 200 ft.");
	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("Good. Altitude is 200 ft."); }
	else
	{
		plane->damageAirframe(roll * 2);

		printEnterWipe("BR-rr-Err-err-Err! Brip-brip!");
		printEnterWipe("Too fast. Airframe damage.");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "400 mi. from NY. Nova Scotia ahead.\n   Passed Gulf of Maine. Only 2-degress off coarse.";
		updateLog(4, 11, 52, AM, "New York", "Nova Scotia", "Gulf of Maine", 200, 107, 10, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#5
	Time: 12:52 a.m. Flight over mt. range. Thick clouds.

****************************************************************/
bool Lindy::checkPoint5(SpiritStLouis* plane)
{
	bool passed = false;
	
	plane->burnFuel();

	printEnterWipe("There\'s a storm front coming!");
	printEnterWipe("You\'ll have to go over a mountain range!");
	printEnterWipe("Careful! the wind-speed just TRIPLED! Stabilize!");

	int roll = plane->getPilot()->rollInitiative();

	if (roll % 2 == 0) { printEnterWipe("Okay.. it\'s still good."); }
	else
	{
		plane->damageAirframe(roll * 3);
		printEnterWipe("BRIP-BRIP-BRIP-pppppppppppppp!");
		printEnterWipe("You got a tear in the fabric.");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Went over Mt. range. Clouds.\n   Storm front approaching.";
		updateLog(5, 12, 52, PM, "mountain range", " ", " ", 200, 107, 30, NW, newNotes);
		printLog();

	}
	return passed;
}

/****************************************************************
Description:						#6
	Time: 2:52 p.m. 
	Flew away from storm. Altitude change to 600 ft.

****************************************************************/
bool Lindy::checkPoint6(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("Ascend to 600 ft. before the storm gets you!");
	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("You avoided it. Altitude is 600 ft."); }
	else
	{
		// airframe
		plane->damageAirframe(roll * 2);
		// engine
		plane->damageEngine(roll * 2);
		// pilot
		plane->getPilot()->defend(roll * 4);

		printEnterWipe("BR-rr-Err-err-Err! Brip-brip!");
		printEnterWipe("STRESS! engine-airframe-YOU");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Had to move away from storm.";
		updateLog(7, 2, 52, PM, " ", " ", " ", 600, 96, 15, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#7
	Time: 3:52 p.m.
	East edge of Nova Scotia Island. Fight to stay awake

****************************************************************/
bool Lindy::checkPoint7(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("Try to stay awake!");
	int roll = plane->getPilot()->rollInitiative();
	if (roll > 4) 
	{ 
		printEnterWipe("shake-squint-wince-squint"); 
		printEnterWipe("Yaawwn. Okay I\'m awake.");
	}
	else
	{
		printEnterWipe("You fell asleep!");
		plane->stall();
		printEnterWipe("meanwhile.. YOUR ENGINE STALLED!");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "East edge of Nova Scotia.\n   Cape Breton Is. below."
			"\n   Almost over water again. Trying to stay awake";
		updateLog(8, 3, 52, PM, "Nova Scotia", "Cape Breton Island", " ", 600, 96, 15, NW, newNotes);
		printLog();
	}
	else
	{
		printEnterWipe("YOU CRASHED!");
		printEnterWipe("GAME OVER!");
		this->setLostGame();
	}
	return passed;
}

/****************************************************************
Description:						#8
	Time: 5:52 p.m.
	Along southern coast of Newfoundland

****************************************************************/
bool Lindy::checkPoint8(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("Descend to 300 ft.");
	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("Good. Altitude is 300 ft."); }
	else
	{
		// airframe
		plane->damageAirframe(roll * 2);

		printEnterWipe("Whoosh ~ SHREeep!");
		printEnterWipe("Fabric tear!");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Off So. coast of New Foundland.";
		updateLog(10, 5, 52, PM, "New Foundland", " ", " ", 300, 92, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#9
	Time: 7:52 p.m.
	Stars out & Fog developing. Ascend to 7500 ft.

****************************************************************/
bool Lindy::checkPoint9(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("It\'s dusk.. and the starts are coming out.");
	printEnterWipe("There\'s a fog developing below QUICKLY!");
	printEnterWipe("Ascend to 7500 ft. to avoid it.");

	int roll = plane->getPilot()->rollInitiative();
	if (roll >= 5) { printEnterWipe("You made it to 7500 ft!"); }
	else
	{
		// airframe
		plane->damageAirframe(roll);
		printEnterWipe("Whoosh ~ SHREeep!"); 
		printEnterWipe("Fabric tear!"); 
		
		// engine
		plane->damageEngine(roll);
		printEnterWipe("BR-rr-Err-err-Err! Brip-brip!"); 

	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Nightfall. Stars coming out, and fog below."
			"\n   Alt gain from 800 ft. to 7500 ft."
			"\n   to avoid quick-rising cloud.";
		updateLog(12, 7, 52, PM, "stars above", "fog below", "Atlantic", 7500, 92, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#10
	Time: 8:52 p.m.
	Still Fog below. Thunder-head coming. Evade and clear ice.

****************************************************************/
bool Lindy::checkPoint10(SpiritStLouis* plane)
{
	bool passed = false;
	
	plane->burnFuel();

	passed = survivedElectrocution(plane);

	if (passed)
	{
		plane->checkAircraft();

		string newNotes = "Cloud that was fog still below. Thunder-head coming."
			"\n   Attempted to fly into cloud, but ice forming.";
		updateLog(13, 8, 52, PM, "fog", "thunder-head", "Atlantic", 10000, 92, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#11
	Time: 10:52 p.m.
	Fight to stay awake. consider clising windows for cold
	but doesn't to stay awake.

****************************************************************/
bool Lindy::checkPoint11(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("It\'s cold.. should I open my windows?");
	int roll = plane->getPilot()->rollInitiative();
	if (roll > 1) { printEnterWipe("Windows open. I should stay awake now"); }
	else
	{
		printEnterWipe("I started to doze off!"); 
		printEnterWipe("Stressful.");
		plane->getPilot()->defend(roll * 10);
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Difficult to stay awake. Cold."
			"\n   Decided to keep windows open to stay awake.";
		updateLog(15, 10, 52, PM, " ", " ", "Atlantic", 10000, 92, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#12
	Time: 11:52 p.m. Air warming. No more ice. 

****************************************************************/
void Lindy::checkPoint12(SpiritStLouis* plane)
{
	plane->burnFuel();
	makeRepairs(plane);

	string newNotes = "500 mi. from New Foundland."
		"\n   Air warming. No ice.";
	updateLog(16, 11, 52, PM, "New Foundland", " ", "Atlantic", 10000, 90, 20, NW, newNotes);
	printLog();
}

/****************************************************************
Description:						#13
	Time: 1:52 a.m. Half way to Paris. Dread

****************************************************************/
void Lindy::checkPoint13(SpiritStLouis* plane)
{
	plane->burnFuel();
	makeRepairs(plane);

	string newNotes = "18 hours to Paris. Half-way there."
		"\n   My excitement has turned to dread.";
	updateLog(18, 1, 52, AM, " ", " ", "Atlantic", 10000, 90, 20, NW, newNotes);
	printLog();

	plane->checkAircraft();
}

/****************************************************************
Description:						#14
	Time: 2:52 a.m.  
	Daylight. light revives.. but fall asleep for a moment

****************************************************************/
bool Lindy::checkPoint14(SpiritStLouis* plane)
{
	bool passed = false;
	
	plane->burnFuel();

	printEnterWipe("Daylight!");
	printEnterWipe("I forgot I\'ve gone through a few time zones..");
	printEnterWipe("The sunlight revives you for a moment but..");
	printEnterWipe("..you STILL fall asleep!");
	printEnterWipe("WAKE UP!");

	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("I\'m awake I\'m awake!"); }
	else
	{
		printEnterWipe("As you fall asleep your plane begins to descend!");
		printEnterWipe("Choking on the oxygen you begin to stall!");
		plane->stall();
		printEnterWipe("C\'mon C\'mon START!");

		// successful re-start
		if (plane->startEngine())
		{
			printEnterWipe("Whoa? huh?");
			printEnterWipe("I was passed out for a moment there.");

			plane->burnFuel();
		}
	}

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		plane->checkAircraft();

		string newNotes = "Daylight!. Revived for a moment,"
			"\n   then fell asleep. Only a moment.";
		updateLog(19, 2, 52, AM, " ", " ", "Atlantic", 10000, 90, 20, NW, newNotes);
		printLog();
	}
	else
	{
		printEnterWipe("You couldn\'t re-start the Spirit of Saint Louis!");
		printEnterWipe("You Crashed into the Atlantic.");
		printEnterWipe("GAME OVER !!");
		this->setLostGame();
	}
	return passed;
}

/****************************************************************
Description:						#15
	Time: 4:52 a.m.
	Flying in fog. Falling asleep with eyes open.. 
	but awakens sec./min. after. Hallucenations

****************************************************************/
bool Lindy::checkPoint15(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("I\'m sooo tired.");
	printEnterWipe("More fog.");
	printEnterWipe("You fall asleep AGAIN!");

	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("I\'m awake I\'m awake!"); }
	else
	{
		printEnterWipe("As you fall asleep your plane begins to descend!");
		printEnterWipe("Choking on the oxygen you begin to stall!");
		plane->stall();
		printEnterWipe("C\'mon C\'mon START!");

		if (!plane->startEngine())
		{
			printEnterWipe("You couldn\'t re-start the Spirit of Saint Louis!");
			printEnterWipe(".. and the Spirit is spiraling into the sea!");
			// 100 % damage !
			plane->damageAirframe(100);
		}
		else
		{
			printEnterWipe("Whoa? huh?");
			printEnterWipe("I was passed out for a moment there.");

			plane->burnFuel();

			printEnterWipe("You start to hallucenate..");
			printEnterWipe(".. wha ... t.. is ?? ");
			printEnterWipe("Attempt to snap out of it!");

			roll = plane->getPilot()->rollInitiative();
			// Snapped out of hallucenations
			if (roll % 2 != 0)
			{
				printEnterWipe("(wags head) shake-shake-shake WHO-A!");
				printEnterWipe("I\'m good.");
			}
			else
			{
				// Major Crash - Possible to survive
				printEnterWipe("YOU\'RE sTiLl H~a~L~l~U~c~E~n~A~t~I~n~G * ~ * ~ *");
				printEnterWipe("You hit the water!");
				printEnterWipe("WARGNING: Severe Engine damage!");
				plane->damageEngine(roll * 4);
				printEnterWipe("DANGER: Engine Stall!");
				plane->stall();
				printEnterWipe("You slammed against the water AGAIN!");
				printEnterWipe("WARGNING: Critical Airframe damage!");
				plane->damageAirframe(80);
			}
		}
	}

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		plane->checkAircraft();

		string newNotes = "Flying in fog again. Fell asleep."
			"\n   Don\'t know how long. Minutes?"
			"\n   Fog starting to clear.";
		updateLog(21, 4, 52, AM, " ", " ", "Atlantic", 10000, 90, 20, NW, newNotes);
		printLog();
	}
	else
	{
		printEnterWipe("You Crashed into the Atlantic.");
		printEnterWipe("GAME OVER !!");
		this->setLostGame();
	}
	return passed;
}

/****************************************************************
Description:						#16
	Time: 7:52 a.m. Feeling less tired.

****************************************************************/
void Lindy::checkPoint16(SpiritStLouis* plane)
{
	printEnterWipe("I\'ve been up for 24 hrs. now.. uuhg.");

	plane->burnFuel();
	makeRepairs(plane);

	string newNotes = "24 hrs. from NY. Less tired now.";
	updateLog(24, 7, 52, AM, " ", " ", "Atlantic", 10000, 90, 20, NW, newNotes);
	printLog();

	plane->checkAircraft();
}

/****************************************************************
Description:						#17
	Time: 9:52 a.m. Fly in circles around fishing boats

****************************************************************/
bool Lindy::checkPoint17(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	FishingBoat* boat = new FishingBoat();

	printEnterWipe("As you draw nearer Europe.. you finally see something..");
	boat->showVehicle();

	// sink boat
	delete boat; boat = nullptr;

	printEnterWipe("Descend closer.. to 100 ft.");
	printEnterWipe("Then circle and ask for directions");

	int roll = plane->getPilot()->rollInitiative();
	if (roll > 2) 
	{ 
		printEnterWipe("Good. Altitude is 100 ft."); 
		printEnterWipe("But nobody is home!");
	}
	else
	{
		// engine
		plane->damageEngine(roll);

		printEnterWipe("SPLASH!");
		printEnterWipe("That was too low. You got salt water in the engine.");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Spotted fishing boats."
			"\n   Circled them to ask for directions. No response.";
		updateLog(26, 9, 52, AM, " ", " ", "Atlantic", 100, 90, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#18
	Time: 10:52 a.m. 
	(3pm local). South tip of Ireland

****************************************************************/
bool Lindy::checkPoint18(SpiritStLouis* plane)
{
	bool passed = false;
	
	plane->burnFuel();

	printEnterWipe("The fishing boats were no help.");
	printEnterWipe("Ascend to 500 ft. for a better view.");

	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("Good. Altitude is 500 ft."); }
	else
	{
		// airframe
		plane->damageAirframe(roll * 2);

		printEnterWipe("BR-rr-Err-err-Err! Brip-brip!");
		printEnterWipe("Too fast. Airframe damage.");
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Spotted land to my left. Suspect is Ireland."
			"\n   Went toward it. 2.5 hrs. ahead of schedule."
			"\n   Only 3 mi. off course.";
		updateLog(27, 10, 52, AM, "Southern tip of Ireland", " ", "Atlantic", 500, 90, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#19
	Time: 12:52 p.m. 
	French coast spotted. Increased speed to get there 
	before dark. Wide awake.

****************************************************************/
bool Lindy::checkPoint19(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	printEnterWipe("Finally getting somewhere..");
	printEnterWipe("The English coast is behind me now.");
	printEnterWipe("I need to get to the French coast before sundown.");
	printEnterWipe("Increase throttle to 110 m.p.h.");

	int roll = plane->getPilot()->rollInitiative();
	if (roll > 3) { printEnterWipe("Okay good. Airspeed now 110 m.p.h."); }
	else
	{
		printEnterWipe("Speed increased.. but you accelerated too fast.");
		printEnterWipe("Burned too much fuel!");
		for (int i = 0; i < 3; i++) { plane->burnFuel(); }
	}
	plane->checkAircraft();

	// validate not dead
	passed = stillFlying(plane);

	if (passed)
	{
		string newNotes = "Wide Awake. England behind."
			"\n   Attempt to reach Fr. coast before night.";
		updateLog(29, 12, 52, PM, "English Coast", "French Coast", "Atlantic", 500, 110, 20, NW, newNotes);
		printLog();
	}
	return passed;
}

/****************************************************************
Description:						#20
	Time: 2:52 p.m. 
	Sunset. Cherbourg. Fr. Coast 
	
****************************************************************/
void Lindy::checkPoint20(SpiritStLouis* plane)
{
	plane->burnFuel();

	printEnterWipe("You are approaching coastal Cherbourg, France!");

	string newNotes = "Approaching coastal Cherbourg."
		"\n   200 mi. from Paris.";
	updateLog(31, 2, 52, PM, "Cherbourg", "Paris", "French Coast", 500, 110, 20, NW, newNotes);
	printLog();

	plane->checkAircraft();
}

/****************************************************************
Description:						#21
	Time: 5:22 p.m.
	LeBourget Aerodome, Paris, France. Landing. (10:22pm local)

****************************************************************/
bool Lindy::checkPoint21(SpiritStLouis* plane)
{
	bool passed = false;

	plane->burnFuel();

	passed = landPlane(plane);
	
	if (passed)
	{
		string newNotes = "Landed! Le Bourget Aerodome, Paris, France."
			"\n   Local time: 10:22 p.m."
			"\n   Made it in 33 hrs., 30 min. 29.8 secs."
			"\n   Haven\'t slept in 55 hrs.";
		updateLog(31, 2, 52, PM, "Le Bourget Aerodome, Paris, France.", " ", " ", 0, 0, 0, NW, newNotes);
		printLog();
	}
	else
	{
		printEnterWipe("You Crashed and Burned!");
		printEnterWipe("Such a tragedy!..");
		printEnterWipe("You were SO close!");
		printEnterWipe("GAME OVER !");
		this->setLostGame();
	}
	return passed;
}

/***********************
   CHECKPOINT HELPERS
************************/

/****************************************************************
(helper) Function: stillFlying()

Parameters:
	SpiritStLouis* plane. The plane 'chuck' is flying.

Description:
	Validates if plane's structural integrity is positive and
	that it is not stalled. If stalled, then will re-start
	engine [and burnFuel()]

Returns:
	If structuralIntegrity > 0 AND not stalled: TRUE
	Otherwise: FALSE

****************************************************************/
bool Lindy::stillFlying(SpiritStLouis* plane)
{
	bool isFlying = false;

	// positive struct. integrity
	if (plane->getStructuralIntegrity() > 0)
	{
		// not stalled
		if (plane->isStalled())
		{
			printEnterWipe("DANGER: Engine Stall!");
			printEnterWipe("Re-start Engine!");
			if (plane->startEngine()) { isFlying = true; }

			plane->checkAircraft();
		}
		else
		{
			isFlying = true;
		}
	}
	else
	{
		cout << "\n   DANGER: structural integrity is: " << plane->getStructuralIntegrity()
			 << "\n\n   <ENTER>";
		cin.get();
		wipeScreen();
	}

	return isFlying;
}

/****************************************************************
(helper) Function: makeRepairs()

Parameters:
	SpiritStLouis* plane. The plane 'chuck' is flying.

Conditions:
	Must have Item* Emergency Kit in plane's compartment to
	operate.

Description:
	Performs repair of Spirit of Saint Louis airplane.
	Even rolls make repairs. Repairs continue with each
	successive even roll. Calls checkAircraft() helper
	at end.

Effect:
	Even initiative rolls by plane's pilot will repair
	plane up to max_structure & max_hp accordingly.

	Lack of an emergency kit will print that error, and no
	repairs will happen.

****************************************************************/
void Lindy::makeRepairs(SpiritStLouis* plane)
{
	int max_structure = 100;
	int max_hp = 223;
	int repair = 0;

	printEnterWipe("I should do some repairs now if I can.");

	if (plane->getCompartment() != nullptr && // check not null first
	   (plane->getCompartment()->getTitle() == "\n   Emergency Kit")
	){
		do
		{
			repair = plane->getPilot()->rollInitiative();

			if (plane->getStructuralIntegrity() < max_structure)
			{
				plane->repairAirframe(repair);
			}
			if (plane->getHp() < max_hp)
			{
				plane->repairEngine(repair);
			}

		} while (repair % 2 == 0);

		printEnterWipe("Repairs complete.");
		plane->checkAircraft();
	}
	else
	{
		printEnterWipe("The emergency kit and all of it\'s tools are missing!");
		printEnterWipe("Couldn\'t make any repairs!");
	}
}

/****************************************************************
(helper) Function: survivedElectrocution()

Parameters:
	SpiritStLouis* plane. The plane you are flying.

Description:
	checkPoint10() Function helper. Handles process of
	Lindbergh [plane->pilot] is electocuted in a storm.

	STEPS:
		1. Roll init. to see if elevated to 10k ft.
			success:
				if emergency kit
					roll to de-ice
						SUCCESS: put kit away <======
						failure: LOSE kit
			failure:
				plane stalls.
				call defend on pilot.
					if pilot->getStrength() >= 0
						engine stalled
						re-starte engine
							success: continue
							fail: crash [GAME OVER]
					else
						ELECTROCUTED [GAME OVER]

Effect:
	If Ascend to 10k ft. & de-iced plane: CONTINUE 
	Else if Ascend to 10k ft. but NOT de-ice. Lose emer. kit
	Else: LOSE GAME

*****************************************************************/
bool Lindy::survivedElectrocution(SpiritStLouis* plane)
{
	bool survived = false;

	printEnterWipe("Looming is a big Thunder-Head!");
	printEnterWipe("You don\'t want to get electrocuted");
	printEnterWipe("Ascend to 10,000 ft. !!");

	int roll = plane->getPilot()->rollInitiative();
	if (roll >= 5)
	{
		printEnterWipe("Altitude now 10,000 ft. !");
		printEnterWipe("Ice is starting to form!");
		printEnterWipe("Get out your emergency kit..");
		printEnterWipe("Find something to break up the ice!");

		// get out Emergency Kit from plane
		Item* kit = plane->getCompartment();
		plane->setInCompartment(nullptr);

		if (kit->getTitle() == "\n   Emergency Kit")
		{
			printEnterWipe("Okay now break up the ice!");
			roll = plane->getPilot()->rollInitiative();
			if (roll % 2 == 0)
			{
				printEnterWipe("Whey! Okay that worked.");
				plane->setInCompartment(kit);
			}
			// lose emergency kit !
			else
			{
				printEnterWipe("You got most of it..");
				printEnterWipe("But you dropped the emergency kit!");
				delete kit; kit = nullptr;
			}
			survived = true;
		}
	}
	else
	{
		plane->stall();

		// attack pilot's strength
		plane->getPilot()->defend(roll * 3);

		if (plane->getPilot()->getStrength() >= 0)
		{
			printEnterWipe("You\'ve been struck by lightening..");
			printEnterWipe("..and you\'re still alive!");
			printEnterWipe("BUT THE SPIRIT HAS STALLED!");
			printEnterWipe("Re-start engine.");

			if (!plane->startEngine())
			{
				printEnterWipe("The Spirit wouldn\'t start.");
				printEnterWipe("You CRASHED into the Atlantic!");
				printEnterWipe("GAME OVER!");
				this->setLostGame();
			}
			else { survived = true; }
		}
		// electrocuted !
		else
		{
			printEnterWipe("DANGER: Engine Stall!");
			printEnterWipe("YOU\'VE BEEN ELECTROCUTED!");
			printEnterWipe("GAME OVER!");
			this->setLostGame();
		}
	}
	return survived;
}

/****************************************************************
(helper) Function: landPlane()

Parameters:
	SpiritStLouis* plane. The plane you are flying.

Description:
	Handles process of landing the Spirit of St. Louis in
	France. There are 3 initiative rolls of the pilot that 
	must succeed. 

Effect:
	Landed plane results in pilot receiving some food bonus'
	a rare document, and a torch. Health() is also called 2X.

	If Crash: GAME OVER

*****************************************************************/
bool Lindy::landPlane(SpiritStLouis* plane)
{
	bool landed = false;

	printEnterWipe("You\'re almost there!");
	printEnterWipe("You see the airstrip..");
	printEnterWipe("Set up your approach..");

	int roll = plane->getPilot()->rollInitiative();
	if (roll > 1)
	{
		printEnterWipe("Okay good. You\'re lined up.");
		printEnterWipe("Now set flaps.. and lower throttle..");

		roll = plane->getPilot()->rollInitiative();
		if (roll > 2)
		{
			printEnterWipe("Now ease \'er down.. and land.");
			roll = plane->getPilot()->rollInitiative();

			if (roll > 3)
			{
				printEnterWipe("BRRRR-RRR-RRR-PAAAA!");
				printEnterWipe("BUMP-bump-screeech! ~~ cruise ~~");
				printEnterWipe("LANDED!");
				landed = true;
			}
			else
			{
				printEnterWipe("SLAM-CRUNCH-BANG-BAng-bang-skiiiidd ~ slide ~ psssht. ~ * ~ * .. .");
			}

		}
		else
		{
			printEnterWipe("Too much throttle!");
			printEnterWipe("WATCH OUT!");
		}

	}
	else
	{
		printEnterWipe("You over-shot the runway!");
	}

	// You Landed! Get stuff!
	if (landed)
	{
		printEnterWipe("The French are cheering you on!");
		printEnterWipe("And they feed you!");

		// wine glass
		plane->getPilot()->addFoodBonus(myInventory->wineGlass());
		cin.get(); wipeScreen();

		// hamburger
		plane->getPilot()->addFoodBonus(myInventory->hamburger());
		cin.get(); wipeScreen();

		printEnterWipe("And shower you with gifts.");
		plane->getPilot()->getSatchel()->enqueue(myInventory->AugustDecrees());
		plane->getPilot()->getSatchel()->enqueue(myInventory->torch());
		printEnterWipe("Take a breather to heal.");
		plane->getPilot()->health();
		printEnterWipe("Maybe one more!");
		plane->getPilot()->health();
		printEnterWipe("And a trip to the hospital.");
		plane->getPilot()->health();
	}
	return landed;
}

//***************************** E O F ********************************
