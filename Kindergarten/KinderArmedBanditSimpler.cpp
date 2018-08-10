#include <iostream>
#include <random>

using namespace std;

default_random_engine generator;

int TrueValues[10] = {3,-1,4,2,-4,0,-3,-2,1,5};
double TrueDeviation[10] = {1,1,1,1,1,1,1,1,1,1};

double rewards(int);
int findMax(vector<double>);

double rewards(int arm){
  normal_distribution<double> distribution(TrueValues[arm],TrueDeviation[arm]);
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

int main(){
  cout << "Start The Kindergarten Bandit" << endl;

  int tEnd = 100;
  double alpha = 0.5;

  vector<double> EstValues = {10,10,10,10,10,10,10,10,10,10};

  for(int t=0; t<tEnd; t++){
    int action = findMax(EstValues);
    double reward = rewards(action);
    EstValues[action] = EstValues[action] + (alpha * (reward - EstValues[action]));
  }

  for(int i=0; i<EstValues.size(); i++){
    cout << "Estimated Values of arm " << i << ": " <<
    EstValues[i] << " vs " << TrueValues[i] << endl;
  }

  cout << "Bandit Caught" << endl;

  return 0;
}
