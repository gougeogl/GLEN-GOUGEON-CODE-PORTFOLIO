/*****************************************************************************
 * File Name: PalaceGuards.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Palace Guards SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_PALACE_GUARDS_HPP
#define GGOUGEON_PALACE_GUARDS_HPP

#include"CharacterClass.hpp"

class PalaceGuards : public Character
{
private:

	// attack die	
	Die die;

	// defense die	
	Die def;

public:
	PalaceGuards();
	virtual ~PalaceGuards();

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

