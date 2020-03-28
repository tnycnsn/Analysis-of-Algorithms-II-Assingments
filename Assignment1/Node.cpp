#include "Ability.h"
#include "Pokemon.h"
#include "Node.h"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <ctime>

using namespace std;

Node(int newP_HP, int newP_PP, int newB_HP, int newB_PP, char Turn, float Prob, int lvlLimit, Node *Upper, Pokemon *Pikachu, Pokemon *Blastoise){ //Constructor
  this->P_HP = newP_HP;
  this->P_PP = newP_PP;
  this->B_HP = newB_HP;
  this->B_PP = newB_PP;
  this->Turn = Turn;
  this->Prob = Prob;
  this->Upper = Upper;
  if (this->Upper) this->Level = this->Upper->Level + 1;
  else this->Level = 0;

  if ((this->Level == lvlLimit) || (Turn == 'P' && P_HP <= 0) || (Turn == 'B' && B_HP <= 0)) this->isLeaf = true;
  else this->isLeaf = false;

  this->Lowers = {};  //initialize as empty list, if not leafnode fill inside
  float next_Prob;
  if (!this->isLeaf){
    if(this->Turn == 'P'){   //pikachu Turn
      for(vector<Ability>::iterator it = Pikachu->AbilityList.begin(); it < Pikachu->AbilityList.end(); it++){
        int nextP_PP = this->P_PP + it->PP;  //Pikachu cast an ability so changed it's PP
        if ((this->Level < it->FirstUsage) || (nextP_PP < 0)) continue; //Round level not enough to use this ability ||OR|| PP is not sufficient to use the Ability
        //Effective Case:
        int nextB_HP = this->B_HP - it->Damage; //Pikachu cast an ability so changed it's opponents HP
        if (this->Level < it->FirstUsage) next_Prob = this->Prob * (1.0 - it->Accuracy / (Pikachu->TotalAccuracy - 100.0)); // Skip cannot be used, so don't count for the Prob calculation
        else next_Prob = this->Prob * (it->Accuracy / Pikachu->TotalAccuracy);
        //Blastoise's HP is decreased, Pikachu's PP is decreased
        Node* lower = new Node(this->P_HP, nextP_PP, nextB_HP, this->B_PP, 'B', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
        this->Lowers.push_back(lower);  //add it to the Lowers list
        //Not Effective Case:
        if (it->Accuracy < 100){
          if (this->Level < it->FirstUsage) next_Prob = this->Prob * (1.0 - it->Accuracy / (Pikachu->TotalAccuracy - 100.0)); // Skip cannot be used, so don't count for the Prob calculation
          else next_Prob = this->Prob * (1.0 - it->Accuracy / Pikachu->TotalAccuracy);
          //Blastoise's HP remains same, Pikachu's PP is decreased
          Node* lower = new Node(this->P_HP, nextP_PP, this->B_HP, this->B_PP, 'B', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
          this->Lowers.push_back(lower);  //add it to the Lowers list
        }
      }
    }else{  //Blastoise Turn  (this->Turn == 'B')
      for(vector<Ability>::iterator it = Blastoise->AbilityList.begin(); it < Blastoise->AbilityList.end(); it++){
        int nextB_PP = this->B_PP + it->PP;  //Pikachu cast an ability so changed it's PP
        if ((this->Level < it->FirstUsage) || (nextB_PP < 0)) continue; //Round level not enough to use this ability ||OR|| PP is not sufficient to use the Ability
        //Effective Case:
        int nextP_HP = this->P_HP - it->Damage; //Pikachu cast an ability so changed it's opponents HP
        if (this->Level < it->FirstUsage) next_Prob = this->Prob * (1.0 - it->Accuracy / (Blastoise->TotalAccuracy - 100.0)); // Skip cannot be used, so don't count for the Prob calculation
        else next_Prob = this->Prob * (it->Accuracy / Blastoise->TotalAccuracy);
        //Pikachu's HP is decreased, Blastoise's PP is decreased
        Node* lower = new Node(nextP_HP, this->P_PP, this->B_HP, nextB_PP, 'P', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
        this->Lowers.push_back(lower);  //add it to the Lowers list
        //Not Effective Case:
        if (it->Accuracy < 100){
          if (this->Level < it->FirstUsage) next_Prob = this->Prob * (1.0 - it->Accuracy / (Blastoise->TotalAccuracy - 100.0)); // Skip cannot be used, so don't count for the Prob calculation
          else next_Prob = this->Prob * (1.0 - it->Accuracy / Blastoise->TotalAccuracy);
          //Pikachu's HP remains same, Blastoise's PP decreased
          Node* lower = new Node(this->P_HP, this->P_PP, this->B_HP, nextB_PP, 'P', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
          this->Lowers.push_back(lower);  //add it to the Lowers list
        }
      }
    }
  }
};

void Node::ShowNode(){
  cout << "Pikachu\t\tHP:\t" << this->P_HP << "\n" << "\t\tPP:\t" << this->P_PP << endl;
  cout << "Blastoise\tHP:\t" << this->B_HP << "\n" << "\t\tPP:\t" << this->B_PP << endl;
  cout << "Stats:\tTurn:\t" << this->Turn << "\n\t\tProb:\t" << this->Prob << "\n\t\tLevel:\t" << this->Level << "\n\t\tisLeaf:\t" << this->isLeaf << "\n\n" << endl;
};

/*
void Node::ShowGraph(int* counter){
  cout << ++counter << endl;
  cout << "Pikachu\t\tHP:\t" << this->P_HP << "\n" << "\t\tPP:\t" << this->P_PP << endl;
  cout << "Blastoise\tHP:\t" << this->B_HP << "\n" << "\t\tPP:\t" << this->B_PP << endl;
  cout << "Stats:\tTurn:\t" << this->Turn << "\n\t\tProb:\t" << this->Prob << "\n\t\tLevel:\t" << this->Level << "\n\t\tisLeaf:\t" << this->isLeaf << "\n\n" << endl;
  for (int i = 0; i < Lowers.size(); i++){
    Lowers[i]->ShowGraph(counter);
  }
};
*/

void Node::BFS(){
  clock_t start = clock();
  int count = 0;

  queue <Node*> NextSearch;
  NextSearch.push(this);
  while (!NextSearch.empty()){
    for (int i = 0; i < NextSearch.front()->Lowers.size(); i++){
      NextSearch.push(NextSearch.front()->Lowers[i]);
    };
    NextSearch.pop();
    count++;
  };
  cout << "Hello There! This is BFS" << endl;
  cout << "Number of Nodes: " << count << endl;
  cout << "Duration: " << (clock() - start) / (double) CLOCKS_PER_SEC << endl;
};

void Node::DFS(){
  clock_t start = clock();
  int count = 0;

  Node *current;
  stack <Node *> NextSearch;
  NextSearch.push(this);
  while (!NextSearch.empty()) {
    current = NextSearch.top(); //save the
    NextSearch.pop();
    count++;
    for (int i = 0; i < current->Lowers.size(); i++) {
      NextSearch.push(current->Lowers[i]);
    };
  };
  cout << "Hello There! This is DFS" << endl;
  cout << "Number of Nodes: " << count << endl;
  cout << "Duration: " << (clock() - start) / (double) CLOCKS_PER_SEC << endl;
};
