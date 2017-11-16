#include "assembler.hpp"

/* prototype declaring of the functions*/
void inputData(Sensor (&)[M]);
void outputData(Sensor (&)[M]);
void evolvingIntervalExtraction(Sensor (&)[M]);
void evolvingIntervalSegmenting(Sensor (&)[M]);

int main(){
	Sensor sensor[M];
	inputData(sensor);
	cout << "sensor data have been inputted." << endl;
	evolvingIntervalExtraction(sensor);
	cout << "evolving interval have been extracted." << endl;
	evolvingIntervalSegmenting(sensor);
	cout << "evolving interval have been segmented." << endl;

	outputData(sensor);

	return 0;
}
