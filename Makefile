ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTGLIBS  = $(shell root-config --glibs)
CXXFLAGS  += $(ROOTCFLAGS)
LIBS       = $(ROOTLIBS) -lASImage
GLIBS      = $(ROOTGLIBS)
GXX	   = g++ -Wall -O3

simpix:  simpix.cpp Utils.cpp Utils.hpp
	$(GXX) -o simpix simpix.cpp Utils.cpp $(ROOTCFLAGS) $(LIBS) $(ROOTGLIBS)

clean:
	rm -f simpix out.png
