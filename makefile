assembler: assembler.o input.o Sensor.o evolvingIntervalExtraction.o segmentingTimeSeries.o output.o
	g++ -Wall -o assembler assembler.o input.o Sensor.o evolvingIntervalExtraction.o segmentingTimeSeries.o output.o
assembler.o: assembler.cpp
	g++ -Wall -c assembler.cpp -o assembler.o
input.o: input.cpp
	g++ -Wall -c input.cpp -o input.o
Sensor.o: Sensor.cpp
	g++ -Wall -c Sensor.cpp -o Sensor.o
evolvingIntervalExtraction.o: evolvingIntervalExtraction.cpp
	g++ -Wall -c evolvingIntervalExtraction.cpp -o evolvingIntervalExtraction.o
segmentingTimeSeries.o: segmentingTimeSeries.cpp
	g++ -Wall -c segmentingTimeSeries.cpp -o segmentingTimeSeries.o
output.o: output.cpp
	g++ -Wall -c output.cpp -o output.o
clean:
	-del *.o
