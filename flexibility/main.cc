#include <chrono>
#include <utility>

#include "base_cell.h"
#include "neuron.h"
#include "neuron_extension.h"

#include <TFile.h>
#include <TVectorD.h>

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
  for (int i = 0; i < 10e6 + 1; i++) {
    sum += cell.GetFoo();

    // serialize sum every 100000th iteration
    if (i % 100000 == 0) {
      TFile *f = new TFile("flexibility.root", "RECREATE");
      // TVectorD vec_w(1);
      // vec_w(0) = sum;
      // f->WriteObject(&vec_w, "sum");
      f->WriteObject(&cell, "cell");
      f->Close();
    }
  }
  std::cout << "Sum " << sum << " - in " << (timestamp() - start) << "ms"
            << std::endl;

  // read sum from root file
  TFile *g = TFile::Open("flexibility.root");
  // TVectorD *vec_r = (TVectorD*)g->Get("sum");
  CustomNeuron *cell_r;
  g->GetObject("cell", cell_r);
  g->Close();

  // std::cout << "Sum read from ROOT file = " << (*vec_r)[0]
  //           << std::endl;

  std::cout << "[Read from ROOT file]: cell.GetFoo() = " << cell_r->GetFoo()
            << std::endl;
  return 0;
}

long timestamp() {
  namespace sc = std::chrono;
  auto time = sc::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
  return millis.count();
}