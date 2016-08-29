#ifndef __NEURON_EXTENSION_H__
#define __NEURON_EXTENSION_H__

#include <iostream>
#include <Rtypes.h>

// -----------------------------------------------------------------------------
// code written by life scientists using libBdmCore.so and libBdmNeuroscience.so
template <typename Base>
class NeuronExtension : public Base {
  double foo_ = 3.14;

 public:
  template <class... A>
  explicit NeuronExtension(double foo, const A&... a)
      : foo_{foo}, Base(a...) {}
  NeuronExtension() = default;
  double GetFoo() const { return foo_; }

 private:
  ClassDefNV(NeuronExtension, 1);
};

// custom operation that adds behaviour
template <typename Cell>
void customOp(const Cell& cell) {
  auto neurites = cell->GetNeurites();
  std::cout << neurites->size() << std::endl;
  std::cout << cell->GetFoo() << std::endl;
}

#endif