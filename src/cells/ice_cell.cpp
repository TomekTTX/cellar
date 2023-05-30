#include "ice_cell.hpp"

#include <QDebug>
#include <QPainterPath>

#include "cell_stack_matrix.hpp"

IceCell::IceCell(Vec pos, CellStackMatrix *env) :
    Cell(pos, env) {}

void IceCell::preMove() {
    if (m_skipPreMove) return;
    std::vector<IceCell *> connectedIce = iceSeq();
    for (IceCell *cell : connectedIce) {
        cell->applyForces();
    }
    for (auto rit = connectedIce.rbegin(); rit != connectedIce.rend(); ++rit) {
        (*rit)->applyForces();
    }
}

void IceCell::tick() {
    m_skipPreMove = m_forceApplied = false;
}

void IceCell::applyForces() {
    m_skipPreMove = true;
    if (m_forceApplied) return;

    CellStackMatrix::CellSeq stacks[] = {
        m_env->at(pos() + Dir::UP + Dir::LEFT),
        m_env->at(pos() + Dir::UP),
        m_env->at(pos() + Dir::UP + Dir::RIGHT),
    };

    for (int i : { 0, 1 }) {
        Dir dirs[] = { Dir::RIGHT, Dir::LEFT };
        for (auto &cellref : stacks[2 * i]) {
            if (cellref.get().dir() == dirs[i]) {
                for (Cell &cell : stacks[1]) {
                    cell.direct(dirs[i], 2);
                }
                m_forceApplied = true;
                break;
            }
        }
    }
}

void IceCell::paintSelf(QPainter &painter, const QRect &rect) const {
    const auto baseSize = rect.width();
    const QPoint offsets[] = {
        { baseSize / 6, baseSize / 7 },
        { 3 * baseSize / 7, baseSize / 4 },
        { 2 * baseSize / 7, 5 * baseSize / 8 },
        { 5 * baseSize / 7, 9 * baseSize / 16 },
    };
    const QPoint p0 = rect.topLeft() + QPoint(0, baseSize / 5);
    const QPoint p1 = rect.topLeft() + QPoint(baseSize / 5, 0);

    painter.save();
    painter.setBrush(QColor(139, 173, 240));
    painter.drawRect(rect);
    painter.setPen(Qt::white);
    for (const QPoint &p : offsets) {
        painter.drawLine(p0 + p, p1 + p);
    }
    painter.restore();
}

std::vector<IceCell *> IceCell::iceSeq() {
    std::vector<IceCell *> cells{ this };

    IceCell *temp = nullptr;
    for (int x = pos().x - 1; m_env->posValid(x, pos().y); --x) {
        for (auto &cellref : m_env->at({ x, pos().y })) {
            if ((temp = dynamic_cast<IceCell *>(&cellref.get())) == nullptr) break;
            cells.push_back(temp);
        }
    }
    for (int x = pos().x + 1; m_env->posValid(x, pos().y); ++x) {
        for (auto &cellref : m_env->at({ x, pos().y })) {
            if ((temp = dynamic_cast<IceCell *>(&cellref.get())) == nullptr) break;
            cells.push_back(temp);
        }
    }

    return cells;
}
