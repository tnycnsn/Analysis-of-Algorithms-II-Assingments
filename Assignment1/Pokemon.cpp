#include "Ability.h"
#include "Pokemon.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Pokemon(string Name, int Hp) {  //Constructor
  this->Name = Name;
  this->HP = Hp;
  this->PP = 100;
  this->AbilityList = {};
  this->TotalAccuracy = 0;
};

void Pokemon::Pokedex(){ //shows pokemon's attributes
  cout << "this is: "<< this->Name << "\tHP is: " << this->HP << "\tPP is: " << this->PP << endl;
  cout << "Ability" << "\t" << "PP" << "\t" << "Accuracy" << "\t" << "Damage" << "\t" << "FirstUsage" << endl;
  for(vector<Ability>::iterator it = this->AbilityList.begin(); it < this->AbilityList.end(); it++){
    cout << it->Name << "\t" << it->PP << "\t" << it->Accuracy << "\t" << it->Damage << "\t" << it->FirstUsage << endl;
  }
};

void Pokemon::LearnAbility(Ability newAbility){ //Adds ability to the pokemon
  this->TotalAccuracy += newAbility.Accuracy;
  this->AbilityList.push_back(newAbility);
};
