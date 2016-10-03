#include <cmath>
#include <iostream>

#include <omp.h>

#include "cell.h"
#include "daosoa.h"
#include "resource_manager.h"
#include "scheduler.h"
#include "timing.h"
#include "timing_aggregator.h"
#include "displacement_op.h" //fixme remove
#include "dividing_cell_op.h" //fixme remove
#include "vtune_op_wrapper.h" //fixme remove
#include "ittnotify.h"

using bdm::Cell;
using bdm::daosoa;
using bdm::ResourceManager;
using bdm::Scheduler;
using bdm::Timing;
using bdm::TimingAggregator;


void execute(TimingAggregator* statistic) {
  const unsigned space = 20;
  const unsigned n_elements_pe_dim = 256;

  Timing timing;
  auto start = timing.timestamp();
  daosoa<Cell> cells(n_elements_pe_dim * n_elements_pe_dim * n_elements_pe_dim);
  auto stop = timing.timestamp();
  std::cout << "reserve cells " << (stop - start) << std::endl;

  {
    Timing timing("setup", statistic);
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
    Timing timing("neighbor_op", statistic);
    bdm::VTuneOpWrapper<bdm::NeighborOp> op(700);
    op.Compute(&cells);
  }

  __itt_resume();

  {
    Timing timing("div_op", statistic);
    bdm::VTuneOpWrapper<bdm::DividingCellOp> biology;
    biology.Compute(&cells);
  }

  {
    Timing timing("displacement op", statistic);
    bdm::VTuneOpWrapper<bdm::DisplacementOp> op;
    op.Compute(&cells);
  }
}

int main(int args, char** argv) {
  ////  Random::setSeed(2L);

  ///  auto c = CellFactory::getCellInstance( { 0.0, 0.0, 0.0 });
  ///  c->addCellModule(CellModule::UPtr { new DividingModule() });



  TimingAggregator statistic;
  if(args > 1 && std::string(argv[1]) == "--scaling-analysis") {
    const int max_threads = omp_get_max_threads();
    for (int i = 1; i <= max_threads; i *= 2) {
      std::cout << "run with " << i << " threads" << std::endl;
      omp_set_num_threads(i);
      execute(&statistic);
    }
  } else {
//    omp_set_num_threads(2);
    execute(&statistic);
  }
  std::cout << statistic << std::endl;

//  ResourceManager<VcBackend>::Get()->SetCells(cells);
//
//  Scheduler scheduler;
//  {
//    Timing timing("simulation");
//    scheduler.Simulate<VcBackend>(2);
//  }
  return 0;
}
