# Brett Deaton
# Fall 2011

CC = g++
CFLAGS = -Wall
SOURCEFILES = LaneEmdenSolution_RK4.cpp LaneEmdenSolution_Euler.cpp
OBJECTFILES = $(SOURCEFILES:.cpp=.o)
EXECUTABLES = $(SOURCEFILES:.cpp=)

all: $(SOURCEFILES) $(EXECUTABLES)

LaneEmdenSolution_RK4: LaneEmdenSolution_RK4.cpp
	$(CC) $(CFLAGS) $? -o $@

LaneEmdenSolution_Euler: LaneEmdenSolution_Euler.cpp
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -rf *o *~ $(EXECUTABLES)
