/********************************************************************************
 * File Name: FlightOfKing_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Flight of the King (SUB) Level.

	Historical recap:
	-----------------
	King Louis XVI and Marie Antoinette attempted to escape to the 
	Fortress of Montmedy where they were to join up with Prussian and
	Austrian counter-revolutionaries. Having declared his support for 
	the revolution before his capture, the event marked the King as a
	traiter to his people. Many distrusted him afterward. 

	Objective:
	----------
	In this level the player's mission is to safely escort the royal family
	to Montemedy without being captured. Madamme Tourzel carries phony
	passports which she must properly show at a checkpoint in Varennes. 
	The level follows the historical sequence of events that led up to the 
	King's capture. Further explanation is found in the historical documents 
	included as part of the game play. This level is WIN or LOSE the game only. 

 ********************************************************************************/
#ifndef GGOUGEON_KING_FLEES_HPP
#define GGOUGEON_KING_FLEES_HPP

#include<cstring>
#include"LevelClass.hpp"

#define MAX_GENDARMARIE 6
#define MAX_DRAGOONS 6

enum FirstAttack {GENDARMARIE, DRAGOON};

class KingFlees : public Level
{
	private:
		int soundTally;
		int recognizedTally;

		bool introComplete;
		bool hasEscapeProvisions;
		bool womenReady;
		bool loadedSilently;
		bool departedTuileries;

		bool mainMissionComplete;

		PalaceGuards* Gendarmarie[MAX_GENDARMARIE];
		int gendarmeCount;
		PalaceGuards* Dragoons[MAX_DRAGOONS];
		int dragoonCount;

	public:
		KingFlees();
		virtual ~KingFlees();

		// VIRTUAL FUNCTIONS
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void kingFleesIntroSequence();

		void startMissionSequence(Character*);
		bool prepMmeQueen(Character* hero, Character* helper, Character* queenie);
		//------------------------
		bool silentlyLoadCarriage(
			Carriage*,
			LouisXVI*,
			MarieAntoinette*,
			HouseWife*,
			MarieAntoinette*,
			PalaceGuards*
		);
		//------------------------
		bool Etoges(Carriage*);
		void Chalons_sur_Marne(Carriage*);
		bool Clermont(Carriage*);
		bool Varennes(Carriage*, bool);

		// HELPERS
		bool checkSupplies(Character*);
		bool transferItems(Character*, string*, int, Character*);

		bool checkMadeNoise(Character*);
		void waitingForKing(Character*);
		bool groupIsIncognito(Carriage*);
		bool stayIncognito(Character*);

		void feedCarriage(Carriage*);
		bool checkPapers(Character*);
		bool inspectCarriage(Carriage*);
		bool stoppedByGendarmes(Carriage*);

		// GENDARMARIE
		void hireGendarme(int);
		void callGendarmarie();
		void armGendarmarie();
		void fireGendarme(int);
		void closeGendarmarie();

		// DRAGOONS
		void recruitDragoon(int);
		void callDragoons();
		void armDragoons();
		void dischargeDragoon(int);
		void disbandDragoons();

		// Gendarmarie vs. Dragoons
		void brawl();
		
		void gendarmareVsDragoon(
			Character* gendarme, 
			int gRank, 
			Character* dragoon, 
			int dRank, 
			enum FirstAttack aggressor
		);

		// Carriage Guards vs. Gendarmarie
		bool guardsDefendCarriage(Carriage*);

};
#endif

//***************************** E O F ********************************
