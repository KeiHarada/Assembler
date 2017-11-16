#include "assembler.hpp"

void inputData(Sensor (&s)[M]){
	/* file open */
	ifstream  ifsLocations("locations.txt");
	if(ifsLocations.fail()){
		cerr << "locations.txt don't exist.\n";
		exit(0);
	}
	ifstream  ifsMeasurements("measurements.txt");
	if(ifsMeasurements.fail()){
		cerr << "measurements.txt don't exist.\n";
		exit(0);
	}

	/* set sensor info and input their reads */
	string strLocation, strMeasurement;
	int flag = 1;
	for(int m=0;m<M;m++){
		getline(ifsLocations,strLocation);
		s[m].setLocation(strLocation);
		for(int n=0;n<N;n++){
			getline(ifsMeasurements,strMeasurement);
			flag = s[m].setDATA(s[m].getID(),strMeasurement);
			if(flag == -1) {n--;}
		}
	}

	/* file close */
	ifsLocations.close();
	ifsMeasurements.close();
}
