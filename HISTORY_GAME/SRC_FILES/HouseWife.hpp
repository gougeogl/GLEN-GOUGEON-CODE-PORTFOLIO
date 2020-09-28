/*****************************************************************************
 * File Name: HouseWife.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for HouseWife SUB Class :

 *****************************************************************************/
#ifndef GGOUGEON_HOUSE_WIFE_HPP
#define GGOUGEON_HOUSE_WIFE_HPP

#include"CharacterClass.hpp"
#define HOUSE_WIFE_MAX_HEALING 10

class HouseWife : public Character
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
		HouseWife();
		virtual ~HouseWife();

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

