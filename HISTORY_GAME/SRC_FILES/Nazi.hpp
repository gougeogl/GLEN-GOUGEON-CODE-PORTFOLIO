/*****************************************************************************
 * File Name: Nazi.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specifications for Nazi SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_NAZI_CLASS_HPP
#define GGOUGEON_NAZI_CLASS_HPP

#include"CharacterClass.hpp"

class Nazi : public Character
{
	private:

		// attack die	
		Die die; 
		Die die2;

		// defense die	
		Die def;
		Die def2;

	public:	
		Nazi();
		virtual ~Nazi();

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
