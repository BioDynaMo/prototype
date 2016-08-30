#ifndef BASE_CELL_H_
#define BASE_CELL_H_

#include <array>
#include <iostream>
#include <Rtypes.h>

// -----------------------------------------------------------------------------
// core library only defines minimal cell
// e.g. core/base_cell.h -> libBdmCore.so
class BaseCell {
  std::array<double, 3> position_;

 public:
  explicit BaseCell(const std::array<double, 3>& position);

  BaseCell();

  const std::array<double, 3>& GetPosition() const;

 private:
  ClassDefNV(BaseCell, 1);
};

// operation in core that processes cells
template <typename Cell>
void coreOp(Cell* cell) {
  std::cout << cell->GetPosition()[2] << std::endl;
}

#endif  // BASE_CELL_H_