#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP
#define _USE_MATH_DEFINES

/* header file */
#include <cstdlib>
#include <cfloat>
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
	multimap<int,int> cluster_final;
	map<int,double> upper;
	map<int,double> lower;
public:
  /* accessor */
	int getID();
  double getLAT();
  double getLON();
  vector<int> getTIME();
  vector<int> getPM25();
  vector<int> getETS();
	map<int,int> getINTERVAL();
	int getSEGMENT_END(int begin);
	map<int,int> getSEGMENT();
	double getSLOPE_VALUE(int key);
	map<int,double> getSLOPE();
	multimap<int,int> getCLUSTER_FINAL();
	vector<int> getCLUSTER_TIMESTAMP(int mode);
	int getCLUSTER_NUM();
	double getUPPER(int mode);
	double getLOWER(int mode);
  /* setter*/
  void setLocation(string str);
  int setDATA(int sensorID, string str);
  void setETS(int begin, int end);
	void setINTERVAL(int begin, int end);
	void setSEGMENT(int begin, int end);
	void setSLOPE(int key, double x);
	void setCLUSTER(double mode, int t);
	void fixCLUSTER(int theta_c, double delta_c);
};

class SCP{
private:
	vector<int> timestamp;
	vector<int> sensors;
	vector<double> upper;
	vector<double> lower;
public:
	/* constructor */
	SCP();
	/* accessor */
	vector<int> getTIMESTAMP();
	vector<int> getSENSORS();
	vector<double> getUPPER();
	vector<double> getLOWER();
	/* setter */
	void setSENSOR(int number, double most, double least);
	void remSENSOR();
};

#endif
