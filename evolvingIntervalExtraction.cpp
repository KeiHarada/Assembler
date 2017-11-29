#include "assembler.hpp"

/* parameter */
// threshold for wavelet coefficient
const static double DELTA = 30.0;
const static double MINI_LEVEL = 0.8;

/* prototype declaring of the functions */
void wavelet(Sensor &,int,int);
void identifyInterval(Sensor &);

/* main */
void evolvingIntervalExtraction(Sensor (&s)[M]){
  int l = (int)(log2(N));
  int mini_level = l*MINI_LEVEL;

  for(int m=0;m<M;m++){
    wavelet(s[m],l,mini_level);
    identifyInterval(s[m]);
  }
}

void wavelet(Sensor &s, int l, int mini_level){
  vector<double> c;
  vector<double> a;
  vector<double> prev;
  vector<int> data = s.getPM25();

  for(int i=l-1;i>=mini_level;i--){
    int k_i = (int)pow(2,i);
    int K = N/k_i;

    for(int j=0;j<k_i;j++){
      if(i == l-1){
        c.push_back((data[2*j]-data[2*j+1])/2);
        a.push_back((data[2*j]+data[2*j+1])/2);
      }else{
        c.push_back((prev[2*j]-prev[2*j+1])/2);
        a.push_back((prev[2*j]+prev[2*j+1])/2);
      }
      if(fabs(c[j]) >= DELTA) s.setETS(K*j,K*(j+1));
    }

    vector<double>().swap(prev);
    copy(a.begin(),a.end(),back_inserter(prev));
    vector<double>().swap(a);
    vector<double>().swap(c);
  }
}

void identifyInterval(Sensor &s){
  vector<int> ets = s.getETS();
  auto itr = ets.begin();
  int p = *itr;
  int i = *itr;

  while(itr != ets.end()){
    if(i != *itr){
      s.setINTERVAL(p,*(itr-1));
      p = *itr;
      i = *itr;
    }
    if(itr == (ets.end()-1)){
      s.setINTERVAL(p,*itr);
    }
    i++;
    itr++;
  }
}
