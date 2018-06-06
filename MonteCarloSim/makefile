CXXFLAGS= -Wall -g
#LDFLAGS=
#COMPOPT=


CXX=g++

CellTest: CellTest.o Cell.o
	$(CXX) $(CXXFLAGS) -o CellTest CellTest.o Cell.o

CellTest.o: CellTest.cpp Cell.hpp
	$(CXX) $(CXXFLAGS) -c CellTest.cpp

LatticeTest: LatticeTest.o Lattice.o Cell.o
	$(CXX) $(CXXFLAGS) -o LatticeTest LatticeTest.o Lattice.o Cell.o

LatticeTest.o: LatticeTest.cpp Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) -c LatticeTest.cpp

Cell.o: Cell.hpp

Lattice.o: Lattice.hpp Cell.hpp
	$(CXX) $(CXXFLAGS) -c Lattice.cpp
