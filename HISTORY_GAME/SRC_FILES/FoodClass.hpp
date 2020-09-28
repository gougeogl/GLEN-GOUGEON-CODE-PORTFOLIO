#ifndef GGOUGEON_FOOD_CLASS_HPP
#define GGOUGEON_FOOD_CLASS_HPP

#include"item.hpp"
//#include"stack.hpp"

class Food
{
	private:
			//Item *burger;
			//Item *brew;
			//Item *insects;
			//Item *b_milk;

	public:
			Food();
			~Food();
			//bool DeleteItem(Item*);

			Item* hamburger();
			Item* whiskey();
			Item* bugs();
			Item* blueMilk();

};
#endif
