#include "assembler.hpp"

/* parameter */
// threshold for neighbor nodes [km]
const static double H = 30.0;
// minimum support
const static int MINI_SUP = 2;
/* global */
static map<int,int> v;

/* prototype declaring of functions */
void scpSearch(Sensor (&)[M],vector<int>,vector<SCP>);
void setSCP(Sensor (&)[M],vector<int>,vector<SCP> &);
void intersecting(Sensor (&)[M],vector<int>,vector<int>,int,vector<SCP> &,SCP);
vector<int> getNeighbors(Sensor (&)[M],vector<int>);
bool checkParent(Sensor (&)[M],vector<int>,vector<int>);
bool checkConnection(Sensor (&)[M],vector<int>);
void visit(Sensor (&)[M],vector<int>,int);
void outputSCP(vector<SCP>);
double dist(double,double,double,double);
double deg2rad(double);

/* main */
void findingSCP(Sensor (&s)[M]){
	vector<SCP> scp_init;
  vector<int> x;
  scpSearch(s,x,scp_init);
}

void scpSearch(Sensor (&s)[M], vector<int> x, vector<SCP> scp){
  /* output spatial co-evolving pattern */
  if(!x.empty()){
    outputSCP(scp);
  }

	/* find neighbor nodes */
  vector<int> ns = getNeighbors(s,x);

  /* search scp */
  for(auto ns_itr=ns.begin();ns_itr!=ns.end();ns_itr++){
		vector<int> y = x;
		y.push_back(*ns_itr);
    sort(y.begin(),y.end());

    /* if x is a parent of y */
    if(checkParent(s,x,y)){
			vector<SCP> scp_y;
			setSCP(s,y,scp_y);
			/* if the scp was found on y */
      if(!scp_y.empty()) scpSearch(s,y,scp_y);
    }
  }
}

void setSCP(Sensor (&s)[M], vector<int> graph, vector<SCP> &scp_y){
	/* start as graph_index = 0 */
	for(int cluster=0;cluster<s[graph[0]].getCLUSTER_NUM();cluster++){
		SCP scp;
		scp.setSENSOR(graph[0],s[graph[0]].getUPPER(cluster),s[graph[0]].getLOWER(cluster));
		vector<int> timestamp = s[graph[0]].getCLUSTER_TIMESTAMP(cluster);
		if(int(graph.size()) > 1){
			intersecting(s,graph,timestamp,1,scp_y,scp);
		}else{
			scp_y.push_back(scp);
		}
	}
}

void intersecting(Sensor (&s)[M], vector<int> graph, vector<int> timestamp1, int graph_index, vector<SCP> &scp_y, SCP scp){
	for(int cluster=0;cluster<s[graph[graph_index]].getCLUSTER_NUM();cluster++){
		vector<int> timestamp2 = s[graph[graph_index]].getCLUSTER_TIMESTAMP(cluster);
		vector<int> intersection;
		set_intersection(timestamp1.begin(),timestamp1.end(),timestamp2.begin(),timestamp2.end(),inserter(intersection,intersection.end()));
		if(int(intersection.size()) >= MINI_SUP){
			scp.setSENSOR(graph[graph_index],s[graph[graph_index]].getUPPER(cluster),s[graph[graph_index]].getLOWER(cluster));
			if(graph_index+1 < int(graph.size())){
				intersecting(s,graph,intersection,graph_index+1,scp_y,scp);
			}else{
				scp_y.push_back(scp);
				scp.remSENSOR();
			}
		}
	}
}

vector<int> getNeighbors(Sensor (&s)[M], vector<int> x){
  vector<int> neighbors;
  if(x.empty()){
    for(int i=0;i<M;i++){
      	neighbors.push_back(i);
    }
  }else{
		for(auto x_itr=x.begin();x_itr!=x.end();x_itr++){
			for(int i=0;i<M;i++){
				double h =dist(s[*x_itr].getLAT(),s[*x_itr].getLON(),s[i].getLAT(),s[i].getLON());
				if(dist(s[*x_itr].getLAT(),s[*x_itr].getLON(),s[i].getLAT(),s[i].getLON()) <= H){
				if(find(x.begin(),x.end(),i) == x.end()){
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

bool checkParent(Sensor (&s)[M], vector<int> parent, vector<int> child){
	if(parent.empty()){
		return true;
	}else{
		for(auto child_itr=child.begin();child_itr!=child.end();child_itr++){
			int keep = *child_itr;
			child.erase(child_itr);
			if(checkConnection(s,child)){
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
}

bool checkConnection(Sensor (&s)[M], vector<int> graph){
  v.clear();
  visit(s,graph,graph[0]);
  for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
    if(v.find(*graph_itr) == v.end()){
      return false;
    }
  }
  return true;
}

void visit(Sensor (&s)[M], vector<int> graph, int start){
  v[start] = 1;
  for(auto graph_itr=graph.begin();graph_itr!=graph.end();graph_itr++){
    if(dist(s[start].getLAT(),s[start].getLON(),s[*graph_itr].getLAT(),s[*graph_itr].getLON()) <= H && v.find(*graph_itr) == v.end()){
      visit(s,graph,*graph_itr);
    }
  }
}

void outputSCP(vector<SCP> scp){
	int scp_num = int(scp.size());
	for(int i=0;i<scp_num;i++){
		vector<int> sensors = scp[i].getSENSORS();
		vector<double> upper = scp[i].getUPPER();
		vector<double> lower = scp[i].getLOWER();
		int sensor_num = int(sensors.size());
		cout << "SCP( k = " << sensors.size() << " )" << endl;
		for(int j=0;j<sensor_num;j++){
			cout << "| s" << sensors[j] << " " << lower[j] << "[/h] ~ " << upper[j] << "[/h]" << endl;
		}
		cout << "| -------------------------------" << endl;
	}
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