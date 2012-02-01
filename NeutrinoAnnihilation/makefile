# Brett Deaton
# Spring 2012

CC = g++
CFLAGS = -Wall
EXECUTABLE = ComputeNuAnnihilationSinglePoint
OBJECTFILES =
GSL_INCLUDE = -I/opt/local/include/gsl -DHAVE_INLINE -DGSL_C99_INLINE \
              -DGSL_RANGE_CHECK_OFF
GSL_LIB     = -L/opt/local/lib/ -lgsl

all: $(EXECUTABLE)

$(EXECUTABLE): $(EXECUTABLE).cpp $(OBJECTFILES)
	$(CC) $(CFLAGS) $(GSL_INCLUDE) $(GSL_LIB) \
	$(EXECUTABLE).cpp -o $(EXECUTABLE)

clean:
	rm -f *o *~ $(EXECUTABLE)
