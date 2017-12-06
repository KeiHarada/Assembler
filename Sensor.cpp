#include "assembler.hpp"

/* prototype declaring of the functions */
int makeTimestamp(string);
int changeSign(double,double);

/* accessor*/
int Sensor::getID(){ return id; }
double Sensor::getLAT(){ return lat; }
double Sensor::getLON(){ return lon; }
vector<int> Sensor::getTIME(){ return time; }
vector<int> Sensor::getPM25(){ return pm25; }
vector<int> Sensor::getETS(){ return ets; }
map<int,int> Sensor::getINTERVAL(){ return interval; }
int Sensor::getSEGMENT_END(int begin){ return segment[begin]; }
map<int,int> Sensor::getSEGMENT(){ return segment; }
double Sensor::getSLOPE_VALUE(int key){ return slope[key]; }
map<int,double> Sensor::getSLOPE(){ return slope;}
multimap<int,int> Sensor::getCLUSTER_FINAL(){ return cluster_final; }
double Sensor::getUPPER(int mode){ return upper[mode]; }
double Sensor::getLOWER(int mode){ return lower[mode]; }
vector<int> Sensor::getCLUSTER_TIMESTAMP(int mode){
  vector<int> timestamp;
  int count = int(cluster_final.count(mode));
  if(count != 0){
    auto itr = cluster_final.find(mode);
    for(int i=0;i<count;i++){
      for(int j = itr->second;j<=segment[itr->second];j++){
        timestamp.push_back(j);
      }
      itr++;
    }
  }
  sort(timestamp.begin(),timestamp.end());
  return timestamp;
}
int Sensor::getCLUSTER_NUM(){
  int count = 0;
  for(auto itr = cluster_final.begin();itr!=cluster_final.end();itr=cluster_final.upper_bound(itr->first)){
		count++;
  }
  return count;
}

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
  while(getline(stream,token,'\t')){
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

void Sensor::setCLUSTER(double mode, int t){
    cluster.insert(make_pair(mode,t));
}

void Sensor::fixCLUSTER(int theta_c, double delta_c){
    int count = 0;
    int mode = 0;
    auto cluster_itr = cluster.begin();
    double prev = cluster_itr->first;
    double mini = slope[cluster_itr->second];
    double max = mini;

  while(cluster_itr != cluster.end()){
		for(int i=cluster_itr->second;i<=segment[cluster_itr->second];i++){
			count++;
		}
		if(mini > slope[cluster_itr->second]) mini = slope[cluster_itr->second];
		if(max < slope[cluster_itr->second]) max = slope[cluster_itr->second];
		cluster_final.insert(make_pair(mode,cluster_itr->second));
		cluster_itr++;
		if(fabs(cluster_itr->first-prev) > delta_c || changeSign(cluster_itr->first,prev) == 1 || cluster_itr == cluster.end()){
      if(count < theta_c) {
        cluster_final.erase(mode);
				mode--;
			}else{
				upper[mode] = max;
				lower[mode] = mini;
			}
			count = 0;
			mini = slope[cluster_itr->second];
      max = mini;
      mode++;
      prev = cluster_itr->first;
		}
  }
}

int changeSign(double a, double b){
	if((a>=0)==(b>=0)){
		return 0;
	}else{
		return 1;
	}
}
