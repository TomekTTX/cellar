#include "wire_cell.hpp"

#include "cell_stack_matrix.hpp"
#include "signal_cell.hpp"

WireCell::WireCell(Vec pos, CellStackMatrix *env) :
    Cell(pos, env) {}

void WireCell::paint(QPainter &painter, const QRect &rect) const {
    painter.setBrush(QColor{ 130, 20, 20 });
    painter.drawRect(rect);
}

bool WireCell::receiveSignal(Dir from) {
    m_env->emplaceCell<SignalCell>(pos(), -from);
    return true;
}
