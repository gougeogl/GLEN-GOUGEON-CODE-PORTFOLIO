/*****************************************************************************
 * File Name: GreatFear_Level.hpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20

 * Description: Function Specifications for Great Fear (Level) Class.

	The Great Fear refers to rumors amongst the French peasantry that their
	noble landlords had sent a vagrant army to destroy and or steal their
	crops during a time of famine. The peasantry rose up and burned their
	masters' chateau and boards of obligations (a type of feudal contract).
	In this level, the player is tasked with burning down a chateau. 
	Circumstance may have the player attempt to burn down a chateau and get
	a special document. Fighting a noble, or 4 peasants is also possible.
	As a secondary objective, the player can raid a food cart and receive
	several items to their satchel (queue).

 *****************************************************************************/
#ifndef GGOUGEON_GREAT_FEAR_HPP
#define GGOUGEON_GREAT_FEAR_HPP

#include"LevelClass.hpp"

class GreatFear : public Level
{
	private:
		bool torchLit;
		bool readIntro;
		bool burnedChateau;
		bool robbedFoodCart;

	public:
		GreatFear();
		virtual ~GreatFear();

		// virtual functions
		virtual void search(Character*);
		virtual void action(Character*);

		// ACTION
		void intro();
		void dialogue();
		bool checkTorch(Character* hero, bool* burning);
		void fightNoble(Character* hero);
		void fightFourPeasants(Character*);
		void declarationFound(Character*);

		// secondary objective (ACTION)
		void attackFoodCart(Character* villain);

};
#endif

//***************************** E O F ********************************
