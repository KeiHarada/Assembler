#include "assembler.hpp"

/* prototype declaring of the functions */
int makeTimestamp(string);

/* accessor*/
int Sensor::getID(){ return id; }
double Sensor::getLAT(){ return lat; }
double Sensor::getLON(){ return lon; }
vector<int> Sensor::getTIME(){ return time; }
vector<int> Sensor::getPM25(){ return pm25; }
vector<int> Sensor::getETS(){ return ets; }
int Sensor::getINTERVAL_END(int begin){ return interval[begin]; }
map<int,int> Sensor::getINTERVAL(){ return interval; }
int Sensor::getSEGMENT_END(int begin){ return segment[begin]; }
map<int,int> Sensor::getSEGMENT(){ return segment; }
double Sensor::getSLOPE_VALUE(int key){ return slope[key]; }
map<int,double> Sensor::getSLOPE(){ return slope;}
multimap<double,int> Sensor::getCLUSTER(){ return cluster; }
double Sensor::getUPPER(double key){ return upper[key]; }
double Sensor::getLOWER(double key){ return lower[key]; }

/* setter*/
void Sensor::setLocation(string str){
  int column = 0;
  string token;
  istringstream stream(str);
  while(getline(stream,token,',')){
    switch(column){
      case 0: id = stoi(token); break;
      case 1: lat = stod(token); break;
      case 2: lon = stod(token); break;
      default: break;
    }
    column++;
  }
}

int Sensor::setDATA(int sensorID, string str){
  int column = 0;
  string token;
  istringstream stream(str);
  while(getline(stream,token,',')){
    switch(column){
      case 0:
        if(stoi(token) == sensorID){
          break;
        }else{
          return -1;
        }
      case 1: time.push_back(makeTimestamp(token));break;
      case 2: pm25.push_back(stoi(token)); return 1;
    }
    column++;
  }
  return 1;
}

int makeTimestamp(string s){
	s.substr(0,13);
	s.erase(s.begin()+4);
	s.erase(s.begin()+6);
	s.erase(s.begin()+8);
	return stoi(s);
}

void Sensor::setETS(int begin, int end){
  vector<int>::iterator itr;
  if(ets.empty()){
    for(int i=begin;i<end;i++){
      ets.push_back(i);
    }
  }else{
    for(int i=begin;i<end;i++){
      itr = find(ets.begin(),ets.end(),i);
      if(itr == ets.end()) ets.push_back(i);
    }
    sort(ets.begin(),ets.end());
  }
}

void Sensor::setINTERVAL(int begin, int end){
  interval[begin] = end;
}

void Sensor::setSLOPE(int key, double x){
  slope[key] = x;
}

void Sensor::setSEGMENT(int begin, int end){
  segment[begin] = end;
}

void Sensor::setCLUSTER(double mode, int t, double delta_c){
    cluster.insert(make_pair(mode,t));
}

void Sensor::fixCLUSTER(int theta_c){
  auto cluster_itr = cluster.begin();
  double value = cluster_itr->first;
  double prev = value;
  int count = 0;
  double mini = slope[cluster_itr->second];
  double max = mini;
  while(cluster_itr != cluster.end()){
    if(value != prev){
      if(count < theta_c){
        cluster.erase(prev);
      }else{
        upper[prev] = max;
        lower[prev] = mini;
      }
      count = 0;
      mini = slope[cluster_itr->first];
      max = mini;
      prev = value;
    }
    for(int i=cluster_itr->second;i<=segment[cluster_itr->second];i++){
      count++;
    }
    if(mini > slope[cluster_itr->second]) mini = slope[cluster_itr->second];
    if(max < slope[cluster_itr->second]) max = slope[cluster_itr->second];
    cluster_itr++;
    value = cluster_itr->first;
  }
}
