/*****************************************************************************
 * File Name: CharacterClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Character BASE Class :

	Game Characters. These are the individual and generic personalities
	found through-out the game. Most have the ability to fight each-other,
	but not all have an initiative, or healing roll available. This is on
	purpose to give unique Characters an advantage. 

	Each Character has:
		* Name. Everybody has one, even if it's a bit generic.

		* Strength (int). Combat reduces your strength if hit.
		
		* Armor(int). Some Characters have armor. During combat, hits go to
		            a Character's armor before they go to their strength
					(usually).

		* Special Ability. Certain abilities give an advantage or dis-advantage
						 for combat and sometimes other situations. 

		* Satchel. A queue data structure for holding an infinite number of
		         Item* objects. EX: fedora. EX: explosives. EX: hamburger

		* Pocket. A single Item* can be held here, though it is NOT used in 
		        this game. The potential is still built in for further
				development.

 *****************************************************************************/
#ifndef GGOUGEON_CHARACTER_CLASS_HPP
#define GGOUGEON_CHARACTER_CLASS_HPP

#include<iostream>
using std::cout;
using std::endl;

#include<string>
using std::string;

#include"item.hpp"
#include"QueueClass.hpp"
#include"DieClass.hpp"

// output
#include "screenFunctions.hpp"

enum Ability {
	NONE = 1,
	TOUGH, 
	DUMB, 
	PILOT, 
	GERMAN, 
	FRENCH, 
	MARKSMANSHIP, 
	HUNCH, 
	SNEAKY,
	RUTHLESS, 
	CHARISMA, 
	PRAYER
};

class Character 
{
	protected:
		
		string name;
		Ability ability;

		int strength;
		int armor;

		Queue *satchel;
		Item* pocket;

		int lastAttackRoll;
		int lastDefenseRoll;

	public:

		Character();
		virtual ~Character();

		// NAME
		void setName(string);
		string getName();

		// ABILITY
		void setAbility(Ability);
		int getAbility(); 	

		// STRENGTH
		int getStrength();
		void setStrength(int);	

		// ARMOR
		void addArmor(int);
		void dentArmor(int);
		int getArmor();

		// SATCHEL & POCKET
		Queue* getSatchel();
		void setPocket(Item*);
		Item* getPocket();

		// VALIDATE an Item* in satchel(QUEUE)
		bool iHaveThis(string);

		// ROLLING : attack
		void setLastAttackRoll(int);
		int getLastAttackRoll();
		
		// ROLLING : defense
		void setLastDefenseRoll(int);	
		int getLastDefenseRoll();

		// PRINT STATS
		void printStats(); // short recurring stats [MENU BAR]
		void printAbility(); // helper to printStats()
		void postCombatStats();

		// WEAPON BONUS'	
		void getNewTrophy(Character* enemy, string item);
		bool findAnyWeapon();
		void addWeaponBonus(Item*);
		bool addNamedWeaponOrAnyWeaponBonus(string);

		// EAT
		void eat();
		char checkYesNoEat(string); // validator
		bool findFoodInSatchel();
		void addFoodBonus(Item*);

		/*********************************
			  PURE VIRTUAL FUNCTIONS
		**********************************/
		// ROLLING	
		virtual int rollAttack() = 0;
		virtual int rollDef() = 0;
		virtual int rollInitiative() = 0;

		// COMBAT
		virtual int attack(Character*) = 0;
		virtual void defend(int) = 0;

		// MEDICAL
		virtual void health() = 0;

};		
#endif 

//***************************** E O F ********************************
