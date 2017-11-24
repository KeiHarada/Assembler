#include "assembler.hpp"

/*parameter*/
// bandwidth for mean shift
const static int W = 10.0;
// convergence threshould
const static double DELTA_Y = 6.0;
// clusteriring threshould
const static double DELTA_C = 3.0;
// minimum shupport
const static int THETA_C = 100;


/* prototype declaring of functions */
vector<int> sortSlope(Sensor);
int calcIndex(double,vector<int>,map<int,double>);

void clusteringSegment(Sensor (&s)[M]){
  vector<int> sortedSlope;
  int slope_index,y_index;
  double a,b,y,y_prev;
  for(int m=0;m<M;m++){
    sortedSlope = sortSlope(s[m]);
    for(slope_index=0;slope_index < int(sortedSlope.size());slope_index++){
      y = s[m].getSLOPE_VALUE(sortedSlope[slope_index]);
      y_index = slope_index;
      y_prev = 0;
      do {
        y_prev = y;
        a = 0;
        b = 0;
        for(int i=y_index+1;s[m].getSLOPE_VALUE(sortedSlope[i])<=y+W && i < int(sortedSlope.size());i++){
          if(m==0)cout << "+ "<<slope_index <<"\t"<< s[m].getSLOPE_VALUE(sortedSlope[slope_index])<< "\t"<< i <<"\t"<< y <<"\t" << s[m].getSLOPE_VALUE(sortedSlope[i]) <<endl;
          a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(s[m].getSLOPE_VALUE(sortedSlope[i]));
          b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
        }
        for(int i=y_index;s[m].getSLOPE_VALUE(sortedSlope[i])<=y-W && i >= 0;i--){
          if(m==0)cout << "-"<< y <<"\t" << s[m].getSLOPE_VALUE(sortedSlope[i]) <<endl;
          a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(s[m].getSLOPE_VALUE(sortedSlope[i]));
          b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
        }
        y = a/b;
        y_index = calcIndex(y,sortedSlope,s[m].getSLOPE());
        y = s[m].getSLOPE_VALUE(sortedSlope[y_index]);
      } while(fabs(y-y_prev) >= DELTA_Y);
      if(m==0)cout << "---" << endl;
      //if(m == 0) cout<<slope_index <<"\t"<< s[m].getSLOPE_VALUE(sortedSlope[slope_index])<<"\t" << y << "\t" << y_index << endl;
      s[m].setCLUSTER(y,sortedSlope[slope_index],DELTA_C);
    }
    //s[m].fixCLUSTER(THETA_C);
  }
}

int calcIndex(double y, vector<int> sortedSlope, map<int,double> slope){
  int mini = 0;
  for(auto i=sortedSlope.begin();i!=sortedSlope.end();i++){
    if(fabs(y-slope[*i]) < fabs(y-slope[mini])){
      mini = distance(sortedSlope.begin(),i);
    }
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
      while(vec_itr != vec.end() && s.getSLOPE_VALUE(segment_itr->first) >= s.getSLOPE_VALUE(*vec_itr)){
        vec_itr++;
      }
      vec.insert(vec_itr,segment_itr->first);
    }
    segment_itr++;
  }
  return vec;
}
