// Tanay Cansin Dogan - 150150119

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<bits/stdc++.h> //sort


int max(const int a, const int b){
  return (a<b) ? b : a;
};


int min(const int a, const int b){
  return (a<b) ? a : b;
};


std::vector<int> Str2Vec(std::string str){

  std::vector<int> FreqVec;
  std::size_t prev = 0;
  for (std::size_t pos; (pos = str.find_first_of("-\0", prev)) != std::string::npos; prev = pos+1){ //parse until last piece
      if (pos > prev) FreqVec.push_back(std::stoi(str.substr(prev, pos-prev)));
  };
  if (prev < str.length()) FreqVec.push_back(std::stoi(str.substr(prev, std::string::npos))); //push last piece

  return FreqVec;
};


std::vector<int> OrderSeq(std::vector<int> Seq){
  std::vector<int> Ordered;
  for (int search = 0; Ordered.size() < Seq.size(); search++) {
    for (int j = 0; j < Seq.size(); j++) {
      if (Seq[j] == search) Ordered.push_back(j+1);
    }
  };
  return Ordered;
};


int EditDistance(std::vector<int> Seq1, std::vector<int> Seq2){
  //problem specified levenshtein's Algorithm based Distance calculation function

  std::vector< std::vector<int> > LevenshteinMatrix;
  for (int i = 0; i <= Seq1.size(); i++){
    std::vector<int> LevenshteinRow;
    for (int j = 0; j <= Seq2.size(); j++){
      if (min(i, j) == 0) LevenshteinRow.push_back(max(i, j));
      else{
        int indicator = (Seq1[i-1] == Seq2[j-1]) ? 0: 1;  //indexing increase by 2 due to the '-' characters
        LevenshteinRow.push_back(min(min(LevenshteinMatrix[i-1][j]+1, LevenshteinRow[j-1]+1), LevenshteinMatrix[i-1][j-1]+indicator));
      };
    };
    LevenshteinMatrix.push_back(LevenshteinRow);
  };

  return LevenshteinMatrix[Seq1.size()-1][Seq2.size()-1];
};


class TestSuite{
public:
  std::string Name;
  int bugsDetected;
  int runningTime;
  std::vector<std::string> testCaseFreq;

  TestSuite(std::string line){
    size_t pos = 0;

    pos = line.find(" ");
    this->Name = line.substr(0, pos);
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    pos = line.find(" ");
    this->bugsDetected = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    pos = line.find(" ");
    this->runningTime = std::stoi(line.substr(0, pos));
    line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

    while((pos = line.find_first_of(" \n", 0)) != std::string::npos){   //parse test case freq
      std::string Profile;

      Profile = line.substr(0, pos);
      line.erase(0, pos+1); //+1 for the delimiter lenght delimiter = " "

      this->testCaseFreq.push_back(Profile);
    };
    this->testCaseFreq.push_back(line); //add last piece
  };


  void Printer(){
    std::cout << "NAME:\t" << this->Name << "\tBUGS:\t" << this->bugsDetected << "\tTIME:\t" << this->runningTime << std::endl;
    for (int i = 0; i < this->testCaseFreq.size(); i++){
      std::cout << this->testCaseFreq[i] << std::endl;
    };
    std::cout << "\n" << std::endl;
  };
};


class Homework{
public:
  int TimeLimit;
  std::vector<TestSuite> TestDataBase;

  Homework(std::string fileName){
    std::ifstream file;
    std::string line;
    file.open(fileName);

    getline(file, line);  //Max MaxAllowedTime information
    size_t pos = line.find(":");
    line.erase(0, pos+2); //+2 diliminater and space
    this->TimeLimit = std::stoi(line);

    getline(file, line);  //skip empty line
    getline(file, line);  //skip header line

    while(getline(file, line)){
      TestSuite test(line);
      this->TestDataBase.push_back(test);
    };
  };


  void ShowDataBase(){
    for(int i = 0; i < this->TestDataBase.size(); i++){
      this->TestDataBase[i].Printer();
    }
  };


  std::vector<int> Part1(){

    std::vector<std::vector<int>> KnapsackMatrix;
    std::vector<int> KnapsackRow;
    for (int j = 0; j <= this->TimeLimit; j++) KnapsackRow.push_back(0);
    KnapsackMatrix.push_back(KnapsackRow);  //Assignment of the first row (i = 0) as all zeros
    KnapsackRow.clear();
    //Knapsack algorithm
    for (int i = 1; i <= this->TestDataBase.size(); i++){  //subsets of test
      for (int j = 0; j <= this->TimeLimit; j++){  //according to lesser or equal time limits
        if (this->TestDataBase[i-1].runningTime > j) KnapsackRow.push_back(KnapsackMatrix[i-1][j]);
        else KnapsackRow.push_back(max(KnapsackMatrix[i-1][j], this->TestDataBase[i-1].bugsDetected + KnapsackMatrix[i-1][j - this->TestDataBase[i-1].runningTime]));
      };
      KnapsackMatrix.push_back(KnapsackRow);
      KnapsackRow.clear();
    };

    int i = KnapsackMatrix.size()-1, j = KnapsackMatrix[0].size()-1;  //bottom rightmost of the matrix
    std::vector<int> selectedTests;
    while (i != 0){ //selection of the picked Test Suites
      if (KnapsackMatrix[i][j] == KnapsackMatrix[i-1][j]) i--;
      else {
        selectedTests.push_back(i-1);
        j -= this->TestDataBase[i-1].runningTime;
      };
    };

    return selectedTests;
  };


  void Part2(int index){

    int maxCount = 0; int maxIndex = 0;
    for (int i = 0; i < this->TestDataBase[index].testCaseFreq.size(); i++){
      int count = 0;
      //std::cout << this->TestDataBase[index-1].testCaseFreq[i] << '\n';
      for (int w = 0; w < this->TestDataBase[index].testCaseFreq[i].length(); w++){
        if (this->TestDataBase[index].testCaseFreq[i][w] != '0') count++;
      };
      if (count > maxCount){  //update max index
        maxIndex = i;
        maxCount = count;
      };
    };

    std::vector< std::pair <int, int> > Distances;
    for (int i = 0; i < this->TestDataBase[index].testCaseFreq.size(); i++){  //calculate the Edit distances of test cases compare to the max coverage
      if (i == maxIndex) continue;
      std::vector<int> Seq1 = OrderSeq(Str2Vec(this->TestDataBase[index].testCaseFreq[maxIndex]));
      std::vector<int> Seq2 = OrderSeq(Str2Vec(this->TestDataBase[index].testCaseFreq[i]));
      Distances.push_back(std::make_pair(EditDistance(Seq1, Seq2), i));
    };
    std::sort(Distances.begin(), Distances.end());  //Sort the <distance, index> vector in ascending order compare to the distances

    std::vector<int> finalOrder;    //the vector which stores the final order of the test cases
    finalOrder.push_back(maxIndex); //Push the sequence cover most of the statements
    for (int i = Distances.size()-1; i >= 0; i--){  //push the sorted <distance, index> array's index values to the finalOrder in reverse order
       finalOrder.push_back(Distances[i].second);
    };

    for (size_t i = 0; i < finalOrder.size(); i++) {  //print the final order vector
      std::cout << finalOrder[i] + 1 << " ";
    };
    std::cout << '\n';
  };
};


using namespace std;

int main(int argc, char* argv[]){

  string fileName = argv[1];
  Homework HW(fileName);

  std::cout << "===== Part1: =====" << '\n';
  std::vector<int> v = HW.Part1();
  for (int i = 0; i < v.size(); i++) {
    std::cout << HW.TestDataBase[v[i]].Name << "\tdiscovered bugs:\t" << HW.TestDataBase[v[i]].bugsDetected << '\n';
  }

  std::cout << "===== Part2: =====" << '\n';

  for (size_t i = 0; i < v.size(); i++) {
    std::cout << HW.TestDataBase[v[i]].Name << " ";
    HW.Part2(v[i]);
  };

  return 0;
}
