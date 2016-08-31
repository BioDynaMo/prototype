#ifndef NEURITE_H_
#define NEURITE_H_

#include <Rtypes.h>

// -----------------------------------------------------------------------------
// libraries for specific specialities add functionality
// e.g. libBdmNeuroscience.so
class Neurite {
 public:
  Neurite();

  virtual ~Neurite();

  ClassDef(Neurite, 1);
};

#endif  // NEURITE_H_