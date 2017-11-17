#include "assembler.hpp"

/*parameter*/
// bandwidth for mean shift
const static int W = 10;
// convergence threshould
const static double DELTA_Y = 1.0;
// clusteriring threshould
const static double DELTA_C = 3.0;


/* prototype declaring of functions */
vector<int> sortSlope(Sensor);
int calcIndex(double,vector<int>,map<int,double>);

void clusteringSegment(Sensor (&s)[M]){
  vector<int> sortedSlope;
  int slope_index,y_index;
  double a,b,y,y_prev;
  double delta_prev;
  for(int m=0;m<M;m++){
    sortedSlope = sortSlope(s[m]);
    for(slope_index=0;slope_index < int(sortedSlope.size());slope_index++){
      y = s[m].getSLOPE_VALUE(sortedSlope[slope_index]);
      y_index = slope_index;
      do {
        delta_prev = fabs(y-y_prev);
        y_prev = y;
        a = 0;
        b = 0;
        for(int i=y_index-W;i<=y_index+W;i++){
          if(i >= 0 && i < int(sortedSlope.size())){
            a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(s[m].getSLOPE_VALUE(sortedSlope[i]));
            b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
          }
        }
        y = a/b;
        y_index = calcIndex(y,sortedSlope,s[m].getSLOPE());
      } while(fabs(fabs(y-y_prev)-delta_prev) > DELTA_Y);
      s[m].setCLUSTER(y,sortedSlope[slope_index],DELTA_C);
    }
  }
}

int calcIndex(double y, vector<int> sortedSlope, map<int,double> slope){
  int mini = 0;
  double prev = 100000.0;
  for(auto i=sortedSlope.begin();i!=sortedSlope.end();i++){
    if(fabs(y-slope[*i]) < fabs(y-slope[mini])){
      mini = distance(sortedSlope.begin(),i);
    }
    if(fabs(y-slope[*i]) >= prev){
      return mini;
    }
    prev = fabs(y-slope[*i]);
  }
  return mini;
}

vector<int> sortSlope(Sensor s){
  map<int,int> segment = s.getSEGMENT();
  auto segment_itr = segment.begin();
  vector<int> vec;
  while(segment_itr != segment.end()){
    if(vec.empty()){
      vec.push_back(segment_itr->first);
    }else{
      auto vec_itr = vec.begin();
      while(vec_itr != vec.end() && s.getSLOPE_VALUE(segment_itr->first) > s.getSLOPE_VALUE(*vec_itr)){
        vec_itr++;
      }
      vec.insert(vec_itr,segment_itr->first);
    }
    segment_itr++;
  }
  return vec;
}
