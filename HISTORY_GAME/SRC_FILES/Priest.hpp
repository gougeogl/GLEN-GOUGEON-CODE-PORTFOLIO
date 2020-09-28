/*****************************************************************************
 * File Name: Priest.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Priest SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_PRIEST_HPP
#define GGOUGEON_PRIEST_HPP

#include"CharacterClass.hpp"
#define PRIEST_MAX_HEALING 20

class Priest : public Character
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
		Priest();
		virtual ~Priest();

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

