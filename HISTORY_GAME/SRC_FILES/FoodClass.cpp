#include"FoodClass.hpp"

Food::Food(){}
Food::~Food(){}

Item* Food::hamburger()
{
	Item* burger = new Item;

	burger->setTitle("\n   Hamburger");
	burger->setDescription("\n\n   Right now Francis, someone in America is pushing an electric squeezer down a garbage disposal"
		"\n   and saying it's busted. Somebody else is opening a can of chocolate-flavored soup"
		"\n   \'cause the car wouldn't start, to eat out - they really wanted a cheeseburger.");
	burger->setWeight(0.25);
	burger->setValue(2.00);
	burger->setHealthBonus(1);

	return burger;
}

Item* Food::whiskey()
{
	Item* brew = new Item;

	brew->setTitle("\n   Whisky");
	brew->setDescription("\n   Cheap, but effective. Amber brown.");
	brew->setWeight(0.4);
	brew->setValue(1.00);
	brew->setHealthBonus(2);

	return brew;
}

Item* Food::bugs()
{
	Item* insects = new Item;

	insects->setTitle("\n   Plate of bugs");
	insects->setDescription("\n   Dead bugs. multiple types. side of leaves.");
	insects->setWeight(0.2);
	insects->setValue(0.0);
	insects->setHealthBonus(1);

	return insects;
}

Item* Food::blueMilk()
{
	Item* b_milk = new Item;

	b_milk->setTitle("\n   Blue Milk");
	b_milk->setDescription("\n   Bantha produced. Takes some getting used to.");
	b_milk->setWeight(1.2);
	b_milk->setValue(0.75);
	b_milk->setHealthBonus(2);

	return b_milk;
}