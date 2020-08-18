#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <ctime>
#include <stack>
#include <sstream>  //to debug a problem of standard lib

using namespace std;


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

  Ability(const Ability &ability){  //Copy Constructor
    //ability->Name.copy(this->Name, ability->Name.size(), 0);
    this->Name = ability.Name;
    this->PP = ability.PP;
    this->Accuracy = ability.Accuracy;
    this->Damage = ability.Damage;
    this->FirstUsage = ability.FirstUsage;
  };

  void Show(){
    cout << this->Name << "\t" << this->PP << "\t" << this->Accuracy << "\t" << this->Damage << "\t" << this->FirstUsage << endl;
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


  Pokemon(const Pokemon &pokemon){  //Copy Constructor
    this->Name = pokemon.Name;
    this->HP = pokemon.HP;
    this->PP = pokemon.PP;
    this->AbilityList = {};

    for (int i = 0; i < pokemon.AbilityList.size(); i++){
      Ability* copyAbility = new Ability(pokemon.AbilityList[i]);
      this->AbilityList.push_back(*copyAbility);
    };
  };


  void Pokedex(){ //shows pokemon's attributes
    cout << "this is: "<< this->Name << "\tHP is: " << this->HP << "\tPP is: " << this->PP << endl;
    cout << "Ability" << "\t" << "PP" << "\t" << "Accuracy" << "\t" << "Damage" << "\t" << "FirstUsage" << endl;
    for(vector<Ability>::iterator it = this->AbilityList.begin(); it < this->AbilityList.end(); it++){
      it->Show();
    };
  };
};


class Node{
public:
  Node *Parent;
  int P_HP; //pikachu hp
  int P_PP; //pikachu hp
  int B_HP; //blastoise hp
  int B_PP; //blastoise hp
  char Turn;  //Turn that indicates Pikachu or Blastoise
  float Prob;
  int Level;
  bool isLeaf;
  string AbilityUsed;
  bool Effective;
  vector <Node*> Lowers;
public:
  Node(int newP_HP, int newP_PP, int newB_HP, int newB_PP, char Turn, float Prob, int lvlLimit, Node *Upper,string AbilityUsed, bool Effective, Pokemon *Pikachu, Pokemon *Blastoise){ //Constructor
    this->P_HP = newP_HP;
    this->P_PP = newP_PP;
    this->B_HP = newB_HP;
    this->B_PP = newB_PP;
    this->Turn = Turn;
    this->Prob = Prob;
    this->Parent = Upper;
    this->AbilityUsed = AbilityUsed;
    this->Effective  = Effective;  //initialize as true, if wouldn't Effective then turn it to the false

    if (this->Parent) this->Level = this->Parent->Level + 1;
    else this->Level = 0;

    if ((this->Level == lvlLimit) || (Turn == 'P' && P_HP <= 0) || (Turn == 'B' && B_HP <= 0)) this->isLeaf = true;
    else this->isLeaf = false;

    this->Lowers = {};  //initialize as empty list, if not leafnode fill inside
    if (!this->isLeaf){
      float next_Prob;
      float NumberOfUsableAbility = 0.0;
      if(this->Turn == 'P'){   //pikachu Turn
        for(vector<Ability>::iterator it = Pikachu->AbilityList.begin(); it < Pikachu->AbilityList.end(); it++){ //count Pikachu's usable skills
          if ((this->Level >= it->FirstUsage) && (this->P_PP >= -it->PP)) NumberOfUsableAbility++;  //AND exclude abilities where PP not sufficient
        };
        for(vector<Ability>::iterator it = Pikachu->AbilityList.begin(); it < Pikachu->AbilityList.end(); it++){
          int nextP_PP = this->P_PP + it->PP;  //Pikachu cast an ability so changed it's PP
          if ((this->Level < it->FirstUsage) || (nextP_PP < 0)) continue; //Round level not enough to use this ability ||OR|| PP is not sufficient to use the Ability
          //Effective Case:
          int nextB_HP;
          if ((this->B_HP-it->Damage) < 0) nextB_HP = 0;
          else nextB_HP = this->B_HP - it->Damage; //Pikachu cast an ability so changed it's opponents HP
          next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (it->Accuracy / 100.0);
          //Blastoise's HP is decreased, Pikachu's PP is decreased
          Node* lower = new Node(this->P_HP, nextP_PP, nextB_HP, this->B_PP, 'B', next_Prob, lvlLimit, this, it->Name, true, Pikachu, Blastoise); //The ability it->Name used and its Effective
          this->Lowers.push_back(lower);  //add it to the Lowers list

          //Not Effective Case:
          if (it->Accuracy < 100.0){
            next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (1.0 - it->Accuracy / 100.0);
            //Blastoise's HP remains same, Pikachu's PP is decreased
            Node* lower = new Node(this->P_HP, nextP_PP, this->B_HP, this->B_PP, 'B', next_Prob, lvlLimit, this, it->Name, false, Pikachu, Blastoise); //The ability it->Name used and its Not Effective
            this->Lowers.push_back(lower);  //add it to the Lowers list
          }
        }
      }else{  //Blastoise Turn  (this->Turn == 'B')
      for(vector<Ability>::iterator it = Blastoise->AbilityList.begin(); it < Blastoise->AbilityList.end(); it++){  //count Blastoise's usable skills
        if ((this->Level >= it->FirstUsage) && (this->B_PP >= -it->PP)) NumberOfUsableAbility++; //AND exclude abilities where PP not sufficient
      };
        for(vector<Ability>::iterator it = Blastoise->AbilityList.begin(); it < Blastoise->AbilityList.end(); it++){
          int nextB_PP = this->B_PP + it->PP;  //Pikachu cast an ability so changed it's PP
          if ((this->Level < it->FirstUsage) || (nextB_PP < 0)) continue; //Round level not enough to use this ability ||OR|| PP is not sufficient to use the Ability
          //Effective Case:
          int nextP_HP;
          if ((this->P_HP-it->Damage) < 0) nextP_HP = 0;
          else nextP_HP = this->P_HP - it->Damage; //Pikachu cast an ability so changed it's opponents HP
          next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (it->Accuracy / 100.0);
          //Pikachu's HP is decreased, Blastoise's PP is decreased
          Node* lower = new Node(nextP_HP, this->P_PP, this->B_HP, nextB_PP, 'P', next_Prob, lvlLimit, this, it->Name, true, Pikachu, Blastoise); //The ability it->Name used and its Effective
          this->Lowers.push_back(lower);  //add it to the Lowers list

          //Not Effective Case:
          if (it->Accuracy < 100.0){
            next_Prob = this->Prob * (1.0/NumberOfUsableAbility) * (1.0 - it->Accuracy / 100.0);
            //Pikachu's HP remains same, Blastoise's PP decreased
            Node* lower = new Node(this->P_HP, this->P_PP, this->B_HP, nextB_PP, 'P', next_Prob, lvlLimit, this, it->Name, false, Pikachu, Blastoise); //The ability it->Name used and its Not Effective
            this->Lowers.push_back(lower);  //add it to the Lowers list
          };
        }
      }
    }
  };


  void PromtMove(){
    if (this->Parent){  //if this->Parent exists  (NOT NULL)
      if (this->Turn == 'B'){ //Pikachu used ability before this node
        if (this->Effective){ //Its effective
          cout << "Pikachu used " << this->AbilityUsed << ". It's <effective>." << endl;
        }else{  //Its Nonefective
          cout << "Pikachu used " << this->AbilityUsed << ". It's <noneffective>." << endl;
        };
      }else{  //Pikachu used ability before this node
        if (this->Effective){ //Its Effective
          cout << "Blastoise used " << this->AbilityUsed << ". It's <effective>." << endl;
        }else{  //Its Nonefective
          cout << "Blastoise used " << this->AbilityUsed << ". It's <noneffective>." << endl;
        };
      }
    };
    //Else, do nothing
  };


  void ShowNode(){
    cout << "Pikachu\t\tHP:\t" << this->P_HP << "\n" << "\t\tPP:\t" << this->P_PP << endl;
    cout << "Blastoise\tHP:\t" << this->B_HP << "\n" << "\t\tPP:\t" << this->B_PP << endl;
    cout << "Stats:\tTurn:\t" << this->Turn << "\n\t\tProb:\t" << this->Prob << "\n\t\tLevel:\t" << this->Level << "\n\t\tisLeaf:\t" << this->isLeaf << endl;
    this->PromtMove();
  };


  void ShowGraph(){
    cout << "Pikachu\t\tHP:\t" << this->P_HP << "\n" << "\t\tPP:\t" << this->P_PP << endl;
    cout << "Blastoise\tHP:\t" << this->B_HP << "\n" << "\t\tPP:\t" << this->B_PP << endl;
    cout << "Stats:\tTurn:\t" << this->Turn << "\n\t\tProb:\t" << this->Prob << "\n\t\tLevel:\t" << this->Level << "\n\t\tisLeaf:\t" << this->isLeaf << "\n\n" << endl;
    for (int i = 0; i < Lowers.size(); i++){
      Lowers[i]->ShowGraph();
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


class Match{
public:
  Pokemon* OurPokemon;
  Pokemon* Opponent;
public:
  Match(const Pokemon &Our, const Pokemon &Opp){  //Constructor
    OurPokemon = new Pokemon(Our);
    Opponent = new Pokemon(Opp);
  };


  Node* KnockOut(int K){  //returns the Node where the Opponent is Knocked Out first
    Node* root;
    if (!OurPokemon->Name.compare("Pikachu")){ //If our Pokemon is Pikachu
      root = new Node(OurPokemon->HP, OurPokemon->PP, Opponent->HP, Opponent->PP, 'P', 1.0, K, NULL, "", true, OurPokemon, Opponent); //Entire Graph is constucted by the Node Constructor
    }else{
      root = new Node(Opponent->HP, Opponent->PP, OurPokemon->HP, OurPokemon->PP, 'P', 1.0, K, NULL, "", true, Opponent, OurPokemon); //Entire Graph is constucted by the Node Constructor
    };

    Node* currentSubroot = root;
    Node* BestNode = root; //Stores the Opponent's lowest HP node in the graph with K level

    queue <Node*> NextSearch;
    while ((!OurPokemon->Name.compare("Pikachu") && BestNode->B_HP > 0) || (!OurPokemon->Name.compare("Blastoise") && BestNode->P_HP > 0)){
      //Iterate till BestNode's Opponent HP reach 0
      NextSearch.push(currentSubroot);
      while (!NextSearch.empty()){
        //BestNode->ShowNode();
        for (int i = 0; i < NextSearch.front()->Lowers.size(); i++){
          NextSearch.push(NextSearch.front()->Lowers[i]);
        };
        if (!OurPokemon->Name.compare("Pikachu")){  //Our pokemon is Pikachu
          //Choose the Node where Opponent's HP is minimum if that HP value is reached with less PP consumption take that node
          if (NextSearch.front()->B_HP < BestNode->B_HP || (NextSearch.front()->B_HP == BestNode->B_HP && BestNode->P_PP > NextSearch.front()->P_PP)) BestNode = NextSearch.front(); //compare with Opponent's HP
          if (BestNode->B_HP <= 0) return BestNode; //if Opponent's (Blastoise) HP is 0 or lower end our search
        }else{ //Our pokemon is Blastoise
          //Choose the Node where Opponent's HP is minimum if that HP value is reached with less PP consumption take that node
          if (NextSearch.front()->P_HP < BestNode->P_HP || (NextSearch.front()->P_HP == BestNode->P_HP && BestNode->B_PP > NextSearch.front()->B_PP)) BestNode = NextSearch.front(); //compare with Opponent's HP
          if (BestNode->P_HP <= 0) return BestNode; //if Opponent's (Pikachu) HP is 0 or lower end our search
        };
        NextSearch.pop();
      };
      // Search ended with that K level graph if couldn't find a KnockOut point search again
      if (!OurPokemon->Name.compare("Pikachu")){ //If our Pokemon is Pikachu
        currentSubroot = new Node(BestNode->P_HP, BestNode->P_PP, BestNode->B_HP, BestNode->B_PP, BestNode->Turn, BestNode->Prob,
          K+BestNode->Level, BestNode->Parent, BestNode->AbilityUsed, BestNode->Effective, OurPokemon, Opponent); //continue to constuct Graph from the BestNode
      }else{
        currentSubroot = new Node(BestNode->P_HP, BestNode->P_PP, BestNode->B_HP, BestNode->B_PP, BestNode->Turn, BestNode->Prob,
          K+BestNode->Level, BestNode->Parent, BestNode->AbilityUsed, BestNode->Effective, Opponent, OurPokemon); //continue to constuct Graph from the BestNode
      };
      BestNode = currentSubroot;  //assign BestNode our additional graph rather then dull old BestNode
    };
  };
};


int main(int argc, char* argv[]){

  Pokemon Blastoise("Blastoise", 361);
  Pokemon Pikachu("Pikachu", 273);

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


  string part = argv[1];  //indicates which part do you want to run
  int K;
  if (part == "part1"){
    K = stoi(argv[2]);  //Max-level
    Node root(273, 100, 361, 100, 'P', 1.0, K, NULL, "", true, &Pikachu, &Blastoise); //Entire Graph is constucted by the Node Constructor, initialize AbilityUsed as "", and Effective doesn't care for the Root Node
    root.LastLayerInfo();
  }
  else if (part == "part2"){
    K = stoi(argv[2]);  //Max-level
    string AlgoType = argv[3];
    Node root(273, 100, 361, 100, 'P', 1.0, K, NULL, "", true, &Pikachu, &Blastoise); //Entire Graph is constucted by the Node Constructor, initialize AbilityUsed as "", and Effective doesn't care for the Root Node
    if (AlgoType == "dfs") root.DFS();
    else if (AlgoType == "bfs") root.BFS();
    else cout << "TYPE A valid algoritm, try lowercase MAYBE!" << endl;
  }
  else if (part == "part3"){
    string MyPokemon = argv[2];
    int BatchSize = 5;
    Node *Best, *current;
    if (MyPokemon == "pikachu"){
      Match PikaVsBlast(Pikachu, Blastoise);
      Best = PikaVsBlast.KnockOut(BatchSize);

      current = Best;
      while (current->Parent){ //Traversing and Prompting The Moves in Match
        current->PromtMove();
        current = current->Parent;
      };
      cout << "Level count: " << Best->Level << endl;
      cout << "Probability: " << Best->Prob << endl;
    }
    else if (MyPokemon == "blastoise"){
      Match BlastVsPika(Blastoise, Pikachu);
      Best = BlastVsPika.KnockOut(BatchSize);

      current = Best;
      while (current->Parent){ //Traversing and Prompting The Moves in Match
        current->PromtMove();
        current = current->Parent;
      };
      cout << "Level count: " << Best->Level << endl;
      cout << "Probability: " << Best->Prob << endl;
    }
    else cout << "Wrong initial inputs, try lowercase pikachu/blastoise" << endl;
  }
  else{
    cout << "Wrong initial input, try lowercase" << endl;
  };

  return 0;
}
