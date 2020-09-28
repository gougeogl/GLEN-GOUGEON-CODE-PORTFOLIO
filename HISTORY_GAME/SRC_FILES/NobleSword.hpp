/*****************************************************************************
 * File Name: NobleSword.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for NobleSword SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_NOBLE_OF_THE_SWORD_HPP
#define GGOUGEON_NOBLE_OF_THE_SWORD_HPP

#include"CharacterClass.hpp"
#define NOBLE_SWORD_MAX_HEALING 8

class NobleSword : public Character
{
private:
	// attack die	
	Die die;
	Die die2;

	// defense die	
	Die def;
	Die def2;

	// initiative die
	Die initiative;

	Die heal;

public:
	NobleSword();
	virtual ~NobleSword();

	// ROLLING
	virtual int rollAttack() override;
	virtual int rollDef() override;
	virtual int rollInitiative() override;

	// COMBAT
	virtual int attack(Character*) override;
	virtual void defend(int) override;

	// MEDICAL
	virtual void health() override;
};

#endif

//***************************** E O F ********************************
