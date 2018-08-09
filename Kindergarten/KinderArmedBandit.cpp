#include <iostream>
#include <random>

using namespace std;

default_random_engine generator;

int TrueValues[10] = {3,-1,4,2,-4,0,-3,-2,1,4};
double TrueDeviation[10] = {1,1,1,1,1,1,1,1,1,1};

template <class T>
class ArmName{
public:
  ArmName(T arm);
  ~ArmName(){};
  double getReward();
private:
  T armName;
};

template <class T>
ArmName<T>::ArmName(T arm){
  armName = arm;
}

template <class T>
double ArmName<T>::getReward(){
  normal_distribution<double> distribution(TrueValues[armName],TrueDeviation[armName]);
  return distribution(generator);
}

template <class T>
using ValueFunctions = double (ArmName<T>::*)();

int main(){
  cout << "Start The Kindergarten Bandit" << endl;
  ArmName<int> arm0(0);
  ArmName<int> arm1(1);
  ArmName<int> arm2(2);
  ArmName<int> arm3(3);
  ArmName<int> arm4(4);
  ArmName<int> arm5(5);
  ArmName<int> arm6(6);
  ArmName<int> arm7(7);
  ArmName<int> arm8(8);
  ArmName<int> arm9(9);

  cout<< arm0.getReward() << endl;

  ValueFunctions<int> rewards[] = {
    &arm0::getReward,
    &arm1::getReward,
    &arm2::getReward,
    &arm3::getReward,
    &arm4::getReward,
    &arm5::getReward,
    &arm6::getReward,
    &arm7::getReward,
    &arm8::getReward,
    &arm9::getReward
  };

  cout << "Here is the Reward Test: " << endl;
  for(int i=0; i<10; i++){
    cout << i << ": " << rewards[i]() << ", " << rewards[i]() << endl;
  }

  return 0;
}
