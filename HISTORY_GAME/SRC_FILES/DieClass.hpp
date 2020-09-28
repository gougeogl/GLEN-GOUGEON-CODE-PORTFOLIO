/*****************************************************************************
 * File Name: DieClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Die Class.

	Die (dice) are used to roll for attack, defense, initiative, and
	health as appropriate. Die are 'rolled' by Characters with few
	exceptions in the game. 
	
	Explanation of die notation:
		1d6 refers to a single 6-sided die.
		The pattern follows for all die types:

		note | qty. of dice | # of sides:
		---------------------------------
		1d8  |       1      |  8-sided
		---------------------------------
		1d10 |       1      | 10-sided
		---------------------------------
		1d12 |       1      | 12-sided
		---------------------------------
		1d20 |       1      | 20-sided

 *****************************************************************************/
#ifndef GGOUGEON_DIE_CLASS_HPP
#define GGOUGEON_DIE_CLASS_HPP

#include <iostream>

class Die
{
	public:
		Die();
		~Die();

		int six(); // 1d6
		int eight(); // 1d8
		int ten(); // 1d10
		int twelve(); // 1d12 
		int twenty(); // 1d20
};

#endif	
//*********************************************************************















































