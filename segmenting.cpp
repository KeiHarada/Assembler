#include "assembler.hpp"

/* parameter */
// threshold for segmenting time series
const static double MAX_ERROR = 30.0; // we have to tune.

/* prototype declaring of the functions */
void segmenting(Sensor &,int,int);
int isZero(double);

/* main */
void segmentingEvolvingInterval(Sensor (&s)[M]){
  for(int m=0;m<M;m++){
    map<int,int> interval = s[m].getINTERVAL();
    auto itr = interval.begin();
    while(itr != interval.end()){
      segmenting(s[m],itr->first,itr->second);
      itr = interval.upper_bound(itr->second);
    }
  }
}

void segmenting(Sensor &s, int begin, int end){
  /* fix the segment on this interval if it has only 2 timestamps */
  if(end-begin == 1){
    s.setSEGMENT(begin,end);
    s.setSLOPE(begin,(s.getPM25()[end]-s.getPM25()[begin]));
  }else{
    double prev,current;
    vector<double> cost;
    map<int,int> seg;

    /* calculate initial costs */
    for(int i=begin;i<end;i++){
        current = double(s.getPM25()[i+1]-s.getPM25()[i]);
        seg[i] = i+1;
        if(i > begin) cost.push_back(abs(prev-current));
        prev = current;
    }

    /* merge neighbors */
    auto cost_itr = min_element(cost.begin(),cost.end());
    int  cost_mini = distance(cost.begin(),cost_itr);
    auto seg_itr = seg.find(begin+cost_mini);
    int seg_mini = seg_itr->first;
    while(cost[cost_mini] < MAX_ERROR && int(cost.size()) > 0){
      /* update seg[begin+mini] */
      int next = seg[seg[seg_mini]];
      seg.erase(seg[seg_mini]);
      seg[seg_mini] = next;

      /* update cost[mini-1] */
      /* if cost_mini doesn't equals to first index of cost */
      if(cost_mini != 0){
        seg_itr--;
        prev = double(s.getPM25()[seg_mini]-s.getPM25()[seg_itr->first])/(seg_mini-seg_itr->first);
        current = double(s.getPM25()[seg[seg_mini]]-s.getPM25()[seg_mini])/(seg[seg_mini]-seg_mini);
        cost[cost_mini-1] = abs(prev-current);
      }

      /* erase cost[mini] */
      cost.erase(cost_itr);

      /* update cost[mini] */
      /* if cost_mini doesn't equals to last index of cost */
      if(cost_mini != int(cost.size())){
        prev = current;
        current = double(s.getPM25()[seg[seg[seg_mini]]]-s.getPM25()[seg[seg_mini]])/(seg[seg[seg_mini]]-seg[seg_mini]);
        cost[cost_mini] = abs(prev-current);
      }

      /* re-calculate minimum cost */
      cost_itr = min_element(cost.begin(),cost.end());
      cost_mini = distance(cost.begin(),cost_itr);
      seg_itr = seg.lower_bound(begin+cost_mini);
      seg_mini = seg_itr->first;
    }

    /* set segments on this interval */
    for(auto itr = seg.begin();itr != seg.end();itr++){
      double slope = double(s.getPM25()[itr->second]-s.getPM25()[itr->first])/(itr->second-itr->first);
      // if the slope value doesn't equal to zero
      if(isZero(slope) == 0) {
        s.setSEGMENT(itr->first, itr->second);
        s.setSLOPE(itr->first, slope);
      }
    }
  }
}

int isZero(double x) {
  return (x>=0 && x<DBL_MIN*100);
}
