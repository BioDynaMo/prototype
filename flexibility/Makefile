ROOTCFLAGS    				= `root-config --cflags`
ROOTLIBS      				= `root-config --libs`
ROOTGLIBS     				= `root-config --glibs`

CXX           				= g++
CXXFLAGS      				=-I$(ROOTSYS)/include -O -Wall -fPIC -Wno-reorder
LD            				= g++
LDFLAGS       				= -g
SOFLAGS       				= -shared

SOURCES						= main.cc base_cell.cc neurite.cc
BDM_NEUROSCIENCE_HEADERS 	= neurite.h
BDM_CORE_HEADERS			= neuron.h neuron_extension.h base_cell.h

CXXFLAGS     				+= $(ROOTCFLAGS)
LIBS          				= $(ROOTLIBS)
GLIBS         				= $(ROOTGLIBS)

all: flexibility
clean:
	rm *.o *C.*

bdmCoreDict.cc: $(BDM_CORE_HEADERS) bmdCoreLinkDef.h
	rootcling -f $@ -c $(CXXFLAGS) $(BDM_CORE_HEADERS) bmdCoreLinkDef.h

bdmCoreDict.o: bdmCoreDict.cc
	$(CXX) -c bdmCoreDict.cc $(CXXFLAGS)

bdmNeuroscienceDict.cc: $(BDM_NEUROSCIENCE_HEADERS) bdmNeuroscienceLinkDef.h
	rootcling -f $@ -c $(CXXFLAGS) $(BDM_NEUROSCIENCE_HEADERS) bdmNeuroscienceLinkDef.h

bdmNeuroscienceDict.o: bdmNeuroscienceDict.cc
	$(CXX) -c bdmNeuroscienceDict.cc $(CXXFLAGS)

flexibility: $(SOURCES) bdmNeuroscienceDict.o bdmCoreDict.o
	$(CXX) -o flexibility $(SOURCES) $(CXXFLAGS) $(GLIBS) bdmNeuroscienceDict.o bdmCoreDict.o