#include "stack_viewer.hpp"

#include <QPainter>

StackViewer::StackViewer(QWidget *parent) :
    QWidget{ parent } {}

void StackViewer::clear() {
    m_cells = nullptr;
    m_label->setText("");
    repaint();
}

void StackViewer::setStack(const CellStackMatrix::ValueType &stack) {
    m_cells = &stack;
    repaint();
}

void StackViewer::paintEvent(QPaintEvent *event) {
    if (m_cells == nullptr) return;
    QPainter painter{ this };

    if (m_cells->size() != 0) {
        Vec pos = (*m_cells)[0]->pos();
        m_label->setText(QString::fromStdString(pos.str()));
    }

    int i = -1;
    for (const auto &cell : *m_cells) {
        cell->paint(painter, QRect{ ++i * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE });
    }
}
