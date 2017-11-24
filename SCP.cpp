#include "assembler.hpp"

/* construcor */
SCP::SCP(){ flag = false; }

/* accessor */
bool SCP::getFLAG(){ return flag; }
double SCP::getLOWER(){ return lower; }
double SCP::getUPPER(){ return upper; }
vector<int> SCP::getTIMESTAMP(){ return timestamp; }
vector<int> SCP::getSENSORS(){ return sensors; }

/* setter */
void SCP::setSCP(){
  
}
