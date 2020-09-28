/*****************************************************************************
 * File Name: MarieAnoinette.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for HouseWife SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_MARIE_ANOINETTE_HPP
#define GGOUGEON_MARIE_ANOINETTE_HPP

#include"CharacterClass.hpp"
#define MARIE_MAX_HEALING 6

class MarieAntoinette : public Character
{
private:
	// attack die	
	Die die;

	// defense die	
	Die def;

	// initiative die
	Die initiative;

	Die heal;

public:
	MarieAntoinette();
	virtual ~MarieAntoinette();

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
