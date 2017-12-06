#include "assembler.hpp"

/*parameter*/
// bandwidth for mean shift
const static int W = 10.0;
// convergence threshold
const static double DELTA_Y = 6.5; // have to tune.
// clustering threshold
const static double DELTA_C = 10; // have to tune.
// minimum support
const static int THETA_C = 60; // have to tune.


/* prototype declaring of functions */
vector<int> sortSlope(map<int,double>,map<int,int>);
int getPositiveIndex(map<int,double>,vector<int>);
int calcIndex(double,vector<int>,map<int,double>,int);
int isZero(double);

void clusteringSegment(Sensor (&s)[M]){
  int i,y_index;
  double a,b,y,y_prev;
  double delta_prev;
  for(int m = 0;m < M;m++){
    map<int,double> slope_value = s[m].getSLOPE();
    vector<int> sortedSlope = sortSlope(slope_value,s[m].getSEGMENT());
    if(m==77){
      cout <<""<<endl;
    }
    int index = 0;
    int positive = getPositiveIndex(slope_value,sortedSlope);

    /* negative part */
    while(index < positive){
      y = slope_value[sortedSlope[index]];
      y_index = index;
      y_prev = 0;
      do{
        delta_prev = fabs(y-y_prev);
        y_prev = y;
        a = 0;
        b = 0;
        i = y_index+1;
        while(slope_value[sortedSlope[i]] <= y+W && i < positive){
          a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(slope_value[sortedSlope[i]]);
          b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
          i++;
        }
        i = y_index;
        while(slope_value[sortedSlope[i]] >= y-W && i >= 0){
          a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(slope_value[sortedSlope[i]]);
          b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
          i--;
        }
        y = a/b;
        if(y != y_prev){
          y_index = calcIndex(y,sortedSlope,slope_value,positive);
        }
      }while(fabs(y-y_prev) >= DELTA_Y && isZero(fabs(y-y_prev)-delta_prev) == 0);
      s[m].setCLUSTER(y,sortedSlope[index]);
      index++;
    }

    /* positive part */
    while(index < int(sortedSlope.size())){
      y = slope_value[sortedSlope[index]];
      y_index = index;
      y_prev = 0;
      do{
        delta_prev = fabs(y-y_prev);
        y_prev = y;
        a = 0;
        b = 0;
        i = y_index+1;
        while(slope_value[sortedSlope[i]] <= y+W && i < int(sortedSlope.size())){
          a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(slope_value[sortedSlope[i]]);
          b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
          i++;
        }
        i = y_index;
        while(slope_value[sortedSlope[i]] >= y-W && i >= positive){
          a += (s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i])*(slope_value[sortedSlope[i]]);
          b += s[m].getSEGMENT_END(sortedSlope[i])-sortedSlope[i];
          i--;
        }
        y = a/b;
        if(y != y_prev){
          y_index = calcIndex(y,sortedSlope,slope_value,positive);
        }
      }while(fabs(y-y_prev) >= DELTA_Y && isZero(fabs(y-y_prev)-delta_prev) == 0);
      s[m].setCLUSTER(y,sortedSlope[index]);
      index++;
    }
    s[m].fixCLUSTER(THETA_C,DELTA_C);
  }
}

int calcIndex(double y, vector<int> sortedSlope, map<int,double> slope, int p){
  int mini;
  if(y < 0.0){
    mini = 0;
    for(int i=0;i<p;i++){
      if(fabs(y-slope[sortedSlope[i]]) < fabs(y-slope[sortedSlope[mini]])){
        mini = i;
      }
    }
    return mini;
  }

  if(y > 0.0){
    mini = p;
    for(int i=p;i<int(sortedSlope.size());i++){
      if(fabs(y-slope[sortedSlope[i]]) < fabs(y-slope[sortedSlope[mini]])){
        mini = i;
      }
    }
    return mini;
  }
}

vector<int> sortSlope(map<int,double>slope, map<int,int> segment){
  auto segment_itr = segment.begin();
  vector<int> vec;
  while(segment_itr != segment.end()){
    if(isZero(slope[segment_itr->first]) == 0){
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

int getPositiveIndex(map<int,double> slope, vector<int> vector){
  auto vec_itr = vector.begin();
  while(slope[*vec_itr] < 0){
    vec_itr++;
  }
  return distance(vector.begin(),vec_itr);
}

int isZero(double x) {
  // if return = 1 then x = 0, else x != 0
  return (x>=0 && x<DBL_MIN*100);
}

