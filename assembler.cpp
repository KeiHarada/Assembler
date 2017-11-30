#include "assembler.hpp"

/* prototype declaring of the functions*/
void inputData(Sensor (&)[M]);
void outputData(Sensor (&)[M]);
void extractingEvolvingInterval(Sensor (&)[M]);
void segmentingEvolvingInterval(Sensor (&)[M]);
void clusteringSegment(Sensor (&)[M]);
void findingSCP(Sensor (&)[M]);


int main(){
	Sensor sensor[M];
	inputData(sensor);
	cout << "sensor data have been inputted." << endl;
	extractingEvolvingInterval(sensor);
	cout << "evolving interval have been extracted." << endl;
	segmentingEvolvingInterval(sensor);
	cout << "evolving interval have been segmented." << endl;
	clusteringSegment(sensor);
	cout << "the segments have been divided into some clusters" << endl;
	findingSCP(sensor);
	cout << "scp search has been completed" << endl;
	outputData(sensor);
	return 0;
}
