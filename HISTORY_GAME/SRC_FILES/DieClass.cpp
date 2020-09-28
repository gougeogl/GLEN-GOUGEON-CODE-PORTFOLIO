/*****************************************************************************
 * File Name: DieClass.cpp
 * Programmer: Glen Gougeon
 * Last-Mod: 9-4-20
 * Game Title: A Nap in Time, A History Major's Unexpected Journey

 * Description:		Definitions for Die Class :

 *****************************************************************************/
#include"DieClass.hpp"
		
Die::Die(){} // constructor 
Die::~Die(){} // destructor
	
int Die::six()
{
	int result = 0;

	result = rand() % 6 + 1;
	return result;	
}

int Die::eight()
{
	int result = 0;

	result = rand() % 8 + 1;
	return result;
}

int Die::ten() 
{
	int result = 0;

	result = rand() % 10 + 1;
	return result;	
}

int Die::twelve() 
{
	int result = 0;

	result = rand() % 12 + 1;
	return result;	
}

int Die::twenty()
{
	int result = 0;

	result = this->ten() + this->ten();
	return result;
}

//********************************* EOF *******************************



































