#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP
#define _USE_MATH_DEFINES
/* header file */
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>

/* parameter */
// the number of sensors
const static int M = 180;
// the number of measurements
const static int N = 2048;

using namespace std;

class Sensor{
private:
	int id;
  double lat, lon;
  vector<int> time;
  vector<int> pm25;
  vector<int> ets;
	map<int,int> interval;
	map<int,int> segment;
	map<int,double> slope;
	multimap<double,int> cluster;
	map<double,double> upper;
	map<double,double> lower;
public:
  /* accessor */
  int getID();
  double getLAT();
  double getLON();
  vector<int> getTIME();
  vector<int> getPM25();
  vector<int> getETS();
	int getINTERVAL_END(int begin);
	map<int,int> getINTERVAL();
	int getSEGMENT_END(int begin);
	map<int,int> getSEGMENT();
	double getSLOPE_VALUE(int key);
	map<int,double> getSLOPE();
	multimap<double,int> getCLUSTER();
	double getUPPER(double key);
	double getLOWER(double key);
  /* setter*/
  void setLocation(string str);
  int setDATA(int sensorID, string str);
  void setETS(int begin, int end);
	void setINTERVAL(int begin, int end);
	void setSEGMENT(int begin, int end);
	void setSLOPE(int key, double x);
	void setCLUSTER(double mode, int t, double delta_c);
	void fixCLUSTER(int theta_c);
};

class SCP{
private:
	bool flag;
	double lower;
	double upper;
	vector<int> timestamp;
	vector<int> sensors;
public:
	/* construcor */
	SCP();
	/* accessor */
	bool getFLAG();
	double getLOWER();
	double getUPPER();
	vector<int> getTIMESTAMP();
	vector<int> getSENSORS();
	/* setter */
	void setSCP();
};

#endif
