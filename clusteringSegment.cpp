#include "assembler.hpp"

/* prototype declaring of functions */
vector<int> sortSlope(Sensor);

void clusteringSegment(Sensor (&s)[M]){
  vector<int> slope;
  vector<int>::iterator slope_itr,y,y_prev,i;
  double a,b;
  for(int m=0;m<M;m++){
    slope = sortSlope(s[m]);
    slope_itr = slope.begin();
    while(slope_itr != slope.end()){
      y = slope_itr;
      do {
        y_prev = y;
        for(i=y-W;i<=y+W;i++){
          if(i >= slope.begin() && i < slope.end()){
            a += (s[m].getSEGMENT_END(*slope_itr)-*slope_itr)*(s[m].getSLOPE_VALUE(*slope_itr));
            b += (s[m].getSEGMENT_END(*slope_itr)-*slope_itr);
          }
        }
        y = slope.begin() + (int)(a/b);
      } while(y != y_prev);
      s[m].setCLUSTER(*slope_itr,*y);
      slope_itr++;
    }
  }
}

vector<int> sortSlope(Sensor s){
  vector<int> vec;
  map<int,int> segment = s.getSEGMENT();
  auto segment_itr = segment.begin();
  while(segment_itr != segment.end()){
    auto p = vec.end();
    p--;
    while(p != vec.begin() && s.getSLOPE_VALUE(segment_itr->first) < s.getSLOPE_VALUE(*p)){
      p--;
    }
    vec.insert(p,segment_itr->first);
    segment_itr++;
  }
  return vec;
}
