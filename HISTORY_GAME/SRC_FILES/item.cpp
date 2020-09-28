/*****************************************************************************
 * File Name: item.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Item Class. 

 *****************************************************************************/
#include"item.hpp"

// Constructor
Item::Item()
{
	title = " ";
	description = "\n   Generic item. Shouldn't see this ever.\n";
	type = THING;
	weight = 0.0;
	value = 0.0;
	healthBonus = 0;
	power = 0;
}

// Destructor
Item::~Item(){}

// TITLE
void Item::setTitle(string named)
{
	this->title = named;
}

string Item::getTitle() const
{
	return this->title;
}

// DESCRIPTION
void Item::setDescription(string explained)
{
	this->description = explained;
}

string Item::getDescription() const
{
	return this->description;
}

// TYPE
void Item::setType(enum WOT variety)
{
	this->type = variety;
}

enum WOT Item::getType()
{
	return this->type;
}

// WEIGHT
void Item::setWeight(double weight)
{
	this->weight = weight;
}

double Item::getWeight() const
{
	return this->weight;
}

// VALUE
void Item::setValue(double cost)
{
	this->value = cost;
}

double Item::getValue() const
{
	return this->value;
}

// HEALTH BONUS
void Item::setHealthBonus(int meds)
{
	this->healthBonus = meds;
}

int Item::getHealthBonus() const
{
	return this->healthBonus;
}

// POWER
void Item::setPower(int value)
{
	this->power = value;
}

int Item::getPower()
{
	return this->power;
}

// prints ALL stats of this item
void Item::printItem()
{
	cout << "\n\n   ========================================"
		 << "\n   Title:" << this->getTitle() << "."
		 << "\n   Description:" << this->getDescription() << "\n";
	if (this->getType() == WEAPON) 
	{cout << "\n   Type        : WEAPON";}
	else if (this->getType() == HEAVY_WEAPON) 
	{cout << "\n   Type        : HEAVY_WEAPON"; }
	else if (this->getType() == BOMB) 
	{ cout << "\n   Type        : BOMB"; }
	else if (this->getType() == FOOD) 
	{ cout << "\n   Type        : FOOD"; }
	else if (this->getType() == DOCUMENT)
	{ cout << "\n   Type        : DOCUMENT"; }
	else if (this->getType() == THING) 
	{ cout << "\n   Type        : THING"; }
	cout << "\n   Weight      : " << this->getWeight();
	// set precision 2 decimal places & fixed rather than scientific notation
	cout.precision(2);
	cout << "\n   Value       : " << std::fixed << this->getValue()
		 << "\n   Health Bonus: " << this->getHealthBonus();
	if (this->getType() == WEAPON) { cout << "\n   Power       : " << this->getPower(); }	
	cout << "\n   ========================================";
	fflush(stdout);
}

// ************************************* EOF ************************************

