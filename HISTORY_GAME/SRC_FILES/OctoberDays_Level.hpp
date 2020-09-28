/*****************************************************************************
 * File Name: OctoberDays_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for October Days (Level) Class.

	History:
	--------
	October 5, 1789. Parisian house-wives were fed up by the government's
	response to the food shortage. High taxation was a consequence of the
	Hundred Years' war as well as mis-management by the royals. They marched
	12 miles to Versailles to demand the King agree to sign a written
	constitution, and approve the abolishement of special obligations to the
	nobles by commoners. 20,000 Parisian Guards backed up the move by the 
	French women, and the royals capitulated. 
	
	Objective:
	----------
	In this level, the player
	goes to join the mob at Versailles, and has an opportunity to convince
	the King and Queen to sign. If unsuccessful, the royals might get away
	with it depending if the women can convince the Parisian Guard to 
	attack the Monarch. If attacked, the King is usually killed, and you
	LOSE the GAME! You want them to stay alive, but sign.

 *****************************************************************************/
#ifndef GGOUGEON_OCTOBER_DAYS_HPP
#define GGOUGEON_OCTOBER_DAYS_HPP

#include"LevelClass.hpp"

class OctoberDays : public Level
{
	private:
		bool introRead;
		int convincedRoyalsCount;
		bool missionAccomplished;

	public:
		OctoberDays();
		virtual ~OctoberDays();

		// virtual function	
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void octDaysIntroDialogue();
		void sapStrength(Character*);

		// talk | fight game Characters
		void talkToWoman(Character*);
		void fightWoman(Character*);
		void talkToGuard(Character*);
		void fightOneGuard(Character*);

		// getting royals to agree
		void convinceRoyalsSequence(Character*);
		bool demandRoyals(Character*, Character* royal);

		// will fail mission
		void guardsKillOneRoyal(LouisXVI*, MarieAntoinette*);
		// if mission successful
		void signDecreesAndRight(Character*);

};
#endif

//***************************** E O F ********************************
