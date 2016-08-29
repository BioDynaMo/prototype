#ifndef __NEURON_H__
#define __NEURON_H__

#include "neurite.h"

#include <vector>
#include <Rtypes.h>

// adds Neurites to BaseCell
template<typename Base, typename TNeurite=Neurite>
class Neuron : public Base {
  std::vector<TNeurite> neurites_;
public:
  template <class... A>
  explicit Neuron(const std::vector<TNeurite>& neurites, const A&... a)
    : neurites_{neurites}, Base(a...) {}
  Neuron() = default;

  const std::vector<TNeurite>* GetNeurites() const {
    return &neurites_;
  }
 private:
  ClassDefNV(Neuron, 1);
};

#endif
