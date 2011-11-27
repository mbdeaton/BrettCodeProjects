# Brett Deaton
# Fall 2011

CC = g++
CFLAGS = -Wall
SOURCEFILES = OutputSin2x.cpp
OBJECTFILES = $(SOURCEFILES:.cpp=.o)
EXECUTABLES = $(SOURCEFILES:.cpp=)

all: $(SOURCEFILES) $(EXECUTABLES)

OutputSin2x: OutputSin2x.cpp
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -rf *o *~ $(EXECUTABLES)
