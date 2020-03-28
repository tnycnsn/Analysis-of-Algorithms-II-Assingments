#ifndef POKEMON_H
#define POKEMON_H

#include "Ability.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Pokemon{
public:
  string Name;
  int HP;
  int PP;
  vector <Ability> AbilityList;
  float TotalAccuracy;
public:
  Pokemon(string Name, int Hp); //Constructor
  void Pokedex(); //shows pokemon's attributes
  void LearnAbility(Ability newAbility);  //Add ability to pokemon
};

#endif
