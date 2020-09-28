/*****************************************************************************
 * File Name: LouisXVI.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for LouisXVI SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_LOUIS_XVI_HPP
#define GGOUGEON_LOUIS_XVI_HPP

#include"CharacterClass.hpp"
#define LOUIS_MAX_HEALING 8

class LouisXVI : public Character
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
	LouisXVI();
	virtual ~LouisXVI();

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
