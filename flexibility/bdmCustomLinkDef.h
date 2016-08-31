#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ class Neuron<BaseCell>+;
#pragma link C++ class NeuronExtension<Neuron<BaseCell> >+;

#endif