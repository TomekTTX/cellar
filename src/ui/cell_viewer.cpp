#include "cell_viewer.hpp"

#include <QMouseEvent>
#include <QSizePolicy>

CellViewer::CellViewer(QWidget *parent) :
    m_drawRect(0, 0, CELL_SIZE, CELL_SIZE),
    QWidget{ parent } {
    setMinimumSize(CELL_SIZE + 1, CELL_SIZE + 1);
    setMaximumSize(minimumSize());
    setBaseSize(minimumSize());
}

void CellViewer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(const_cast<Cell *>(m_cell));
    }
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
