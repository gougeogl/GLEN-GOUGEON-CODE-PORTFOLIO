/*****************************************************************************
 * File Name: InventoryClass.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for InventoryClass.hpp

	Class Description:

		Inventory of all Items used in game. Allows instantiation
		on the heap (and on the fly during development) of items
		used in gameplay. Each function call returns an allocated
		Item* that will go into a Level/level's itemStack, enqueued 
		to a Character's queue(satchel), or dropped into a level's 
		droppedItems stack.

 *****************************************************************************/
#ifndef GGOUGEON_INVENTORY_CLASS_HPP
#define GGOUGEON_INVENTORY_CLASS_HPP

#include"item.hpp"

class Inventory
{
public:
	Inventory();
	~Inventory();

	// each function call will create and return that item

	// THINGS
	Item* aviatorHat();
	Item* goggles();
	Item* emergencyKit();
	Item* torch();
	Item* fedora();
	Item* concordance();
	Item* ChangeOfClothes();

	// DOCUMENTS
	Item* royalSeal();
	Item* royalSummons();
	Item* warrant();
	Item* caponesLedger();
	Item* AugustDecrees();
	Item* DeclarationRights();
	Item* DeBouilleMap();
	Item* phonyPassports();

	// WEAPONS
	Item* pike();
	Item* sabre();
	Item* charleville();
	Item* smith();
	Item* shotgun();
	Item* Thompson();

	// HEAVY WEAPONS
	Item* Artillery();
	Item* RoundShot();

	// BOMB
	Item* exploders();

	// FOOD
	Item* wineGlass();
	Item* staleBread();
	Item* croutons();
	Item* hamburger();
	Item* whiskey();
	Item* bugs();

};
#endif

// ************************************* EOF ************************************
