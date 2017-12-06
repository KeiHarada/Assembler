#include "assembler.hpp"

/* parameter */
// threshold for segmenting time series
const static double MAX_ERROR = 10.0; // have to tune.

/* prototype declaring of the functions */
void segmenting(Sensor &,int,int);

/* main */
void segmentingEvolvingInterval(Sensor (&s)[M]){
  for(int m=0;m<M;m++){
    map<int,int> interval = s[m].getINTERVAL();
    auto interval_itr = interval.begin();
    while(interval_itr != interval.end()){
      segmenting(s[m],interval_itr->first,interval_itr->second);
			map<int,int> test = s[m].getSEGMENT();
      interval_itr++;
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
        if(i > begin) cost.push_back(fabs(prev-current));
        prev = current;
    }

    /* merge neighbors */
    auto cost_itr = min_element(cost.begin(),cost.end());
    int  cost_mini = distance(cost.begin(),cost_itr);
    auto seg_itr = seg.find(begin+cost_mini);
    int seg_mini = seg_itr->first;
    while(cost[cost_mini] < MAX_ERROR && !cost.empty()){
      /* update seg[begin+mini] */
      int next = seg[seg[seg_mini]];
      seg.erase(seg[seg_mini]);
      seg[seg_mini] = next;

      /* update cost[mini-1] */
      /* if cost_mini doesn't equals to first index of cost */
      if(cost_mini >= 1){
        seg_itr--;
        prev = double(s.getPM25()[seg_mini]-s.getPM25()[seg_itr->first])/(seg_mini-seg_itr->first);
        current = double(s.getPM25()[seg[seg_mini]]-s.getPM25()[seg_mini])/(seg[seg_mini]-seg_mini);
        cost[cost_mini-1] = fabs(prev-current);
      }

      /* erase cost[mini] */
      cost.erase(cost_itr);

      if(!cost.empty()){
        /* update cost[mini] */
        prev = double(s.getPM25()[seg[seg_mini]]-s.getPM25()[seg_mini])/(seg[seg_mini]-seg_mini);
        current = double(s.getPM25()[seg[seg[seg_mini]]]-s.getPM25()[seg[seg_mini]])/(seg[seg[seg_mini]]-seg[seg_mini]);
        cost[cost_mini] = fabs(prev-current);

        /* re-calculate minimum cost */
        cost_itr = min_element(cost.begin(),cost.end());
        cost_mini = distance(cost.begin(),cost_itr);
        seg_itr = seg.lower_bound(begin+cost_mini);
        seg_mini = seg_itr->first;
      }
    }

    /* set segments on this interval */
    for(auto itr = seg.begin();itr != seg.end();itr++){
      double slope = double(s.getPM25()[itr->second]-s.getPM25()[itr->first])/(itr->second-itr->first);
      if(itr->second != 0){
        s.setSEGMENT(itr->first, itr->second);
        s.setSLOPE(itr->first, slope);
      }
		}
	}
}