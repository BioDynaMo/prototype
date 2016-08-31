ROOTCFLAGS    				= `root-config --cflags`
ROOTLIBS      				= `root-config --libs`
ROOTGLIBS     				= `root-config --glibs`

CXX           				= g++
CXXFLAGS      				= -I$(ROOTSYS)/include -O -Wall -fPIC -Wno-reorder
LD            				= g++
LDFLAGS       				= -g
SOFLAGS       				= -shared

SOURCES						= main.cc base_cell.cc neurite.cc
BDM_NEUROSCIENCE_HEADERS 	= neurite.h neuron.h
BDM_CORE_HEADERS			= base_cell.h
BDM_CUSTOM_HEADERS			= neuron_extension.h base_cell.h neuron.h

CXXFLAGS     				+= $(ROOTCFLAGS)
LIBS          				= $(ROOTLIBS)
GLIBS         				= $(ROOTGLIBS)

all: flexibility
clean:
	@rm *.o *.pcm flexibility *Dict.cc *.root 2>/dev/null || true

bdmCoreDict.cc: $(BDM_CORE_HEADERS) bmdCoreLinkDef.h
	@rootcling -f $@ -c $(CXXFLAGS) $(BDM_CORE_HEADERS) bmdCoreLinkDef.h

bdmCoreDict.o: bdmCoreDict.cc
	@$(CXX) -c bdmCoreDict.cc $(CXXFLAGS)
	@echo "\033[92mBuilt bdmCoreDict\033[0m"

bdmNeuroscienceDict.cc: $(BDM_NEUROSCIENCE_HEADERS) bdmNeuroscienceLinkDef.h
	@rootcling -f $@ -c $(CXXFLAGS) $(BDM_NEUROSCIENCE_HEADERS) bdmNeuroscienceLinkDef.h

bdmNeuroscienceDict.o: bdmNeuroscienceDict.cc
	@$(CXX) -c bdmNeuroscienceDict.cc $(CXXFLAGS)
	@echo "\033[92mBuilt bdmNeuroscienceDict\033[0m"

bdmCustomDict.cc: $(BDM_CUSTOM_HEADERS) bdmCustomLinkDef.h
	@rootcling -f $@ -c $(CXXFLAGS) $(BDM_CUSTOM_HEADERS) bdmCustomLinkDef.h

bdmCustomDict.o: bdmCustomDict.cc
	@$(CXX) -c bdmCustomDict.cc $(CXXFLAGS)
	@echo "\033[92mBuilt bdmCustomDict\033[0m"

flexibility: $(SOURCES) bdmNeuroscienceDict.o bdmCoreDict.o bdmCustomDict.o
	@$(CXX) -o flexibility $(SOURCES) $(CXXFLAGS) $(GLIBS) bdmNeuroscienceDict.o bdmCoreDict.o bdmCustomDict.o
	@echo "\033[95mBuilt flexibility executable\033[0m"