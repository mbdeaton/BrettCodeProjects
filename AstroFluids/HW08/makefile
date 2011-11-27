# Brett Deaton
# Fall 2011

CC = g++
CFLAGS = -Wall
EXECUTABLE = EvolveRiemannShockTube
OBJECTFILES = DataWriter.o Function.o HydroInverter.o FDSolution.o AnalyticShockTubeSolution.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(EXECUTABLE).cpp $(OBJECTFILES)
	$(CC) $(CFLAGS) $(EXECUTABLE).cpp -o $(EXECUTABLE)

DataWriter.o: DataWriter.hpp
	$(CC) $(CFLAGS) -c $? -o $@

Function.o: Function.hpp
	$(CC) $(CFLAGS) -c $? -o $@

HydroInverter.o: HydroInverter.hpp
	$(CC) $(CFLAGS) -c $? -o $@

FDSolution.o: FDSolution.hpp
	$(CC) $(CFLAGS) -c $? -o $@

AnalyticShockTubeSolution.o: AnalyticShockTubeSolution.hpp
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -f *o *~ $(EXECUTABLE)
