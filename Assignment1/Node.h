#ifndef NODE_H
#define NODE_H

#include "Pokemon.h"
#include "Ability.h"
#include <vector>

using namespace std;

class Node{
public:
  Node *Upper;
  int P_HP; //pikachu hp
  int P_PP; //pikachu hp
  int B_HP; //blastoise hp
  int B_PP; //blastoise hp
  char Turn;  //Turn that indicates Pikachu or Blastoise
  float Prob;
  int Level;
  bool isLeaf;
  vector <Node*> Lowers;
public:
  Node(int newP_HP, int newP_PP, int newB_HP, int newB_PP, char Turn, float Prob, int lvlLimit, Node *Upper, Pokemon *Pikachu, Pokemon *Blastoise); //Constructor
  void ShowNode(); //Shows attributes of only one node
  //void ShowGraph(int* counter); //Shows entire graph by recursive calls
  void BFS(); //Breath First Search in graph
  void DFS(); //Depth First Search in graph
};

#endif
