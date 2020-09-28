/*****************************************************************************
 * File Name: Algiers_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Algiers Class (SUB) Level

	Algiers, Algeria, North Africa during WWII was the staging point
	for covert missions into Southern France. In this level, you are
	part of a Jedbourgh 3-man special operations team sent to train
	French Partisans. During the process you fight a group of 3 Nazi
	Wehrmacht troopers which, if you live, will give you explosives.
	Those explosives are used in the level above RR_Bridge_Level to
	WIN the GAME!

 *****************************************************************************/
#ifndef GGOUGEON_ALGIERS_CLASS_HPP
#define GGOUGEON_ALGIERS_CLASS_HPP

#include "LevelClass.hpp"

class Algiers : public Level
{
	private:
		bool introRead;

	public:
		Algiers();
		virtual ~Algiers();

		// virtual function	
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void introAlgiers();
		bool NaziAttack(Character*);

};
#endif 

//***************************** E O F ********************************
