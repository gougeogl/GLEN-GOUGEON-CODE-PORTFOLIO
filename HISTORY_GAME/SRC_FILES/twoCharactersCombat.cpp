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
#include"twoCharactersCombat.hpp"

/***********************************************************************************
Function: twoCharactersCombat()

Helpers:
	bothCanFight()
	determineWinner()

Parameters:
	Character* play1. The first attacking character.
	string talk1. play1's combat dialogue [sound effects]

	Character* play2. The first defending character.
	string talk2. play2's combat dialogue [sound effects]

Description:
	Similar to function: twoCharactersCombat() with improvements. Added listed 
	helpers for code clarity. Presents a formatted output of who is fighting who.

	'play1' attacks first, 'play2' defends. Then 'play2' attacks and 'play1' defends.
	Combat continues until either play1 or play2 has negative strength.

	Output of determineWinner() print the winner, and returns int equivalent of
	who won

Returns:
	If 'play1' was winner: 1
	If 'play2' was winner: 2
	otherwise: -1 [err]

************************************************************************************/
int twoCharactersCombat(Character* play1, string talk1, Character* play2, string talk2)
{
	int winner = -1;

	// assert they both have strength to fight!
	if (bothCanFight(play1, play2))
	{
		int play1Attack = 0;
		int play2Attack = 0;

		// fight while one of them has strength >= 0
		while (bothCanFight(play1, play2))
		{
			play1Attack = play1->attack(play2);
			play2->defend(play1Attack);

			play2Attack = play2->attack(play1);
			play1->defend(play2Attack);
		}

		cout << "\n\n   ************************************************************"
			<< "\n   FIGHT: " << play1->getName() << " <= VS. => " << play2->getName()
			<< "\n   ************************************************************\n";
		
		// print sound effects of each
		cout << "\n   " << talk2 << "\n\t\t" << talk1 << "\n\n";
		cout << "\n   " << talk1 << "\n\t\t" << talk2 << "\n\n";

		// print winner
		winner = determineWinner(play1, play2);
		printEnterWipe(" ");
	}
	fflush(stdout);

	return winner;
}

/**************************************************************************
(helper) Function: bothCanFight()

Parameters:
	Character* attack. The Character that started the fight.
	Character* defender. The Character that started defending.

Description:
	Determines if BOTH 'attacker' and 'defender' have enought strength
	to combat (must be >= 0). Helper to melee() function call.

Returns:
	If both have strength >= 0: TRUE
	Otherwise: FALSE

***************************************************************************/
bool bothCanFight(Character* attacker, Character* defender)
{
	bool result = false;
	// attacker has strength
	if (attacker->getStrength() > (-1))
	{
		// defender has strength
		if (defender->getStrength() > (-1))
		{
			result = true;
		}
	}
	return result;
}

/**************************************************************************
(helper) Function: determineWinner()
Parameters:
	Character* attack. The Character that started the fight.
	Character* defender. The Character that started defending.

Description:
	Determines winner by comparing the strength values of attacker and
	defender. Helper to melee() function call.

Returns:
	If error: -1
	If attacker won: 1
	If defender won: 2

***************************************************************************/
int determineWinner(Character* attacker, Character* defender)
{
	int result = -1;

	int attackerStrength = attacker->getStrength();
	int defenderStrength = defender->getStrength();

	// attacker won
	if (attackerStrength > defenderStrength)
	{
		cout << "\n   -----------------------------------------------------------"
			 << "\n   " << attacker->getName() << " won that combat!"
			 << "\n   -----------------------------------------------------------";
		fflush(stdout);

		result = 1;
	}
	// defender won
	else if (defenderStrength > attackerStrength)
	{
		cout << "\n   -----------------------------------------------------------"
			<< "\n   " << defender->getName() << " won that combat!"
			<< "\n   -----------------------------------------------------------";
		fflush(stdout);

		result = 2;
	}

	return result;
}
