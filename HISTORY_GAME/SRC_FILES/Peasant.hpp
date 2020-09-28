/*****************************************************************************
 * File Name: Peasant.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for Peasant SUB Class :

 *****************************************************************************/

#ifndef GGOUGEON_PEASANT_HPP
#define GGOUGEON_PEASANT_HPP

#include"CharacterClass.hpp"

class Peasant : public Character
{
private:

	// attack die	
	Die die;

	// defense die	
	Die def;

public:
	Peasant();
	virtual ~Peasant();

	// ROLLING
	virtual int rollAttack() override;
	virtual int rollDef() override;
	virtual int rollInitiative() override;

	// COMBAT
	virtual int attack(Character*);
	virtual void defend(int) override;

	// MEDICAL
	virtual void health() override;
};
#endif 

//***************************** E O F ********************************

