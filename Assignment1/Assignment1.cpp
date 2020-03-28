#include "Ability.h"
#include "Pokemon.h"
#include "Node.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctime>

//https://stackoverflow.com/questions/19549793/c-classes-in-separate-file

using namespace std;

int main(){
  clock_t baslangic = clock();

  Pokemon Blastoise("Blastoise", 361);
  Pokemon Pikachu("Pikachu", 273);

  //Blastoise.Pokedex();
  //Pikachu.Pokedex();

  string line;
  ifstream file;

  file.open("blastoise.txt");
  getline(file, line);
  while(getline(file, line)){
    Ability theAbility(line);
    Blastoise.LearnAbility(theAbility);
  };
  file.close();

  file.open("pikachu.txt");
  getline(file, line);
  while(getline(file, line)){
    Ability theAbility(line);
    Pikachu.LearnAbility(theAbility);
  };

  int counter = 0;
  Node root(273, 100, 361, 100, 'P', 1.0, 2, NULL, &Pikachu, &Blastoise); //Entire Graph is constucted by the Node Constructor
  //root.ShowGraph(&counter);

  root.BFS();
  //root.DFS();

  //cout << "\nTOTAL SURE: " << (clock() - baslangic) / (double) CLOCKS_PER_SEC << endl;

  return 0;
}
