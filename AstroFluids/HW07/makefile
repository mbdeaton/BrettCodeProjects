# Brett Deaton
# Fall 2011

CC = g++
CFLAGS = -Wall
EXECUTABLE = EvolveAdvectionEquation
OBJECTFILES = OneDimDataWriter.o Function.o FDSolution.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(EXECUTABLE).cpp $(OBJECTFILES)
	$(CC) $(CFLAGS) $(EXECUTABLE).cpp -o $(EXECUTABLE)

OneDimDataWriter.o: OneDimDataWriter.hpp
	$(CC) $(CFLAGS) -c $? -o $@

Function.o: Function.hpp
	$(CC) $(CFLAGS) -c $? -o $@

FDSolution.o: FDSolution.hpp
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -f *o *~ $(EXECUTABLE)
