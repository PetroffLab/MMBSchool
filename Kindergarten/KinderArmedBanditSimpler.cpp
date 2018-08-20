#include <iostream>
#include <random>
#include <fstream>

using namespace std;

default_random_engine generator;

double TrueDeviation[10] = {1,1,1,1,1,1,1,1,1,1};

double rewards(int, vector<double>);
int findMax(vector<double>);
int epsilonChoice(vector<double>);
vector<double> initValues();
int isTrue(int, vector<double>);

double rewards(int arm, vector<double> values){
  normal_distribution<double> distribution(values[arm],TrueDeviation[arm]);
  return distribution(generator);
}

int findMax(vector<double> values){
  int max = 0;
  for(int i=1; i<values.size(); i++){
    if(values[i]>values[max]){
      max = i;
    }
  }
  return max;
}

int epsilonChoice(vector<double> values){
  double prob = (double) rand()/(RAND_MAX);
  double epsilon = 0.1;

  if(prob<(1-epsilon)){
    return findMax(values);
  } else{
    int explore = 0;
    int step = rand() % (values.size() - 1);
    for(int i=0; i<step; ){
      if(explore != findMax(values)){
        i++;
      }
      explore++;
    }
    return explore;
  }
}

vector<double> initValues(){
  vector<double> values = {0,0,0,0,0,0,0,0,0,0};
  for(int i=0; i<10; i++){
    normal_distribution<double> value(0,1);
    values[i] = value(generator);
  }
  return values;
}

int isTrue(int EstValue, vector<double> TrueValues){
  if(EstValue == findMax(TrueValues)){
    return 1;
  } else{
    return 0;
  }
}



int main(){
  cout << "Start The Kindergarten Bandit" << endl;

  int tEnd = 1000;
  int totalRuns = 1000;
  double alpha = 0.5;
  vector<double> TrueValues = initValues();

  cout << "The Number of Time Steps is " << tEnd << endl;
  cout << "The Number of Total Runs is " << totalRuns << endl;
  cout << endl;

  cout << "Start Optimistic Greedy Model" << endl;

  ofstream file;
  file.open("Greedy.dat");

  vector<vector<double>> GreedyHistory;

  for(int run = 0; run < totalRuns; run++){
    vector<double> EstValues = {10,10,10,10,10,10,10,10,10,10};

    vector<double> runHistory;

    for(int t=0; t<tEnd; t++){
      int action = findMax(EstValues);
      runHistory.push_back(isTrue(action, TrueValues));
      double reward = rewards(action, TrueValues);
      EstValues[action] = EstValues[action] + (alpha * (reward - EstValues[action]));
    }

    GreedyHistory.push_back(runHistory);
  }

  for(int choice = 0; choice < GreedyHistory[0].size(); choice++){
    int successes = 0;
    for(int run = 0; run < totalRuns; run++){
      successes += GreedyHistory[run][choice];
    }
    double percentSuccess = (double)successes/totalRuns;

    file << percentSuccess << endl;
  }
  //
  // cout << "According to this Model the best arm is " << findMax(EstValues)
  // << " vs " << findMax(TrueValues) << endl;
  //
  // for(int i=0; i<EstValues.size(); i++){
  //   cout << "Estimated Values of arm " << i << ": " <<
  //   EstValues[i] << "\t vs \t" << TrueValues[i] << endl;
  // }
  // cout << endl;

  file.close();

  cout << "Start Epsilon-Greedy Model" << endl;

  file.open("EpsilonGreedy.dat");

  vector<vector<double>> EpsilonGreedyHistory;

  for(int run = 0; run < 1000; run++){
    vector<double> EstValues = {0,0,0,0,0,0,0,0,0,0};

    vector<double> runHistory;

    for(int t=0; t<tEnd; t++){
      int action = epsilonChoice(EstValues);
      runHistory.push_back(isTrue(action, TrueValues));
      double reward = rewards(action, TrueValues);
      EstValues[action] = EstValues[action] + (alpha * (reward - EstValues[action]));
    }

    EpsilonGreedyHistory.push_back(runHistory);
  }

  for(int choice = 0; choice < EpsilonGreedyHistory[0].size(); choice++){
    int successes = 0;
    for(int run = 0; run < totalRuns; run++){
      successes += EpsilonGreedyHistory[run][choice];
    }
    double percentSuccess = (double)successes/totalRuns;

    file << percentSuccess << endl;
  }

  // cout << "According to this Model the best arm is " << findMax(EstValues)
  // << " vs " << findMax(TrueValues) << endl;
  //
  // for(int i=0; i<EstValues.size(); i++){
  //   cout << "Estimated Values of arm " << i << ": " <<
  //   EstValues[i] << "\t vs \t" << TrueValues[i] << endl;
  // }
  // cout << endl;

  file.close();

  cout << "Bandit Caught" << endl;
  return 0;
}
