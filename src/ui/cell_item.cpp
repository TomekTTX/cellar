#include "cell_item.hpp"

CellItem::CellItem(Cell &cell) :
    m_cell(cell) {}

QRectF CellItem::boundingRect() const {
    return { 0, 0, CELL_SIZE, CELL_SIZE };
}

void CellItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    m_cell.paint(*painter);
}
