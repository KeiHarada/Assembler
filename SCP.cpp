#include "assembler.hpp"

/* constructor */
SCP::SCP(){ flag = false; }

/* accessor */
bool SCP::getFLAG(){ return flag; }
vector<int> SCP::getTIMESTAMP(){ return timestamp; }
vector<int> SCP::getSENSORS(){ return sensors; }
vector<double> SCP::getUPPER(){ return upper; }
vector<double> SCP::getLOWER(){ return lower; }

/* setter */
void SCP::setFLAG(bool T){
	flag = T;
}

void SCP::setSENSOR(int number, double most, double least){
	sensors.push_back(number);
	upper.push_back(most);
	lower.push_back(least);
}

void SCP::remSENSOR(){
	sensors.pop_back();
	upper.pop_back();
	lower.pop_back();
}

