#include "assembler.hpp"

/*parameter*/
// bandwidth for mean shift
const static int W = 10.0;
// convergence threshold
const static double DELTA_Y = 6.5; // we have to tune.
// clustering threshold
const static double DELTA_C = 3.0; // we have to tune.
// minimum support
const static int THETA_C = 10; // we have to tune.


/* prototype declaring of functions */
vector<int> sortSlope(map<int,double>,map<int,int>);
int calcIndex(double,vector<int>,map<int,double>);

void clusteringSegment(Sensor (&s)[M]){
  int i,y_index;
  double a,b,y,y_prev;
  for(int m=0;m<M;m++){
	  map<int,double> slope_value = s[m].getSLOPE();
	  vector<int> sortedSlope = sortSlope(slope_value,s[m].getSEGMENT());
	  auto slope_itr = sortedSlope.begin();
	  while(slope_itr != sortedSlope.end()){
		  y = slope_value[*slope_itr];
		  y_index = distance(sortedSlope.begin(),slope_itr);
		  do {
              vector<int> replace;
              y_prev = y;
              a = 0; b = 0;
			  i = y_index+1;
        while(slope_value[sortedSlope[i]]<=y+W && i<int(sortedSlope.size())){
            a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(slope_value[sortedSlope[i]]);
            b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
            replace.push_back(sortedSlope[i]);
            i++;
        }
        i=y_index;
        while(slope_value[sortedSlope[i]]>=y-W && i >= 0){
            a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(slope_value[sortedSlope[i]]);
            b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
            replace.push_back(sortedSlope[i]);
            i--;
        }
        y = a/b;
        if(y != y_prev){
            y_index = calcIndex(y,sortedSlope,s[m].getSLOPE());
        }
      } while(fabs(y-y_prev) >= DELTA_Y);
      s[m].setCLUSTER(y,*slope_itr);
      slope_itr++;
    }
    s[m].fixCLUSTER(THETA_C,DELTA_C);
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

vector<int> sortSlope(map<int,double>slope,map<int,int> segment){
  auto segment_itr = segment.begin();
  vector<int> vec;
  while(segment_itr != segment.end()){
    if(vec.empty()){
      vec.push_back(segment_itr->first);
    }else{
      auto vec_itr = vec.begin();
      while(vec_itr != vec.end() && slope[segment_itr->first] >= slope[*vec_itr]){
        vec_itr++;
      }
      vec.insert(vec_itr,segment_itr->first);
    }
    segment_itr++;
  }
  return vec;
}
