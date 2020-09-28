/*****************************************************************************
 * File Name: item.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Specification for Item Class. Primary structure
					for data related to Weapons, Food, Documents etc.
					found, lost, or won during gameplay. An 'Item'
					performs the 'is a' relationship. Some member vars
					only used for printing in game-play.

 *****************************************************************************/
#ifndef GOUGEON_ITEM_CLASS_HPP
#define GOUGEON_ITEM_CLASS_HPP

#include<cstdlib>
#include<iostream>
#include<string>
using std::cin;
using std::cout;
using std::string;

enum WOT { WEAPON = 1, HEAVY_WEAPON, BOMB, FOOD, DOCUMENT, THING /* default */ };

class Item
{
	private:
		string title;
		string description;
		enum WOT type;
		double weight;
		double value;
		int healthBonus;
		int power; 

	public:
		Item();
		~Item();

		void setTitle(string);
		string getTitle() const;

		void setDescription(string);
		string getDescription() const;

		void setType(enum WOT);
		enum WOT getType();

		void setWeight(double);
		double getWeight() const;

		void setValue(double);
		double getValue() const;

		void setHealthBonus(int);
		int getHealthBonus() const;

		void setPower(int);
		int getPower();

		void printItem();
};
#endif 

// ************************************* EOF ************************************
