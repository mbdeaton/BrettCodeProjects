# Brett Deaton
# Fall 2011

CC = g++
CFLAGS = -Wall
EXECUTABLE = FindRadialMode
OBJECTFILES = DataWriter.o FindRoot.o Function.o RadialOscillationSolutionForGamma2Polytrope.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(EXECUTABLE).cpp $(OBJECTFILES)
	$(CC) $(CFLAGS) $(EXECUTABLE).cpp -o $(EXECUTABLE)

DataWriter.o: DataWriter.hpp
	$(CC) $(CFLAGS) -c $? -o $@

Function.o: Function.hpp
	$(CC) $(CFLAGS) -c $? -o $@

FindRoot.o: FindRoot.hpp
	$(CC) $(CFLAGS) -c $? -o $@

RadialOscillationSolutionForGamma2Polytrope.o: RadialOscillationSolutionForGamma2Polytrope.hpp
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -f *o *~ $(EXECUTABLE)
