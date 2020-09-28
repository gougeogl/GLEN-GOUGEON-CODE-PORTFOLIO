# Program Name: Capstone Project makefile
# Author: Glen Gougeon
# Date: 9-17-20
# Description: 		makefile for Game: A Nap in Time  

CXX = g++
CXXFLAGS = -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -pedantic-errors
CXXFLAGS += -g

#OBJS = GameClass.o DieClass.o screenFunctions.o\
QueueClass.o stack.o\
InventoryClass.o item.o\
twoCharactersCombat.o\
LevelClass.o\
UntouchablesOffice_Level.o LiquorRaid_Level.o\
Lindy_Level.o\
Algiers_Level.o RR_Bridge_Level.o\
EstatesGeneral_Level.o Bastille_Level.o GreatFear_Level.o\
OctoberDays_Level.o FlightOfKing_Level.o\
CharacterClass.o\
HeroClass.o\
Henchmen.o Nazi.o\
LouisXVI.o MarieAntoinette.o NobleSword.o PalaceGuards.o\
Priest.o Bourgeoisie.o Peasant.o HouseWife.o\
VehicleClass.o\
Carriage_Vehicle.o FoodCart_Vehicle.o FishingBoat_Vehicle.o SpiritStLouis_Vehicle.o

SRCS = main.cpp GameClass.cpp DieClass.cpp screenFunctions.cpp\
QueueClass.cpp stack.cpp\
InventoryClass.cpp item.cpp\
twoCharactersCombat.cpp\
LevelClass.cpp\
UntouchablesOffice_Level.cpp LiquorRaid_Level.cpp\
Lindy_Level.cpp\
Algiers_Level.cpp RR_Bridge_Level.cpp\
EstatesGeneral_Level.cpp Bastille_Level.cpp GreatFear_Level.cpp\
OctoberDays_Level.cpp FlightOfKing_Level.cpp\
CharacterClass.cpp\
HeroClass.cpp\
Henchmen.cpp Nazi.cpp\
LouisXVI.cpp MarieAntoinette.cpp NobleSword.cpp PalaceGuards.cpp\
Priest.cpp Bourgeoisie.cpp Peasant.cpp HouseWife.cpp\
VehicleClass.cpp\
Carriage_Vehicle.cpp FoodCart_Vehicle.cpp FishingBoat_Vehicle.cpp SpiritStLouis_Vehicle.cpp
  
HEADERS = GameClass.hpp DieClass.hpp screenFunctions.hpp\
QueueClass.hpp stack.hpp\
InventoryClass.hpp item.hpp\
twoCharactersCombat.hpp\
LevelClass.hpp\
UntouchablesOffice_Level.hpp LiquorRaid_Level.hpp\
Lindy_Level.hpp\
Algiers_Level.hpp RR_Bridge_Level.hpp\
EstatesGeneral_Level.hpp Bastille_Level.hpp GreatFear_Level.hpp\
OctoberDays_Level.hpp FlightOfKing_Level.hpp\
CharacterClass.hpp\
HeroClass.hpp\
Henchmen.hpp Nazi.hpp\
LouisXVI.hpp MarieAntoinette.hpp NobleSword.hpp PalaceGuards.hpp\
Priest.hpp Bourgeoisie.hpp Peasant.hpp HouseWife.hpp\
VehicleClass.hpp\
Carriage_Vehicle.hpp FoodCart_Vehicle.hpp FishingBoat_Vehicle.hpp SpiritStLouis_Vehicle.hpp
		
#
#target: dependencies
#	rule to build
#

OBJS:= $(patsubst %.cpp, %.o,$(wildcard *.cpp)) 

main: ${OBJS} ${HEADERS}
	${CXX} ${CXXFLAGS} -o main $(OBJS)
	clear
clean:
	rm *.o main
 
























