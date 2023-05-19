#include "wire_cell.hpp"

#include "cell_stack_matrix.hpp"
#include "signal_cell.hpp"

WireCell::WireCell(Vec pos, CellStackMatrix &env) :
    Cell(pos, env) {}

void WireCell::paint(QPainter &painter) const {
    painter.setBrush(QColor{ 130, 20, 20 });
    painter.drawRect(cellRect());
}

bool WireCell::receiveSignal() {
    m_env.pushCell<SignalCell>(pos());
    return true;
}
