#include <iostream>
#include <fstream>
#include <random>
#include "spline.h"

using namespace std;

// Prototypes
tk::spline surface(int size, double dev);
vector<double> update_data(tk::spline s, double dt, double x, double v, double prob, int size, double D);

// Main Function
int main(){
  int size = 10;
  cout << "Start" << endl;
  tk::spline s = surface(size, 1.0);

  double dt = 0.0001;
  double tEnd = 1.0;
  int tTot = (int)(tEnd/dt);

  ofstream file;
  file.open("Particle.dat");
  ofstream pos;
  pos.open("Position.dat");
  file << 0.5 << "\t" << 0 << endl;
  pos << 0.5 << "\t" << s(0.5) << endl;
  double x = 0.5;
  double v = 0.0;
  double prob = 0.5;
  double D = 0;
  vector<double> newDat(2);

  for(int t=0; t<=tTot; t++){
    cout << "At time " << t*dt << endl;
    newDat = update_data(s, dt, x, v, prob, size, D);
    x = newDat[0];
    v = newDat[1];
    file << x << "\t" << v << endl;
    pos << x << "\t" << s(x) << endl;
  }
  file.close();
}

// Defined Functions
///////////////////////////////////////////////////////////////////////////////

//  *Defines the Surface of our Game Board
tk::spline surface(int size, double dev){
  vector<double> Xh(size), Yh(size);

  default_random_engine generator;
  normal_distribution<double> distribution((double)size,dev);
  for(int i=0; i<size; i++){
    Xh[i] = (1.0/size)*i;
    Yh[i] = distribution(generator);
  }

  tk::spline s;
  s.set_points(Xh,Yh);

  ofstream file;
  file.open("Height.dat");

  for(int i=0; i<(10*size); i++){
    double x = 1.0/(10*size)*i;
    file << fixed << x << "\t" << fixed << s(x) << endl;
  }
  file.close();
  return s;
}
///////////////////////////////////////////////////////////////////////////////

//  *Updates the poisiton data for a single particle
vector<double> update_data(tk::spline s, double dt, double x, double v, double prob, int size, double D){
  bool vdir = v >= 0;
  bool sdir = s(x+pow(10,-8))-s(x) <= 0;
  default_random_engine generator;
  normal_distribution<double> distribution(0.5,0.5);
  double dw = sqrt(dt)*distribution(generator);
  double newv = -((s(x+pow(10,-8))-s(x))/pow(10,-8)) + v;
  //cout << newv << endl;
  double dx = (newv*dt) + (sqrt(D)*dw);
  vector<double> soln(2);

  if(vdir && sdir){
    soln[0] = x + dx;
    soln[1] = newv;
  } else{
    double p = (double)rand()/RAND_MAX;
    if(p <= prob){
      soln[0] = x + dx;
      soln[1] = newv;
    } else{
      soln[0] = x;
      soln[1] = 0;
    }
  }

  return soln;
}
///////////////////////////////////////////////////////////////////////////////
