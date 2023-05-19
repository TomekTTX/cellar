#include "cell.hpp"

#include "cell_stack_matrix.hpp"

void Cell::destroySelf() {
    m_destroyFlag = true;  // m_env.extractCell(*this);
}

void Cell::direct(Dir d) {
    if (isMovable()) m_dir = d;
}

void Cell::confirmMove() {
    // m_pos += m_dir;
    m_dir = Dir::NONE;
}
