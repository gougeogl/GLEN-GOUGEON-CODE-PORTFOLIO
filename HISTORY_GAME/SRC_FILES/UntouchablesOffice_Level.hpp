/*****************************************************************************
 * File Name: UntouchablesOffice_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Untouchables Office Class (SUB) Level

	The trouble with convincting Al Capone, besides the inherent corruption
	of the political systems in Chicago and elsewhere, there wasn't a solid
	paper trail to link Capone with his illegal business activities. This
	level has the user deliver Capone's accounting ledger to Frank Wilson 
	and Elmer Irey, the treasury agents to were responsible to getting a
	conviction on tax-evasion for Capone.

 *****************************************************************************/
#ifndef GGOUGEON_UNTOUCABLES_OFFICE_LEVEL_HPP
#define GGOUGEON_UNTOUCABLES_OFFICE_LEVEL_HPP

#include"LevelClass.hpp"

class Office : public Level
{
	private:
		bool introRead;

	public:
		Office();
		virtual ~Office();

		// virtual functions	
		virtual void search(Character*);
		virtual void action(Character*);

		void untouchablesIntroDialogue();

};
#endif 

//***************************** E O F ********************************
