#include <cmath>
#include <iostream>
#include <sstream>

#include <omp.h>

#include "backend.h"
#include "cell.h"
#include "daosoa.h"
#include "resource_manager.h"
#include "scheduler.h"
#include "timing.h"
#include "timing_aggregator.h"
#include "displacement_op.h"
#include "dividing_cell_op.h"

using bdm::Cell;
using bdm::daosoa;
using bdm::ResourceManager;
using bdm::Scheduler;
using bdm::ScalarBackend;
using bdm::Timing;
using bdm::TimingAggregator;
using bdm::VcBackend;

void execute(size_t cells_per_dim, size_t iterations,
             TimingAggregator* statistic) {
  const unsigned space = 20;

  daosoa<Cell> cells(cells_per_dim * cells_per_dim * cells_per_dim);
  {
    Timing timing("setup", statistic);
    for (size_t i = 0; i < cells_per_dim; i++) {
      for (size_t j = 0; j < cells_per_dim; j++) {
        for (size_t k = 0; k < cells_per_dim; k++) {
          // todo improve syntax
          Cell<ScalarBackend> cell(std::array<ScalarBackend::real_v, 3>{
              i * space, j * space, k * space});
          cell.SetDiameter(30);
          cell.SetAdherence(0.4);
          cell.SetMass(1.0);
          cell.UpdateVolume();
          cells.push_back(cell);
        }
      }
    }
  }

  {
    Timing timing("neighbor_op", statistic);
    bdm::NeighborOp op(700);
    op.Compute(&cells);
  }

  {
    Timing timing("div_op", statistic);
    bdm::DividingCellOp biology;
    for (size_t i = 0; i < iterations; i++) {
      biology.Compute(&cells);
    }
  }

  {
    Timing timing("displacement op", statistic);
    bdm::DisplacementOp op;
    for (size_t i = 0; i < iterations; i++) {
      op.Compute(&cells);
    }
  }
}

void benchmark(size_t cells_per_dim, size_t iterations,
               TimingAggregator* statistic) {
  const int max_threads = omp_get_max_threads();
  for (int i = 1; i <= max_threads; i *= 2) {
    std::stringstream ss;
    ss << i << " thread(s) - " << cells_per_dim << " cells per dim - "
       << iterations << " iteration(s)";
    statistic->AddDescription(ss.str());
    omp_set_num_threads(i);
    execute(cells_per_dim, iterations, statistic);
  }
}

int main(int args, char** argv) {
  std::cout << "Cell<VcBackend> size: " << sizeof(Cell<VcBackend>) << std::endl;

  TimingAggregator statistic;
  if (args > 1 && std::string(argv[1]) == "--scaling-analysis") {
    benchmark(4, 1e5, &statistic);
  } else if (args > 1 && std::string(argv[1]) == "--full-analysis") {
    benchmark(4, 1e5, &statistic);
    benchmark(128, 1, &statistic);
  } else {
    omp_set_num_threads(1);
    execute(4, 1e5, &statistic);
  }
  std::cout << statistic << std::endl;
  return 0;
}
