/*****************************************************************************
 * File Name: Henchmen.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Henchmen SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_HENCHMEN_CLASS_HPP
#define GGOUGEON_HENCHMEN_CLASS_HPP

#include"CharacterClass.hpp"

class Henchmen : public Character
{
private:

	// attack die	
	Die die;

	// defense die	
	Die def;

public:
	Henchmen();
	virtual ~Henchmen();

	// ROLLING
	virtual int rollAttack() override;
	virtual int rollDef() override;
	virtual int rollInitiative() override;

	// COMBAT
	virtual int attack(Character*);
	virtual void defend(int) override;
	virtual void health() override;
};
#endif // !Henchmen.hpp

//***************************** E O F ********************************

