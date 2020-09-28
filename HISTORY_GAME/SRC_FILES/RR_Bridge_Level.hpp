/*****************************************************************************
 * File Name: RR_Bridge_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for RR_Bridge_Level Class (SUB) Level

	During WWII Jedbourghs were special operations teams that aided the
	French Partisan resistence fighters. Such teams first trained, then
	accompanied on missions to fight the Germans. In this level, the user
	goes on such a raid of a railroad bridge. If successful, you can
	WIN the GAME! To do so, you must first have the explosives which are
	only available in the Algiers level adjacent to THIS Level. After
	successfully fighting 3 Nazi soldiers, the user gains the explosives.
	The explosives are required here to blow the bridge and WIN.

 *****************************************************************************/
#ifndef GGOUGEON_RR_BRIDGE_CLASS_HPP
#define GGOUGEON_RR_BRIDGE_CLASS_HPP

#include"LevelClass.hpp"

class Bridge : public Level
{
private:
	bool introRead; 

public:
	Bridge();
	virtual ~Bridge();

	// virtual function	
	virtual void search(Character*);
	virtual void action(Character*);

	// ACTION
	void bridgeIntroDialogue();
	void covertMission(Character*);
	void covertMissionCutScene(Character*);
	void plantBridgeExplosives(Item*);
	bool stripWires();
	bool blowBridge(int);
	void successEffects();
	void failEffects();
	
};
#endif 

//***************************** E O F ********************************
