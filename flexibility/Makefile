ROOTCFLAGS    = `root-config --cflags`
ROOTLIBS      = `root-config --libs`
ROOTGLIBS     = `root-config --glibs`

# Linux with egcs
CXX           = g++
CXXFLAGS      =-I$(ROOTSYS)/include -O -Wall -fPIC -Wno-reorder
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared

SOURCES		  = main.cc base_cell.cc neurite.cc
HEADERS		  = base_cell.h neurite.h neuron.h neuron_extension.h

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)

all: flexibility
clean:
	rm *.o *C.*

flexDict.cc: $(HEADERS) LinkDef.h
	rootcling -f $@ -c $(CXXFLAGS) $(HEADERS) LinkDef.h
flexDict.o: flexDict.cc
	$(CXX) -c flexDict.cc $(CXXFLAGS)

flexibility: $(SOURCES) flexDict.o
	$(CXX) -o flexibility $(SOURCES) $(CXXFLAGS) $(GLIBS) flexDict.o

