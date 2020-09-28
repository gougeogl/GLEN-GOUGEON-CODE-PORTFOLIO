/*****************************************************************************
 * File Name: main.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:	 main function wrapper for the game.

 *****************************************************************************/
#include<ctime>
#include"GameClass.hpp"

int main(int argc, char* argv[])
{
	time_t seed;
	seed = time(0);
	srand((unsigned int)seed);

	Game map;
	map.enterPrimaryGameMenu();
	return 0;
}

//***************************** E O F ********************************
