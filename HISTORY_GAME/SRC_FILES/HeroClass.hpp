/*****************************************************************************
 * File Name: HeroClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Hero SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_HERO_CLASS_HPP
#define GGOUGEON_HERO_CLASS_HPP

#include"CharacterClass.hpp"
#define HERO_MAX_HEALING 8

class Hero : public Character
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
		Hero();
		virtual ~Hero();

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
