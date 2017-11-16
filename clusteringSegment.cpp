#include "assembler.hpp"

/* prototype declaring of functions */
vector<int> sortSlope(Sensor);
int calcCenter(Senseor s, int prev);

void clusteringSegment(Sensor (&s)[M]){
  vector<int> sequence;
  int y, y_prev;
  for(int m=0;m<M;m++){
    sequence = sortSlope(s[m]);
    auto itr = sequence.begin();
    y = -1;
    y_prev = *itr;
    while(itr != sequence.end()){
      while(y != y_prev){
        y = calcCenter(s[m].getSEGMENT(),y_prev);
      }

    }
  }
}

vector<int> sortSlope(Sensor s){
 vector<int> vec;
 map<int,int> segment = s.getSEGMENT();
 map<int,double> slope = s.getSLOPE();
 auto itr = segment.begin();
 while(itr != segment.end()){
   vec.push_back(itr->first);
   itr++;
 }
 return vec;
}

int calcCenter(map<int> segment, int prev){
  int y;
  for(int i=prev-W;i<=prev+W;i++){
    if()
  cout << endl;
  }
  return y;
}
