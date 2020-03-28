#ifndef ABILITY_H
#define ABILITY_H

#include <string>

using namespace std;

class Ability{
public:
  string Name;
  int PP;
  float Accuracy;
  int Damage;
  int FirstUsage;
public:
  Ability(string);
};

#endif
