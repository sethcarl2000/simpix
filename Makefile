ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTGLIBS  = $(shell root-config --glibs)
CXXFLAGS  += $(ROOTCFLAGS)
LIBS       = $(ROOTLIBS) -lASImage
GLIBS      = $(ROOTGLIBS)
GXX	   = g++ -Wall -O3

simpix_start:  simpix_start.cpp Utils.cpp
	$(GXX) -o simpix_start simpix_start.cpp Utils.cpp $(ROOTCFLAGS) $(LIBS) $(ROOTGLIBS)



clean:
	rm -f simpix_start out.png
