#include "cell_viewer.hpp"

CellViewer::CellViewer(QWidget *parent) :
    QWidget{ parent } {
    setMinimumSize(CELL_SIZE + 1, CELL_SIZE + 1);
    setContentsMargins(0, 0, 0, 0);
}

void CellViewer::paintEvent(QPaintEvent *event) {
    QPainter painter{ this };
    if (m_cell) {
        m_cell->paint(painter, m_drawRect);
        return;
    }
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::lightGray);
    painter.drawRect(m_drawRect);
    painter.drawLine(m_drawRect.bottomLeft(), m_drawRect.topRight());
}

void CellViewer::resizeEvent(QResizeEvent *event) {
    m_drawRect = QRect((width() - CELL_SIZE) / 2, (height() - CELL_SIZE) / 2, CELL_SIZE, CELL_SIZE);
}
