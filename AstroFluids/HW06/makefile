# Brett Deaton
# Fall 2011

CC = g++
CFLAGS = -Wall
SOURCEFILES = ComputeAnalyticRiemannSolution.cpp
OBJECTFILES = $(SOURCEFILES:.cpp=.o)
EXECUTABLES = $(SOURCEFILES:.cpp=)

all: $(SOURCEFILES) $(EXECUTABLES)

FindRoot: FindRoot.cpp
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -rf *o *~ $(EXECUTABLES)
