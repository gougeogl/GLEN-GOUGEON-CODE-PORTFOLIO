/*****************************************************************************
 * File Name: twoCharactersCombat.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for function used in combat between
					2 individual game Characters:

					twoCharactersCombat()
					bothCanFight()
					determineWinner()

 *****************************************************************************/
#ifndef GGOUGEON_TWO_CHAR_COMBAT_HPP
#define GGOUGEON_TWO_CHAR_COMBAT_HPP

#include<string>
using std::string;

#include<iostream>
using std::cin;
using std::cout;

#include"CharacterClass.hpp"

int twoCharactersCombat(Character *, string, Character *, string);
bool bothCanFight(Character* attacker, Character* defender);
int determineWinner(Character* attacker, Character* defender);

#endif
// **************************** EOF ***********************************
