#include "Ability.h"
#include <string>

using namespace std;

Ability(string line){ //Constructor
  size_t pos = 0;

  pos = line.find(",");
  this->Name = line.substr(0, pos);
  line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = ","

  pos = line.find(",");
  this->PP = stoi(line.substr(0, pos));
  line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = ","

  pos = line.find(",");
  this->Accuracy = (float)stoi(line.substr(0, pos));
  line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = ","

  pos = line.find(",");
  this->Damage = stoi(line.substr(0, pos));
  line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = ","

  pos = line.find(",");
  this->FirstUsage = stoi(line.substr(0, pos));
  //line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = ","
};
