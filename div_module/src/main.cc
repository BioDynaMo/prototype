#include <cmath>
#include <iostream>

#include "cell.h"
#include "daosoa.h"
#include "resource_manager.h"
#include "scheduler.h"
#include "timing.h"
#include "displacement_op.h" //fixme remove
#include "dividing_cell_op.h" //fixme remove

using bdm::Cell;
using bdm::daosoa;
using bdm::ResourceManager;
using bdm::Scheduler;
using bdm::Timing;


int main() {
  ////  Random::setSeed(2L);

  ///  auto c = CellFactory::getCellInstance( { 0.0, 0.0, 0.0 });
  ///  c->addCellModule(CellModule::UPtr { new DividingModule() });
  const unsigned space = 20;
  const unsigned n_elements_pe_dim = 128;
  daosoa<Cell> cells(n_elements_pe_dim * n_elements_pe_dim * n_elements_pe_dim);

  {
    Timing timing("setup");
    int counter = 0;
    for (size_t i = 0; i < n_elements_pe_dim; i++) {
      for (size_t j = 0; j < n_elements_pe_dim; j++) {
        for (size_t k = 0; k < n_elements_pe_dim; k++) {
          // todo that's ugly
          Cell<ScalarBackend> cell(std::array<ScalarBackend::real_v, 3> { i * space, j * space, k * space });
          cell.SetDiameter(30);
          cell.SetAdherence(0.4);
          cell.SetMass(1.0);
          cell.UpdateVolume();
          cells.SetScalar(counter++, cell);
        }
      }
    }
  }

  {
    Timing timing("neighbor_op");
    bdm::NeighborOp op(700);
    op.Compute(&cells);
  }

  {
    Timing timing("div_op");
    bdm::DividingCellOp biology;
    biology.Compute(&cells);
  }

  {
    Timing timing("displacement op");
    bdm::DisplacementOp op;
    op.Compute(&cells);
  }


//  ResourceManager<VcBackend>::Get()->SetCells(cells);
//
//  Scheduler scheduler;
//  {
//    Timing timing("simulation");
//    scheduler.Simulate<VcBackend>(2);
//  }
  return 0;
}
