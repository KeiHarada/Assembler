#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

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
/* ------------------------ */
// the number of sensors
const static int M = 180;
/* ------------------------ */
// the number of measurements
const static int N = 2048;
/* ------------------------- */
// threshold for wavelet cofficient
// @ evolvingIntervalExtraction.cpp -> wavlet()
const static double DELTA = 30.0;
const static double MINI_LEVEL = 0.8;
/* ------------------------- */
// threshold for segmenting time seriese
// @ segmentingTimeSeries.cpp -> segmentin()
const static double MAX_ERROR = 30.0;
/* ------------------------- */
// bandwidth for mean shift
// @ clusteringSegment.cpp -> calcCenter()
const static int W = 10;
/* ------------------------- */

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
	map<int,int> cluster;


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
  /* setter*/
  void setLocation(string str);
  int setDATA(int sensorID, string str);
  void setETS(int begin, int end);
	void setINTERVAL(int begin, int end);
	void setSEGMENT(int begin, int end);
	void setSLOPE(int key, double x);
	void setCLUSTER(int x, int y);
};

#endif
