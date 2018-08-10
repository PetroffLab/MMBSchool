#include <iostream>
#include <random>

using namespace std;

default_random_engine generator;

double TrueDeviation[10] = {1,1,1,1,1,1,1,1,1,1};

double rewards(int, vector<double>);
int findMax(vector<double>);
int epsilonChoice(vector<double>);
vector<double> initValues();

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
    values[i] = (2*((double)rand()/RAND_MAX)) - 1;
  }
  return values;
}

int main(){
  cout << "Start The Kindergarten Bandit" << endl;

  int tEnd = 10000;
  double alpha = 0.1;
  vector<double> TrueValues = initValues();

  cout << "The Number of Time Steps is " << tEnd << endl;
  cout << endl;

  cout << "Start Optimistic Greedy Model" << endl;

  vector<double> EstValues = {10,10,10,10,10,10,10,10,10,10};

  for(int t=0; t<tEnd; t++){
    int action = findMax(EstValues);
    double reward = rewards(action, TrueValues);
    EstValues[action] = EstValues[action] + (alpha * (reward - EstValues[action]));
  }

  cout << "According to this Model the best arm is " << findMax(EstValues)
  << " vs " << findMax(TrueValues) << endl;

  for(int i=0; i<EstValues.size(); i++){
    cout << "Estimated Values of arm " << i << ": " <<
    EstValues[i] << "\t vs \t" << TrueValues[i] << endl;
  }
  cout << endl;

  cout << "Start Epsilon-Greedy Model" << endl;

  EstValues = {0,0,0,0,0,0,0,0,0,0};

  for(int t=0; t<tEnd; t++){
    int action = epsilonChoice(EstValues);
    double reward = rewards(action, TrueValues);
    EstValues[action] = EstValues[action] + (alpha * (reward - EstValues[action]));
  }

  cout << "According to this Model the best arm is " << findMax(EstValues)
  << " vs " << findMax(TrueValues) << endl;

  for(int i=0; i<EstValues.size(); i++){
    cout << "Estimated Values of arm " << i << ": " <<
    EstValues[i] << "\t vs \t" << TrueValues[i] << endl;
  }
  cout << endl;

  cout << "Bandit Caught" << endl;
  return 0;
}
