#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <ctime>
#include <stack>

using namespace std;

class Ability;
class Pokemon;
class Node;

class Ability{
public:
  string Name;
  int PP;
  float Accuracy;
  int Damage;
  int FirstUsage;
public:
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
};


class Pokemon{
public:
  string Name;
  int HP;
  int PP;
  vector <Ability> AbilityList;
public:
  Pokemon(string Name, int Hp) {  //Constructor
    this->Name = Name;
    this->HP = Hp;  //Constructor
    this->PP = 100;
    this->AbilityList = {};
  };

  void Pokedex(){ //shows pokemon's attributes
    cout << "this is: "<< this->Name << "\tHP is: " << this->HP << "\tPP is: " << this->PP << endl;
    cout << "Ability" << "\t" << "PP" << "\t" << "Accuracy" << "\t" << "Damage" << "\t" << "FirstUsage" << endl;
    for(vector<Ability>::iterator it = this->AbilityList.begin(); it < this->AbilityList.end(); it++){
      cout << it->Name << "\t" << it->PP << "\t" << it->Accuracy << "\t" << it->Damage << "\t" << it->FirstUsage << endl;
    }
  };


  Node* KnockOut(Pokemon* Opponents, int K, char startTurn){  //K = max deepness of the search graph, startTurn = 'P'
    //if (Opponents->HP <= 0) return
    if (this->Name.compare("Pikachu")){
      Node root(this->HP, this->PP, Opponents->HP, Opponents->PP, startTurn, 1.0, K, NULL, this, Opponents); //Entire Graph is constucted by the Node Constructor

      Node* BestNode = &root;
      queue <Node*> NextSearch;
      NextSearch.push(&root);
      while (!NextSearch.empty()){
        if (NextSearch.front()->B_HP < BestNode->B_HP) BestNode = NextSearch.front(); //find the best node for our pokemon
        for (int i = 0; i < NextSearch.front()->Lowers.size(); i++){
          NextSearch.push(NextSearch.front()->Lowers[i]);
        };
        NextSearch.pop();
      };
      if (BestNode->B_PP <= 0) return BestNode; //If opponent is knocked-out terminate
      else {
        this->HP = BestNode->P_HP;  //Update the pokemon's attributes
        this->PP = BestNode->P_PP;  //Update the pokemon's attributes
        Opponents->HP = BestNode->B_HP; //Update the pokemon's attributes
        Opponents->HP = BestNode->B_PP; //Update the pokemon's attributes
        return KnockOut(Opponents, K, BestNode->Turn);  //if couldn't find the KnockOut solution continue to search.
      };
    }else{
      Node root(Opponents->HP, Opponents->PP, this->HP, this->PP, startTurn, 1.0, K, NULL, Opponents, this); //Entire Graph is constucted by the Node Constructor

      Node* BestNode = &root;
      queue <Node*> NextSearch;
      NextSearch.push(&root);
      while (!NextSearch.empty()){
        if (NextSearch.front()->P_HP < BestNode->P_HP) BestNode = NextSearch.front(); //find the best node for our pokemon
        for (int i = 0; i < NextSearch.front()->Lowers.size(); i++){
          NextSearch.push(NextSearch.front()->Lowers[i]);
        };
        NextSearch.pop();
      };
      if (BestNode->P_PP <= 0) return BestNode; //If opponent is knocked-out terminate
      else {
        this->HP = BestNode->B_HP;  //Update the pokemon's attributes
        this->PP = BestNode->B_PP;  //Update the pokemon's attributes
        Opponents->HP = BestNode->P_HP; //Update the pokemon's attributes
        Opponents->HP = BestNode->P_PP; //Update the pokemon's attributes
        return KnockOut(Opponents, K, BestNode->Turn);  //if couldn't find the KnockOut solution continue to search.
      };
    };
  };

};


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
    if (!this->isLeaf){
      float next_Prob;
      float NumberOfUsableAbility = 0.0;
      if(this->Turn == 'P'){   //pikachu Turn
        for(vector<Ability>::iterator it = Pikachu->AbilityList.begin(); it < Pikachu->AbilityList.end(); it++){ //count Pikachu's usable skills
          if (this->Level >= it->FirstUsage) NumberOfUsableAbility++;
        };
        for(vector<Ability>::iterator it = Pikachu->AbilityList.begin(); it < Pikachu->AbilityList.end(); it++){
          int nextP_PP = this->P_PP + it->PP;  //Pikachu cast an ability so changed it's PP
          if ((this->Level < it->FirstUsage) || (nextP_PP < 0)) continue; //Round level not enough to use this ability ||OR|| PP is not sufficient to use the Ability
          //Effective Case:
          int nextB_HP = this->B_HP - it->Damage; //Pikachu cast an ability so changed it's opponents HP
          next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (it->Accuracy / 100.0);
          //Blastoise's HP is decreased, Pikachu's PP is decreased
          Node* lower = new Node(this->P_HP, nextP_PP, nextB_HP, this->B_PP, 'B', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
          this->Lowers.push_back(lower);  //add it to the Lowers list
          //Not Effective Case:
          if (it->Accuracy < 100.0){
            next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (1.0 - it->Accuracy / 100.0);
            //Blastoise's HP remains same, Pikachu's PP is decreased
            Node* lower = new Node(this->P_HP, nextP_PP, this->B_HP, this->B_PP, 'B', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
            this->Lowers.push_back(lower);  //add it to the Lowers list
          }
        }
      }else{  //Blastoise Turn  (this->Turn == 'B')
      for(vector<Ability>::iterator it = Blastoise->AbilityList.begin(); it < Blastoise->AbilityList.end(); it++){  //count Blastoise's usable skills
        if (this->Level >= it->FirstUsage) NumberOfUsableAbility++;
      };
        for(vector<Ability>::iterator it = Blastoise->AbilityList.begin(); it < Blastoise->AbilityList.end(); it++){
          int nextB_PP = this->B_PP + it->PP;  //Pikachu cast an ability so changed it's PP
          if ((this->Level < it->FirstUsage) || (nextB_PP < 0)) continue; //Round level not enough to use this ability ||OR|| PP is not sufficient to use the Ability
          //Effective Case:
          int nextP_HP = this->P_HP - it->Damage; //Pikachu cast an ability so changed it's opponents HP
          next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (it->Accuracy / 100.0);
          //Pikachu's HP is decreased, Blastoise's PP is decreased
          Node* lower = new Node(nextP_HP, this->P_PP, this->B_HP, nextB_PP, 'P', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
          this->Lowers.push_back(lower);  //add it to the Lowers list
          //Not Effective Case:
          if (it->Accuracy < 100.0){
            next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (1.0 - it->Accuracy / 100.0);
            //Pikachu's HP remains same, Blastoise's PP decreased
            Node* lower = new Node(this->P_HP, this->P_PP, this->B_HP, nextB_PP, 'P', next_Prob, lvlLimit, this, Pikachu, Blastoise); //Warning about this line
            this->Lowers.push_back(lower);  //add it to the Lowers list
          }
        }
      }
    }
  };


  void ShowNode(){
    cout << "Pikachu\t\tHP:\t" << this->P_HP << "\n" << "\t\tPP:\t" << this->P_PP << endl;
    cout << "Blastoise\tHP:\t" << this->B_HP << "\n" << "\t\tPP:\t" << this->B_PP << endl;
    cout << "Stats:\tTurn:\t" << this->Turn << "\n\t\tProb:\t" << this->Prob << "\n\t\tLevel:\t" << this->Level << "\n\t\tisLeaf:\t" << this->isLeaf << "\n\n" << endl;
  };


  void ShowGraph(int &counter){
    cout << ++counter << endl;
    cout << "Pikachu\t\tHP:\t" << this->P_HP << "\n" << "\t\tPP:\t" << this->P_PP << endl;
    cout << "Blastoise\tHP:\t" << this->B_HP << "\n" << "\t\tPP:\t" << this->B_PP << endl;
    cout << "Stats:\tTurn:\t" << this->Turn << "\n\t\tProb:\t" << this->Prob << "\n\t\tLevel:\t" << this->Level << "\n\t\tisLeaf:\t" << this->isLeaf << "\n\n" << endl;
    for (int i = 0; i < Lowers.size(); i++){
      Lowers[i]->ShowGraph(counter);
    }
  };


  void LastLayerInfo(){
    if (this->isLeaf) cout << "P_HP:" << this->P_HP << "  P_PP:" << this->P_PP << "  B_HP:" << this->B_HP << "  B_PP:" << this->B_PP << "  PROB:" << this->Prob << endl;
    else{
      for (int i = 0; i < Lowers.size(); i++){
        Lowers[i]->LastLayerInfo();
      }
    }
  };


  void BFS(){
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


  void DFS(){
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
};


int main(){
  clock_t baslangic = clock();

  Pokemon Blastoise("Blastoise", 361);
  Pokemon Pikachu("Pikachu", 273);

  Blastoise.Pokedex();
  Pikachu.Pokedex();

  string line;
  ifstream file;

  file.open("blastoise.txt");
  getline(file, line);
  while(getline(file, line)){
    Ability theAbility(line);
    Blastoise.AbilityList.push_back(theAbility);
  };
  file.close();

  file.open("pikachu.txt");
  getline(file, line);
  while(getline(file, line)){
    Ability theAbility(line);
    Pikachu.AbilityList.push_back(theAbility);
  };

  //int counter = 0;
  Node root(273, 100, 361, 100, 'P', 1.0, 5, NULL, &Pikachu, &Blastoise); //Entire Graph is constucted by the Node Constructor
  //Node root(150, 100, 150, 100, 'P', 1.0, 13, NULL, &Pikachu, &Blastoise); //Entire Graph is constucted by the Node Constructor
  //root.ShowGraph(counter);
  //root.LastLayerInfo();

  root.BFS();
  //root.DFS();

  //cout << "\nTOTAL SURE: " << (clock() - baslangic) / (double) CLOCKS_PER_SEC << endl;

  return 0;
}
