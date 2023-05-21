#include "draw_area.hpp"

#include <memory>
#include <sstream>

#include <QDebug>
#include <QMouseEvent>

#include "cell_types.hpp"


DrawArea::DrawArea(QWidget *parent) :
    QWidget{ parent },
    m_mat(8, 8) {
    testInit();
}

void DrawArea::testInit() {
    for (int i = 0; i < 7; ++i)
        m_mat.emplaceCell<BeltCell>({ i, 3 }, Dir::RIGHT);
    for (int i = 0; i < 7; ++i)
        m_mat.emplaceCell<BeltCell>({ i + 1, 5 }, Dir::LEFT);
    for (int i = 0; i < 7; ++i)
        m_mat.emplaceCell<BeltCell>({ i, 7 }, Dir::RIGHT);
    // m_mat.emplaceCell<DataCell>({ 0, 0 }, Color(255, 140, 140));
    // m_mat.emplaceCell<DataCell>({ 1, 3 }, Color(140, 255, 140));
    // m_mat.emplaceCell<DataCell>({ 2, 5 }, Color(140, 140, 255));
}


void DrawArea::tick() {
    m_mat.tick();
    m_sv->repaint();
}

void DrawArea::mousePressEvent(QMouseEvent *event) {
    const auto point = event->position().toPoint();
    const Vec cellPos = m_mat.mapPosToCell(point.x(), point.y());

    if (event->button() == Qt::LeftButton) {
        // qDebug() << cellPos.x << cellPos.y;
        if (m_mat.posValid(cellPos)) {
            m_sv->setStack(m_mat[cellPos]);
        } else {
            m_sv->clear();
        }
    } else if (event->button() == Qt::RightButton) {
        if (!m_mat.posValid(cellPos)) return;
        if (std::unique_ptr<Cell> cell = m_palette->getCell()) {
            cell->setPos(cellPos);
            m_mat.pushCell(std::move(cell));
            repaint();
        }
    }
}

void DrawArea::paintEvent(QPaintEvent *event) {
    QPainter painter{ this };

    for (const auto &stack : m_mat)
        stack.back()->paint(painter);
}
