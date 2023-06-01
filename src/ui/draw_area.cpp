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
    m_mat.emplaceCell<FanCell>({ 5, 0 }, Dir::DOWN, -5, 6);
}


void DrawArea::tick() {
    select(nullptr);
    m_mat.tick();
    m_sv->repaint();
}

void DrawArea::mousePressEvent(QMouseEvent *event) {
    const auto point = event->position().toPoint();
    const Vec cellPos = m_mat.mapPosToCell(point.x(), point.y());

    if (event->button() == Qt::LeftButton) {
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

void DrawArea::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        const auto point = event->pos();
        const Vec cellPos = m_mat.mapPosToCell(point.x(), point.y());
        if (m_mat.posValid(cellPos) && !m_mat[cellPos].empty()) {
            select(m_mat[cellPos].back().get());
        } else
            select(nullptr);
    }
}

void DrawArea::select(Cell *cell) {
    if (m_selectedCell) {
        m_selectedCell->setSelected(false);
    }
    if (cell) {
        cell->setSelected(true);
    }
    m_selectedCell = cell;
    m_sv->repaint();
    repaint();
}

void DrawArea::deleteSelected() {
    if (m_selectedCell) {
        m_mat.extractCell(*m_selectedCell);
        m_selectedCell = nullptr;
    }
    select(nullptr);
}

void DrawArea::moveSelectedUp() {
    if (!m_selectedCell) return;
    if (m_mat.moveCellUp(*m_selectedCell)) {
        repaint();
        m_sv->repaint();
    }
}

void DrawArea::moveSelectedDown() {
    if (!m_selectedCell) return;
    if (m_mat.moveCellDown(*m_selectedCell)) {
        repaint();
        m_sv->repaint();
    }
}

void DrawArea::cut() {
    copy();
    deleteSelected();
}

void DrawArea::copy() {
    if (!m_selectedCell) return;
    m_cellCopy = m_selectedCell->serialize();
}

void DrawArea::paste() {
    if (!m_selectedCell) return;
    m_mat.pushCell(Cell::deserialize(m_cellCopy), m_selectedCell->pos());
    repaint();
    m_sv->repaint();
}

void DrawArea::paintEvent(QPaintEvent *event) {
    QPainter painter{ this };

    for (const auto &stack : m_mat) {
        if (!stack.empty()) {
            stack.back()->paint(painter);
        }
    }
}
