#ifndef __NEURITE_H__
#define __NEURITE_H__

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

#endif