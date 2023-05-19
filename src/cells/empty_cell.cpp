#include "empty_cell.hpp"

EmptyCell::EmptyCell(Vec pos, CellStackMatrix &env) :
    Cell(pos, env) {}

void EmptyCell::paint(QPainter &painter) const {
    painter.setBrush(Qt::white);
    painter.drawRect(cellRect());
}
