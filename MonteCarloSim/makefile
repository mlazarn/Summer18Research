CXXFLAGS= -Wall -g -I/home/micarn/boost_1_67_0 -std=c++11
LDFLAGS= -L/home/micarn/boost_1_67_0/stage/lib -L/home/micarn/usr/lib/
#COMPOPT=


CXX=g++

CellTest: CellTest.o Cell.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o CellTest CellTest.o Cell.o

CellTest.o: CellTest.cpp Cell.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c CellTest.cpp

LatticeTest: LatticeTest.o Lattice.o Cell.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o LatticeTest LatticeTest.o Lattice.o Cell.o

LatticeTest.o: LatticeTest.cpp Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c LatticeTest.cpp

LatticeMLRPSTest: LatticeMLRPSTest.o LatticeMLRPS.o Lattice.o Cell.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o LatticeMLRPSTest LatticeMLRPSTest.o LatticeMLRPS.o Lattice.o Cell.o -lm -lfftw3

LatticeMLRSPTest.o: LatticeMLRPSTest.cpp LatticeMLRPS.hpp Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c LatticeMLRPSTest.cpp

RenderIsolated: RenderIsolated.o LatticeMLRPS.o Lattice.o Cell.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o RenderIsolated RenderIsolated.o LatticeMLRPS.o Lattice.o Cell.o

RenderIsolated.o: RenderIsolated.cpp LatticeMLRPS.hpp Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c RenderIsolated.cpp

Cell.o: Cell.hpp

Lattice.o: Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c Lattice.cpp

LatticeMLRPS.o: LatticeMLRPS.hpp Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c LatticeMLRPS.cpp
