// This example demonstrates how classes can be extended / modified using
// mixins and templates
// Contruction problem is solved using variadic templates and perfect forwarding

#include <array>
#include <vector>
#include <chrono>
#include <utility>
#include <iostream>

// -----------------------------------------------------------------------------
// core library only defines minimal cell
// e.g. core/base_cell.h -> libBdmCore.so
class BaseCell {
  std::array<double, 3> position_;
public:
  explicit BaseCell(const std::array<double, 3>& position)
    : position_(position) {}
  BaseCell() : position_{0, 0, 0} {}
  const std::array<double, 3>& GetPosition() const {
    return position_;
  }
};

// operation in core that processes cells
template<typename Cell>
void coreOp(Cell *cell){
  std::cout << cell->GetPosition()[2] << std::endl;
}

// -----------------------------------------------------------------------------
// libraries for specific specialities add functionality
// e.g. libBdmNeuroscience.so
class Neurite {
};

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
};

// -----------------------------------------------------------------------------
// code written by life scientists using libBdmCore.so and libBdmNeuroscience.so

// customize Neuron (add attribute foo_ and member function GetFoo)
template<typename Base>
class NeuronExtension : public Base {
  double foo_ = 3.14;
public:
  template <class... A>
  explicit NeuronExtension(double foo, const A&... a)
    : foo_{foo}, Base(a...) {}
  NeuronExtension() = default;
  double GetFoo() const {
    return foo_;
  }
};

// custom operation that adds behaviour
template<typename Cell>
void customOp(const Cell& cell) {
  auto neurites = cell->GetNeurites();
  std::cout << neurites->size() << std::endl;
  std::cout << cell->GetFoo() << std::endl;
}

// -----------------------------------------------------------------------------
long timestamp();

int main() {
  // define CustomNeuron
  typedef NeuronExtension<Neuron<BaseCell>> CustomNeuron;
  // note: also Neurites can be modified and then inserted into Neuron
  // typedef NeuriteExtension<Neurite> CustomNeurite;
  // typedef NeuronExtension<Neuron<BaseCell, CustomNeurite>> CustomNeuron;

  // default constructor
  CustomNeuron cell;
  coreOp(&cell);
  customOp(&cell);

  // pass initialization values to NeuronExtension, Neuron and BaseCell
  CustomNeuron cell2(1.2, std::vector<Neurite>{Neurite()},
                     std::array<double, 3>{1, 2, 3});
  coreOp(&cell2);
  customOp(&cell2);

  // small benchmark
  auto start = timestamp();
  double sum = 0;
  for(int i=0; i < 10e6; i++) {
    sum += cell.GetFoo();
  }
  std::cout << "Sum " << sum << " - in " << (timestamp() - start) << "ms"
            << std::endl;

  return 0;
}

long timestamp() {
  namespace sc = std::chrono;
  auto time = sc::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  auto millis = sc::duration_cast < sc::milliseconds > (since_epoch);
  return millis.count();
}
