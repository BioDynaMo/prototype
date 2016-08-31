#include "base_cell.h"

BaseCell::BaseCell(const std::array<double, 3>& position)
    : position_(position) {}

BaseCell::BaseCell() : position_{0, 0, 0} {}

const std::array<double, 3>& BaseCell::GetPosition() const { return position_; }