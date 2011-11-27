##############################################################################
# $Id: Makefile 14856 2010-11-23 23:03:42Z kidder $
##############################################################################
# Makefile for HydroStates
##############################################################################
CODE_HOME = ../../SpEC

SUBDIRECTORIES = 

LINKED_LIBS = \
	HydroStates HydroComputeItems FiniteDifference \
	Functionals SpherePack \
	SpatialCoordMaps Domain Slice Subdomain Topology IndexMap CoordMap \
	EinsteinSolutions TensorFunctions CoordinateTransformations \
	Functionals Domain Slice Subdomain Topology \
	DataMesh IndexMap CoordMap MiscUtils \
	IO DataBox Vars StringParsing Tensor Math \
	MpiWrappers MyContainers LowLevelUtils ErrorHandling

EXECUTABLES           = TOVSequence PlotEOS ConvertToTable StellarProfile TOVAddGridRadius \
	AppendPolytropeCrust

##############################################################################
# Nothing below here needs to be changed
##############################################################################
include $(CODE_HOME)/MakefileRules/this_machine.def
include $(CODE_HOME)/MakefileRules/Rules
