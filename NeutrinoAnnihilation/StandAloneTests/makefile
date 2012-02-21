# Brett Deaton
# Spring 2012

CC = g++
CFLAGS = -Wall
# EXECUTABLE = TestMonteCarlo2D
# EXECUTABLE = TestMonteCarlo6D
EXECUTABLE = ComputeNuAnnihilationTest2
OBJECTFILES =
GSL_INCLUDE = -DHAVE_INLINE -DGSL_C99_INLINE -DGSL_RANGE_CHECK_OFF
GSL_LIB     = -lgsl -lcblas
all: $(EXECUTABLE)

$(EXECUTABLE): $(EXECUTABLE).cpp $(OBJECTFILES)
	$(CC) $(CFLAGS) $(GSL_INCLUDE) $(GSL_LIB) \
	$(EXECUTABLE).cpp -o $(EXECUTABLE)

clean:
	rm -f *o *~ $(EXECUTABLE)
