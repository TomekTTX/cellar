#include "empty_cell.hpp"

EmptyCell::EmptyCell(Vec pos, CellStackMatrix *env) :
    Cell(pos, env) {}

void EmptyCell::paintSelf(QPainter &painter, const QRect &rect) const {
    painter.setBrush(Qt::white);
    painter.drawRect(rect);
}
