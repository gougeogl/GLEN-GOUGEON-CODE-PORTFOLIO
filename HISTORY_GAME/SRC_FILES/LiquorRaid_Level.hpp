/*****************************************************************************
 * File Name: LiquorRaid_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Function Specifications for Liquor Class.

	The 'Untouchables' job with the Bureau of Alcohol [a defunct agency]
	was to apply pressure to Capone by sabotaging his illegal black-market
	liquor operations. The raids were also publicized to help tarnish his
	image as a simple businessman bringing a product most customers still
	wanted (alcohol). Capone of course probably bribed the papers to paint
	him in a good light despite tactics of fear, intimidation, and murder
	that he used. 

	This level has the user fight 4 of Capone's henchmen in a liquor raid.
	If successful, the user gets Capone's accounting Ledger. When that is
	delivered to the Untouchables Office Level, the user WINS the GAME!

 *****************************************************************************/
#ifndef GGOUGEON_LIQUOR_RAID_LEVEL_HPP
#define GGOUGEON_LIQUOR_RAID_LEVEL_HPP

#include"LevelClass.hpp"

class Liquor : public Level
{
	private:
		bool introRead;
		bool raidFinished;

	public:
		Liquor();
		virtual ~Liquor();

		// virtual function	
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void liquorRaidIntro();
		bool liquorRaid(Character*);
		bool fightFourHenchmen(Character*);
		void drinkWhiskey(Character*);
		void ledgerFound(Character*);

};
#endif

//***************************** E O F ********************************
