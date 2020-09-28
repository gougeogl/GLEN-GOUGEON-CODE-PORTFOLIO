/*****************************************************************************
 * File Name: Bastille_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Bastille (Level) Class.

	The storming of the Bastille prision on July 14, 1789 was the first
	violent act of the first French Revolution. The Fortress turned prision
	was where the Monarchy sent embarrassments to the state, and opposers of
	the King. It was pillaged in effigy by a cohort of small shop keepers,
	day laborers, and vagrants.

 *****************************************************************************/
#ifndef GGOUGEON_BASTILLE_HPP
#define GGOUGEON_BASTILLE_HPP

#include"LevelClass.hpp"

class Bastille : public Level
{
	private:

		// CANNONS
		bool allCannonsIn;
		int cannonCount;
		Item* armory[5];

		// AMMO
		bool fullMagazine;
		int ammoCount;
		Item* magazine[10];

		bool introCompleted;
		bool sneakAttackCompleted;
		bool reload;
		bool missionCompleted;

	public:
		Bastille();
		virtual ~Bastille();

		// virtual function	
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void introSequence(Character*);
		bool sneakAttackFirst(Character *);

		// CANNON
		bool wheelCannon(Character*);
		void destroyArmory();
		void destroyLastCannon();

		// MAGAZINE (ammo)
		bool loadCannons(Character*);
		void destroyMagazine();
		void destroyLastRound();

};
#endif

//***************************** E O F ********************************
