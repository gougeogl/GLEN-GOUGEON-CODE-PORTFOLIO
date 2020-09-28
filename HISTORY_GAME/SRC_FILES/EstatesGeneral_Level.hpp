/*****************************************************************************
 * File Name: EstatesGeneral_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Estates General (Level) Class.

	The holding of the Estates General was the true beginning of the first
	French Revolution of 1789. It was the moment of opportunity that the
	Bourgeoisie with ambition sought to cancel the Royal Privelages, and
    obligations, to invoke the creation of a written constitution enabling
	them to rise up political, social, and economic ladders.

 *****************************************************************************/
#ifndef GGOUGEON_ESTATES_GENERAL_HPP
#define GGOUGEON_ESTATES_GENERAL_HPP

#include"LevelClass.hpp"
enum DelegateType {PRIEST, NOBLE, COMMONER};

class EstatesGeneral : public Level
{
	private:

		bool introRead;
		bool middleRead;
		bool firstContactComplete;
		bool startedInviting;

		Item* boxOfSummons[3];
		int summonsTally;
		bool allSummonsIn;
		DelegateType invites;

		bool convincedOthers;

	public:
		EstatesGeneral();
		virtual ~EstatesGeneral();

		// virtual function	
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void estatesIntroDialogue();
		void firstContact(Character*);
		void fightTwoGuards(Character*);

		bool callEstatesGeneral(Character*);
		void estatesMiddleDialogue();
		bool attemptDeliveryToAllEstates(Character*);
		bool convinceAndDeliverDelegate(Character*, enum DelegateType);
		bool talkToDelegate(Character*, Character* delegate);

		void stashInvite(Character*);
		void burnAllSummons();
		void burnLastSummons();

		void endingDialogue();
};
#endif

//***************************** E O F ********************************
