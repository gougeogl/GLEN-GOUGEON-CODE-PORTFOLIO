/*****************************************************************************
 * File Name: InventoryClass.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Inventory Class.

******************************************************************************/
#include"InventoryClass.hpp"

// all weights given in U.S. pounds.
// all values in U.S. dollars.

Inventory::Inventory(){}
Inventory::~Inventory() {}

Item* Inventory::aviatorHat()
{
	Item* hat = new Item;

	hat->setTitle("\n   Aviator Cap");
	hat->setDescription("\n\n   Lambskin leather. Carmel brown. Lambswool lining. Metal closure.");
	hat->setType(THING);
	hat->setWeight(0.5);
	hat->setValue(5); // at the time??
	hat->setHealthBonus(2);

	return hat;
}

Item* Inventory::goggles()
{
	Item* goggles = new Item;

	goggles->setTitle("\n   Aviator Goggles");
	goggles->setDescription("\n\n   Leather banded. Clear glass. Two-Piece. Elastic band.");
	goggles->setType(THING);
	goggles->setWeight(0.25);
	goggles->setValue(4);
	goggles->setHealthBonus(2);

	return goggles;
}

Item* Inventory::emergencyKit()
{
	Item* kit = new Item;

	kit->setTitle("\n   Emergency Kit");
	kit->setDescription(
		"\n\n   ===================================="
		"\n   1 air raft with pump and repair kit"
		"\n   1 canteen of water-4 qts."
		"\n   1 Armbrust cup"
		"\n   5 cans of Army emergency rations"
		"\n   1 hunting knife"
		"\n   1 ball of cord"
		"\n   1 ball of string"
		"\n   1 large needle"
		"\n   1 flashlight"
		"\n   4 red flares, sealed in rubber tubes"
		"\n   1 match safe with matches"
		"\n   1 hack-saw blade"
		"\n   ===================================="
	);
	kit->setType(THING);
	kit->setWeight(2.5);
	kit->setValue(3);
	kit->setHealthBonus(8);

	return kit;
}

Item* Inventory::torch()
{
	Item* fire = new Item;

	fire->setTitle("\n   torch");
	fire->setDescription("\n\n   Oak branch. Cloth head. Dipped in oil.");
	fire->setType(THING);
	fire->setWeight(1.15);
	fire->setValue(1.00);

	return fire;
}

Item* Inventory::fedora()
{
	Item* hat = new Item;

	hat->setTitle("\n   hat: Fedora");
	hat->setDescription("\n\n   Stetson felt hat; 100 grams (.22lbs.); black band");
	hat->setType(THING);
	hat->setWeight(0.22);
	hat->setValue(5.00);
	hat->setHealthBonus(2);

	return hat;
}

Item* Inventory::concordance()
{
	Item* book = new Item;

	book->setTitle("\n   English-to-French Concordance");
	book->setDescription("\n\n   Allows conversion of English words to French and vice versa. Useful for communication.");
	book->setType(THING);
	book->setWeight(4.25);
	book->setValue(4.00);
	book->setHealthBonus(2);

	return book;
}

Item* Inventory::ChangeOfClothes()
{
	Item* clothes = new Item;

	clothes->setTitle("\n   a set of clothes");
	clothes->setDescription("\n   Plain workman\'s clothes, useful for disguising royalty.");
	clothes->setWeight(3.5);
	clothes->setValue(5.25);
	clothes->setHealthBonus(2);

	return clothes;
}

// MMMMMMMMMMMMMMMMMMM
//		DOCUMENTS
// MMMMMMMMMMMMMMMMMMM

Item* Inventory::royalSeal()
{
	Item* seal = new Item;

	seal->setTitle("\n   Royal Seal of Approval");
	seal->setDescription("\n\n   Authorizates any document in the Kingdom of France. (1789)");
	seal->setType(DOCUMENT);
	seal->setWeight(0.250);
	seal->setValue(500);
	seal->setHealthBonus(10);

	return seal;
}

Item* Inventory::royalSummons()
{
	Item* paper = new Item;

	paper->setTitle("\n   Royal Summons");
	paper->setDescription("\n\n   Invitation by the King to attend convene an Estates General.");
	paper->setType(DOCUMENT);
	paper->setWeight(0.05);
	paper->setValue(30.50);
	paper->setHealthBonus(5);

	return paper;
}

Item* Inventory::warrant()
{
	Item* arrest = new Item;

	arrest->setTitle("\n   Arrest warrant");
	arrest->setDescription("\n\n   Official court approved warrant of arrest!");
	arrest->setType(DOCUMENT);
	arrest->setWeight(0.05);
	arrest->setValue(5.00);
	arrest->setHealthBonus(-5);

	return arrest;
}


// Evidence against Al Capone
Item* Inventory::caponesLedger()
{
	Item* c_book = new Item;

	c_book->setTitle("\n   Al Capone\'s accounting ledger");
	c_book->setDescription("\n\n   Contains information referencing Capone\'s black market businesses to his legitimate ones.");
	c_book->setType(DOCUMENT);
	c_book->setWeight(0.625);
	c_book->setValue(2000000);

	return c_book;
}

Item* Inventory::AugustDecrees()
{
	Item* aug = new Item;

	aug->setTitle("\n   The August Decrees");
	aug->setDescription("\n\n   Contract to abolish Noble privelages."
		"\n   Formalized by the Nationaly Assembly, but already implemented by"
		"\n   Peasant riots during the Great Fear."
		"\n   Not Ratified by the King");
	aug->setType(DOCUMENT);
	aug->setWeight(0.550);
	aug->setValue(2500);
	aug->setHealthBonus(10);

	return aug;
}

Item* Inventory::DeclarationRights()
{
	Item* doc = new Item;

	doc->setTitle("\n   Declaration of the Rights of Man, and of the Citizen");
	doc->setDescription("\n\n   Declaration of rights to be gauranteed to all Frenchmen."
		"\n   Approved by the National Assembly August 4, 1789, but not by King."
		"\n   Preceded the creation of the Constitution.");
	doc->setType(DOCUMENT);
	doc->setWeight(0.250);
	doc->setValue(1500);
	doc->setHealthBonus(10);

	return doc;
}

Item* Inventory::DeBouilleMap()
{
	Item* map = new Item;

	map->setTitle("\n   De Bouille\'s Route");
	map->setDescription("\n   Description of safe passage to the Fortress of Montmedy");
	map->setType(DOCUMENT);
	map->setWeight(0.250);
	map->setValue(100);
	map->setHealthBonus(10);

	return map;
}

Item* Inventory::phonyPassports()
{
	Item* pp = new Item;

	pp->setTitle("\n   Phony passport documents");
	pp->setDescription("\n   Stating \"Madamme de Korff and Family\".");
	pp->setType(DOCUMENT);
	pp->setWeight(0.625);
	pp->setValue(50.75);
	pp->setHealthBonus(25);

	return pp;
}

// MMMMMMMMMMMMMMMMMMM
//		WEAPONS
// MMMMMMMMMMMMMMMMMMM

// Revolutionary Pike
Item* Inventory::pike()
{
	Item* pike = new Item;

	pike->setTitle("\n   Revolutionary Pike");
	pike->setDescription("\n\n   Weapon of the Sans-Culottes. 6-inch iron tip. 9-foot wood handle.");
	pike->setType(WEAPON);
	pike->setWeight(5.50);
	pike->setValue(2.50);
	pike->setPower(4);

	return pike;
}

Item* Inventory::sabre()
{
	Item* sword = new Item;

	sword->setTitle("\n   French Sabre");
	sword->setDescription("\n\n   Straight-bladed. Single edge. Favored by French officers.");
	sword->setType(WEAPON);
	sword->setWeight(5.65);
	sword->setValue(12.64);
	sword->setPower(3);

	return sword;
}
Item* Inventory::charleville()
{
	Item* musket = new Item;

	musket->setTitle("\n   French Musket");
	musket->setDescription("\n\n   Charleville model 1777 flintlock rifle."
		"\n   Caliber: .69 round-ball. 45-inch smooth-bore barrel.");
	musket->setType(WEAPON);
	musket->setWeight(9.60);
	musket->setValue(8.50);
	musket->setPower(6);

	return musket;
}

// S&W .38 special revolver
Item* Inventory::smith()
{
	Item* SnW = new Item;

	SnW->setTitle("\n   Smith and Wesson Revolver");
	SnW->setDescription("\n\n   Caliber: .38 Special ; 6 shot; double-action;"
		"\n   checkered wood grips, blued finish, brown leather holster included");
	SnW->setType(WEAPON);
	SnW->setWeight(1.25);
	SnW->setValue(3.85);
	SnW->setPower(5);

	return SnW;
}

Item* Inventory::shotgun()
{
	Item* skeet = new Item;
	skeet->setTitle("\n   Double Barrel Shotgun");
	skeet->setDescription("\n\n   Gauge: 12 ; 2 shot; side-by-side;"
		"\n   26-inch barrel; checkered walnut stock.");
	skeet->setType(WEAPON);
	skeet->setWeight(6.25);
	skeet->setValue(1243.59);
	skeet->setPower(10);

	return skeet;
}

Item* Inventory::Thompson()
{
	Item* tommy = new Item;

	tommy->setTitle("\n   Thompson sub-machine-gun");
	tommy->setDescription("\n\n    \"Tommy-Gun\". Fully-Automatic machine gun. Caliber: .45 ACP. 50-round drum.");
	tommy->setType(WEAPON);
	tommy->setWeight(17.00);
	tommy->setValue(25.00);
	tommy->setPower(20);

	return tommy;
}

Item* Inventory::Artillery()
{
	Item* cannon = new Item;

	cannon->setTitle("\n   Cannon");
	cannon->setDescription("\n\n   Heavy Artillery piece. Requires ammunition. Good against fortresses.");
	cannon->setType(HEAVY_WEAPON);
	cannon->setWeight(2000.00);
	cannon->setValue(250.25);
	cannon->setPower(2500);

	return cannon;
}

Item* Inventory::RoundShot()
{
	Item* ball = new Item;

	ball->setTitle("\n   1 Round Shot");
	ball->setDescription("\n\n   4-in. Cannon-Ball for smooth-bore cannon. Black. Heavy.");
	ball->setType(HEAVY_WEAPON);
	ball->setWeight(40.50);
	ball->setValue(10.00);
	
	return ball;
}

// Suitcase of Explosives
Item* Inventory::exploders()
{
	Item* explosives = new Item;

	explosives->setTitle("\n   Suitcase of Explosives");
	explosives->setDescription("\n\n   T.N.T., timers, wire, toolkit, detonater");
	explosives->setType(BOMB);
	explosives->setWeight(15.50);
	explosives->setValue(500.00);
	explosives->setPower(5000);

	return explosives;
}

// MMMMMMMMMMMMMMMMMMMMMM
//			FOOD
// MMMMMMMMMMMMMMMMMMMMMM

Item* Inventory::wineGlass()
{
	Item* glass = new Item;

	glass->setTitle("\n   Wine Glass");
	glass->setDescription("\n\n   A single glass of Merlot Wine, of French origin.");
	glass->setType(FOOD);
	glass->setWeight(0.18);
	glass->setValue(5.00);
	glass->setHealthBonus(2);

	return glass;
}

Item* Inventory::staleBread()
{
	Item* loaf = new Item;

	loaf->setTitle("\n   Loaf of Bread");
	loaf->setDescription("\n\n   Stale loaf of French Bread");
	loaf->setType(FOOD);
	loaf->setWeight(0.25);
	loaf->setValue(12.50);
	loaf->setHealthBonus(2);

	return loaf;
}

Item* Inventory::croutons()
{
	Item* crouton = new Item;

	crouton->setTitle("\n   Croutons");
	crouton->setDescription("\n\n   Best on salads.. but still just bread crumbs.");
	crouton->setType(FOOD);
	crouton->setWeight(0.10);
	crouton->setValue(2.50);
	crouton->setHealthBonus(1);

	return crouton;
}

Item* Inventory::hamburger()
{
	Item* burger = new Item;

	burger->setTitle("\n   Hamburger");
	burger->setDescription("\n\n   Made of dead cow muscle, tomatoes, pickes, lettuce, mayo and mustard.");
	burger->setType(FOOD);
	burger->setWeight(0.25);
	burger->setValue(2.00);
	burger->setHealthBonus(4);

	return burger;
}

Item* Inventory::whiskey()
{
	Item* brew = new Item;

	brew->setTitle("\n   Whisky");
	brew->setDescription("\n\n   Cheap, but effective. Canadian. Amber brown.");
	brew->setType(FOOD);
	brew->setWeight(0.4);
	brew->setValue(1.00);
	brew->setHealthBonus(2);

	return brew;
}

Item* Inventory::bugs()
{
	Item* insects = new Item;

	insects->setTitle("\n   Plate of bugs");
	insects->setDescription("\n\n   Dead bugs. multiple types. side of leaves.");
	insects->setType(FOOD);
	insects->setWeight(0.2);
	insects->setValue(0.0);
	insects->setHealthBonus(1);

	return insects;
}

// ************************************* EOF ************************************

