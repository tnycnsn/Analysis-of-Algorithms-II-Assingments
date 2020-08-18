#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

bool in(std::vector<int> vec, int search){
  for (int i = 0; i < vec.size(); i++){
    if (vec[i] == search) return true;
  }
  return false;
};

class AdjMtx{
public:
  int N;
  int JH;
  int JD;
  int LH;
  int LD;
  std::vector<std::vector <int> > Matrix;

  AdjMtx(std::string fileName){

    std::ifstream file;
    file.open(fileName);

    std::string line;
    getline(file, line);  //Set Matrix Size
    this->N = stoi(line);

    getline(file, line);  //JH JD LH LD
    size_t pos = 0;

    pos = line.find(" ");
    this->JH = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    pos = line.find(" ");
    this->JD = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    pos = line.find(" ");
    this->LH = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    pos = line.find(" ");
    this->LD = std::stoi(line.substr(0, pos));

    for (int i = 0; i < this->N; i++){  //inserting Row
      std::vector<int> row_i;
      for (int j = 0; j < this->N; j++){
        row_i.push_back(0);
      };
      this->Matrix.push_back(row_i);
    };

    while (getline(file, line)){
      this->insertEdge(line);
    };
  };


  void ShowMatrix(){
    std::cout << "JH: " << this->JH << " JD: " << this->JD << " LH: " << this->LH << " LD: " << this->LD << std::endl;

    for (int i = 0; i < this->N; i++){
      for (int j = 0; j < this->N; j++){
        std::cout << this->Matrix[i][j] << "\t";
      }
      std::cout << std::endl;
    }
  };


  void insertEdge(std::string line){
    size_t pos = 0;

    pos = line.find(" ");
    int source = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    pos = line.find(" ");
    int target = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    this->Matrix[source][target] = std::stoi(line);
  };


  std::vector<int> calcPath(int source, int destination, int bannedNode=-1){  //if bannedNode is default(-1) banned node doesnt exist

    std::vector<int> visited; //initially empty
    std::vector<int> distances;
    std::vector<std::vector<int> > paths;   //paths till the nodes

    for (int i = 0; i < this->N; i++){
      std::vector<int> element_path;
      element_path.push_back(source);
      paths.push_back(element_path);
    };

    for (int i = 0; i < this->N; i++){
      distances.push_back(std::numeric_limits<int>::max());  //Initialize as INFINITE
    };
    distances[source] = 0;  //since we are at source make it's distance 0

    if (bannedNode != -1) visited.push_back(bannedNode); //there is a bannedNode
    visited.push_back(source);  //since we are at source add it to the visited list

    while (visited.size() < this->N) {  //while there are unvisited nodes exists
      for (int i = 0; i < this->N; i++){  //Update distances
        if (this->Matrix[visited.back()][i] != 0){  //this nodes can be reachable from the last visited nod and NOT bannedNode
          if (distances[i] == std::numeric_limits<int>::max()){ //if we didnt encounter with this node before (since its distance set to INF)
            distances[i] = this->Matrix[visited.back()][i] + distances[visited.back()]; //update it immediately
            paths[i] = paths[visited.back()]; //we encounter with a new node so spread our path knowledge
            paths[i].push_back(i); //push our node
          }
          else if (distances[i] > distances[visited.back()] + this->Matrix[visited.back()][i]){
            distances[i] = distances[visited.back()] + this->Matrix[visited.back()][i]; //new route is better
            paths[i].clear(); //clear the old route
            paths[i] = paths[visited.back()]; //copy the route till the node that offers better node
            paths[i].push_back(i); //push our node
          };
        };
      };

      int nearest; //Find nearest
      for (int i = 0; i < this->N; i++){
        if (!in(visited, i)){
          nearest = i;   //Initialize nearest as the first unvisited node
          break;
        }
      };
      for (int i = 0; i < this->N; i++){  //search for the nearest node among the unvisited nodes
        if (!in(visited, i) && (distances[i] < distances[nearest])) nearest = i;
      };

      visited.push_back(nearest);
      if (nearest == destination) break; //person reached the destination
    };

    return paths[destination];
  };


  std::vector<int> getTimeAcc(std::vector<int> nodeVector, int initial){
    std::vector<int> timeAcc;
    timeAcc.push_back(initial);
    for (int i = 1; i < nodeVector.size(); i++){
      timeAcc.push_back(timeAcc[i-1] + this->Matrix[nodeVector[i-1]][nodeVector[i]]);
    };
    return timeAcc;
  };


  int intersectIn(std::vector<int> TimeLucy, std::vector<int> PathLucy, std::vector<int> TimeJoseph, std::vector<int> PathJoseph){

    int time = 0;
    int indexLucy = 0;
    int indexJoseph = 0;

    while (indexLucy < PathLucy.size() || indexJoseph < PathJoseph.size()){
      if (time == TimeLucy[indexLucy] && time == TimeJoseph[indexJoseph] && PathLucy[indexLucy] == PathJoseph[indexJoseph]) return PathLucy[indexLucy]; //return intersected point
      else if (TimeLucy[indexLucy] >= TimeJoseph[indexJoseph]){
        indexJoseph++;
        if (indexJoseph == TimeJoseph.size()) break;
        else time = TimeJoseph[indexJoseph];
      }
      else if (TimeLucy[indexLucy] < TimeJoseph[indexJoseph]){
        indexLucy++;
        if (indexLucy == TimeLucy.size()) break;
        else time = TimeLucy[indexLucy];
      }
    };
    return -1;  //if not intersected return -1 as default
  };


  void PrintResult(std::vector<int> LucyForward, std::vector<int> LucyBackward, std::vector<int> JosephForward, std::vector<int> JosephBackward){
    std::vector<int> LucyForwardTime, LucyBackwardTime, JosephForwardTime, JosephBackwardTime;  //arriving times for each node
    //I decided to recalculate vectors for time again in order to sustain readibility
    LucyForwardTime = this->getTimeAcc(LucyForward, 0); //starts from 0
    LucyBackwardTime = this->getTimeAcc(LucyBackward, LucyForwardTime.back()+30); //starts from 30 min after arriving the destination
    JosephForwardTime = this->getTimeAcc(JosephForward, 0); //starts from 0
    JosephBackwardTime = this->getTimeAcc(JosephBackward, JosephForwardTime.back()+30); //starts from 30 min after arriving the destination

    std::cout << "Joseph's Path, duration: " << JosephBackwardTime.back() << std::endl;
    for (int i = 0; i < JosephForward.size(); i++){
      std::cout << "Node: " << JosephForward[i] << "  Time: " << JosephForwardTime[i] << std::endl;
    };
    std::cout << "-- return --" << std::endl;
    for (int i = 0; i < JosephBackward.size(); i++){
      std::cout << "Node: " << JosephBackward[i] << "  Time: " << JosephBackwardTime[i] << std::endl;
    };
    std::cout << "\nLucy's Path, duration: " << LucyBackwardTime.back() << std::endl;
    for (int i = 0; i < LucyForward.size(); i++){
      std::cout << "Node: " << LucyForward[i] << "  Time: " << LucyForwardTime[i] << std::endl;
    };
    std::cout << "-- return --" << std::endl;
    for (int i = 0; i < LucyBackward.size(); i++){
      std::cout << "Node: " << LucyBackward[i] << "  Time: " << LucyBackwardTime[i] << std::endl;
    };
  };


  void Homework(){
    std::vector<int> LucyTimeForward, LucyTimeBackward, LucyPathForward, LucyPathBackward;
    std::vector<int> JosephTimeForward, JosephTimeBackward, JosephPathForward, JosephPathBackward;

    LucyPathForward = this->calcPath(this->LH, this->LD);
    LucyPathBackward = this->calcPath(this->LD, this->LH);
    LucyTimeForward = this->getTimeAcc(LucyPathForward, 0); //starts from 0
    LucyTimeBackward = this->getTimeAcc(LucyPathBackward, LucyTimeForward.back()+30); //starts from TimeForward.back()+30

    JosephPathForward = this->calcPath(this->JH, this->JD);
    JosephPathBackward = this->calcPath(this->JD, this->JH);
    JosephTimeForward = this->getTimeAcc(JosephPathForward, 0); //starts from 0
    JosephTimeBackward = this->getTimeAcc(JosephPathBackward, JosephTimeForward.back()+30); //starts from TimeForward.back()+30

    int intersect00 = this->intersectIn(LucyTimeForward, LucyPathForward, JosephTimeForward, JosephPathForward);  //Both Forward
    int intersect01 = this->intersectIn(LucyTimeForward, LucyPathForward, JosephTimeBackward, JosephPathBackward);  //Lucy Forward & Joseph Backward
    int intersect10 = this->intersectIn(LucyTimeBackward, LucyPathBackward, JosephTimeForward, JosephPathForward);  //Lucy Backward & Joseph Forward
    int intersect11 = this->intersectIn(LucyTimeBackward, LucyPathBackward, JosephTimeBackward, JosephPathBackward);  //Both Backward

    std::vector<int> LucyAlternativePath, LucyAlternativeTime, JosephAlternativePath, JosephAlternativeTime;
    if (intersect00 != -1){ //intersection happened when both Forward
      //======================================================================================================
      LucyAlternativePath = this->calcPath(this->LH, this->LD, intersect00);  //intersect00 flag is banned
      JosephAlternativePath = this->calcPath(this->JH, this->JD, intersect00);  //intersect00 flag is banned

      if(LucyAlternativePath.back() != this->LH && JosephAlternativePath.back() != this->JH){ //There is an alternative path for both Joseph and Lucy --> Pick shortest one
        LucyAlternativeTime = this->getTimeAcc(LucyAlternativePath, 0); //starts from 0
        JosephAlternativeTime = this->getTimeAcc(JosephAlternativePath, 0); //starts from 0
        if (LucyAlternativeTime.back() - LucyTimeForward.back() < JosephAlternativeTime.back() - JosephTimeForward.back())
          PrintResult(LucyAlternativePath, LucyPathBackward, JosephPathForward, JosephPathBackward);//Lucy's Alternative path is better
        else PrintResult(LucyPathForward, LucyPathBackward, JosephAlternativePath, JosephPathBackward); //Joseph's path alter. path
      }
      else if (LucyAlternativePath.back() != this->LH) PrintResult(LucyAlternativePath, LucyPathBackward, JosephPathForward, JosephPathBackward); //There is an alternative path for only Lucy
      else if (JosephAlternativePath.back() != this->JH) PrintResult(LucyPathForward, LucyPathBackward, JosephAlternativePath, JosephPathBackward); //There is an alternative path for only Joseph
      else std::cout << "No solution!" << std::endl;  //There is no alternative solution
    }
    else if (intersect01 != -1){ //intersection happened when Lucy Forward & Joseph Backward
      //======================================================================================================
      LucyAlternativePath = this->calcPath(this->LH, this->LD, intersect01);  //intersect01 flag is banned
      JosephAlternativePath = this->calcPath(this->JD, this->JH, intersect01);  //intersect01 flag is banned

      if(LucyAlternativePath.back() != this->LH && JosephAlternativePath.back() != this->JD){ //There is an alternative path for both Joseph and Lucy --> Pick shortest one
        LucyAlternativeTime = this->getTimeAcc(LucyAlternativePath, 0); //starts from 0
        JosephAlternativeTime = this->getTimeAcc(JosephAlternativePath, JosephTimeForward.back()+30); //starts from TimeForward.back()+30
        if (LucyAlternativeTime.back() - LucyTimeForward.back() < JosephAlternativeTime.back() - JosephTimeBackward.back())
          PrintResult(LucyAlternativePath, LucyPathBackward, JosephPathForward, JosephPathBackward); //Lucy's Alternative path is better
        else PrintResult(LucyPathForward, LucyPathBackward, JosephPathForward, JosephAlternativePath); //Joseph's path alter. path better
      }
      else if (LucyAlternativePath.back() != this->LH) PrintResult(LucyAlternativePath, LucyPathBackward, JosephPathForward, JosephPathBackward);  //There is an alternative path for only Lucy
      else if (JosephAlternativePath.back() != this->JD) PrintResult(LucyPathForward, LucyPathBackward, JosephPathForward, JosephAlternativePath);  //There is an alternative path for only Joseph
      else std::cout << "No solution!" << std::endl; //There is no alternative solution
    }
    else if (intersect10 != -1){ //intersection happened when Backward & Joseph Forward
      //======================================================================================================
      LucyAlternativePath = this->calcPath(this->LD, this->LH, intersect10);  //intersect10 flag is banned
      JosephAlternativePath = this->calcPath(this->JH, this->JD, intersect10);  //intersect10 flag is banned

      if(LucyAlternativePath.back() != this->LD && JosephAlternativePath.back() != this->JH){ //There is an alternative path for both Joseph and Lucy --> Pick shortest one
        LucyAlternativeTime = this->getTimeAcc(LucyAlternativePath, LucyTimeForward.back()+30); //starts from TimeForward.back()+30
        JosephAlternativeTime = this->getTimeAcc(JosephAlternativePath, 0); //starts from 0
        if (LucyAlternativeTime.back() - LucyTimeBackward.back() < JosephAlternativeTime.back() - JosephTimeForward.back())
          PrintResult(LucyPathForward, LucyAlternativePath, JosephPathForward, JosephPathBackward); //Lucy's Alternative path is better
        else PrintResult(LucyPathForward, LucyPathBackward, JosephAlternativePath, JosephPathBackward); //Joseph's path alter. path better
      }
      else if (LucyAlternativePath.back() != this->LD) PrintResult(LucyPathForward, LucyAlternativePath, JosephPathForward, JosephPathBackward);  //There is an alternative path for only Lucy
      else if (JosephAlternativePath.back() != this->JH) PrintResult(LucyPathForward, LucyPathBackward, JosephAlternativePath, JosephPathBackward);  //There is an alternative path for only Joseph
      else std::cout << "No solution!" << std::endl;  //There is no alternative solution
    }
    else if (intersect11 != -1){ //intersection happened when both Backward
      //======================================================================================================
      LucyAlternativePath = this->calcPath(this->LD, this->LH, intersect11);  //intersect11 flag is banned
      JosephAlternativePath = this->calcPath(this->JD, this->JH, intersect11);  //intersect11 flag is banned

      if(LucyAlternativePath.back() != this->LD && JosephAlternativePath.back() != this->JD){ //There is an alternative path for both Joseph and Lucy --> Pick shortest one
        LucyAlternativeTime = this->getTimeAcc(LucyAlternativePath, LucyTimeForward.back()+30); //starts from TimeForward.back()+30
        JosephAlternativeTime = this->getTimeAcc(JosephAlternativePath, JosephTimeForward.back()+30); //starts from TimeForward.back()+30
        if (LucyAlternativeTime.back() - LucyTimeBackward.back() < JosephAlternativeTime.back() - JosephTimeBackward.back())
          PrintResult(LucyPathForward, LucyAlternativePath, JosephPathForward, JosephPathBackward); //Lucy's Alternative path is better
        else PrintResult(LucyPathForward, LucyPathBackward, JosephPathForward, JosephAlternativePath); //Joseph's path alt. path better
      }
      else if (LucyAlternativePath.back() != this->LD) PrintResult(LucyPathForward, LucyAlternativePath, JosephPathForward, JosephPathBackward);  //There is an alternative path for only Lucy
      else if (JosephAlternativePath.back() != this->JD) PrintResult(LucyPathForward, LucyPathBackward, JosephPathForward, JosephAlternativePath);  //There is an alternative path for only Joseph
      else std::cout << "No solution!" << std::endl; //There is no alternative solution
    }
    else PrintResult(LucyPathForward, LucyPathBackward, JosephPathForward, JosephPathBackward); // No intersection Print directly
  };
};


using namespace std;

int main(int argc, char* argv[]){

  string testName = argv[1];
  AdjMtx mtx(testName);

  mtx.Homework();

  return 0;
}
