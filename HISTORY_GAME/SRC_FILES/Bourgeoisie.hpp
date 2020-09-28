/*****************************************************************************
 * File Name: Bourgeoisie.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Hero SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_BOURGEOISIE_HPP
#define GGOUGEON_BOURGEOISIE_HPP

#include"CharacterClass.hpp"
#define BOURGEOISIE_MAX_HEALING 6

class Bourgeoisie : public Character
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
	Bourgeoisie();
	virtual ~Bourgeoisie();

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
