#include "stack_viewer.hpp"

#include <QMouseEvent>
#include <QPainter>

StackViewer::StackViewer(QWidget *parent) :
    QWidget{ parent } {
    setMinimumHeight(CELL_SIZE);
}

void StackViewer::clear() {
    m_cells = nullptr;
    m_label->setText("");
    repaint();
}

void StackViewer::setStack(const CellStackMatrix::ValueType &stack) {
    m_cells = &stack;
    repaint();
}

void StackViewer::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        const int index = event->pos().x() / CELL_SIZE;
        if (index < m_cells->size()) {
            emit clicked(const_cast<Cell *>(m_cells->at(index).get()));
        }
    }
}

void StackViewer::paintEvent(QPaintEvent *event) {
    if (m_cells == nullptr) return;
    QPainter painter{ this };
    //    painter.setBrush(Qt::lightGray);
    //    painter.setPen(Qt::transparent);
    //    painter.drawRect(rect());

    if (m_cells->size() != 0) {
        Vec pos = (*m_cells)[0]->pos();
        m_label->setText(QString::fromStdString(pos.str()));
    }

    int i = -1;
    for (const auto &cell : *m_cells) {
        cell->paint(painter, QRect{ ++i * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE });
    }
}
