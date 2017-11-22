#include "assembler.hpp"

/* parameter */
// threshould for neighbor nodes [km]
const static double H = 30.0;

/* global */
static map<int,int> v;

/* prototype declaring of functions */
void scpSearch(array<array<double,M>,M>,vector<int>,SCP &);
vector<int> getNeighbors(array<array<double,M>,M>,vector<int>);
bool checkMember(vector<int>,int);
bool checkParent(array<array<double,M>,M>,vector<int>,vector<int>);
bool checkConnection(array<array<double,M>,M>,vector<int>);
void visit(array<array<double,M>,M>,vector<int>,int);
void outputSCP(SCP);
double dist(double,double,double,double);
double deg2rad(double);

/* main */
void findingSCP(Sensor (&s)[M]){
  array<array<double,M>,M> distance;
  for(int i=0;i<M;i++){
    for(int j=i;j<M;j++){
      if(i==j) distance[i][j] = 0.0;
      distance[i][j] = dist(s[i].getLAT(),s[i].getLON(),s[j].getLAT(),s[j].getLON());
      distance[j][i] = distance[i][j];
    }
  }
  SCP scp_init;
  vector<int> x;
  scpSearch(distance,x,scp_init);

}

void scpSearch(array<array<double,M>,M> distance, vector<int> x, SCP &scp){
  if(scp.getFLAG()){
    outputSCP(scp);
  }
  vector<int> ns = getNeighbors(distance,x);
  for(auto ns_itr=ns.begin();ns_itr!=ns.end();ns_itr++){
    vector<int> y = x;
    y.push_back(*ns_itr);
    sort(y.begin(),y.end());
    if(checkParent(distance,x,y)){
      SCP scp_y;
      scp_y.setSCP();
      if(scp_y.getFLAG()) scpSearch(distance,y,scp_y);
    }
  }
}

vector<int> getNeighbors(array<array<double,M>,M> distance, vector<int> x){
  vector<int> neighbors;
  if(x.empty()){
    for(int i=0;i<M;i++){
      neighbors.push_back(i);
    }
  }else{
    for(auto x_itr=x.begin();x_itr!=x.end();x_itr++){
      for(int i=0;i<M;i++){
        if(!checkMember(x,i)){
        if(distance[*x_itr][i] <= H){
        if(find(neighbors.begin(),neighbors.end(),i) == neighbors.end()){
            neighbors.push_back(i);
        }
        }
        }
      }
    }
  }
  sort(neighbors.begin(),neighbors.end());
  return neighbors;
}

bool checkMember(vector<int> x, int i){
  for(auto x_itr=x.begin();x_itr!=x.end();x_itr++){
    if(*x_itr == i) return true;
  }
  return false;
}

bool checkParent(array<array<double,M>,M> distance, vector<int> parent, vector<int> child){
  for(auto child_itr=child.begin();child_itr!=child.end();child_itr++){
    int keep = *child_itr;
    child.erase(child_itr);
    if(checkConnection(distance,child)){
      if(child == parent){
        return true;
      }else{
        return false;
      }
    }else{
      child.insert(child_itr,keep);
    }
  }
}

bool checkConnection(array<array<double,M>,M> distance, vector<int> graph){
  v.clear();
  visit(distance,graph,graph[0]);
  for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
    if(v.find(*graph_itr) == v.end()){
      return false;
    }
  }
  return true;
}

void visit(array<array<double,M>,M> distance, vector<int> graph, int start){
  v[start] = 1;
  for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
    if(distance[start][*graph_itr] >= H && v.find(*graph_itr) == v.end()){
      visit(distance,graph,*graph_itr);
    }
  }
}

void outputSCP(SCP scp){

}

double dist(double lat1, double lon1, double lat2, double lon2){
  /* calculates distance using hubeny formula */

  double R_E = 6378137.0; // Equatorial radius
  double R_P = 6356752.314140356; // Polar radius
  double E = (R_E*R_E - R_P*R_P)/(R_E*R_E); //(First eccentricity)^2
  double CR_MA = R_E*(1.0-E); // Curvature radius of meridian on equator

  double delta_lat = deg2rad(lat1)-deg2rad(lat2);
  double delta_lon = deg2rad(lon1)-deg2rad(lon2);
  double lat_ave = (deg2rad(lat1)+deg2rad(lat2))/2.0;
  double sin_lat = sin(lat_ave);

  double w = 1.0 - E*(sin_lat*sin_lat);
  double CR_M = CR_MA/(sqrt(w)*w); // Curvature radius of meridian
  double CR_P = R_E/sqrt(w); // Curvature radius of prime vertical

  double t1 = CR_M*delta_lat;
  double t2 = CR_P*cos(lat_ave)*delta_lon;

  return sqrt((t1*t1)+(t2*t2))/1000.0;
}

double deg2rad(double deg){
  return (deg*M_PI/180.0);
}
