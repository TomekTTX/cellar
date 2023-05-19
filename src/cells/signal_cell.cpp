#include "signal_cell.hpp"

#include "cell_stack_matrix.hpp"

SignalCell::SignalCell(Vec pos, CellStackMatrix &env) :
    Cell(pos, env) {}

void SignalCell::paint(QPainter &painter) const {
    painter.setBrush(Qt::red);
    painter.drawRect(cellRect());
}

void SignalCell::stageDirection() {
    CellStackMatrix::CellSeq targets, around = m_env.around(pos());

    std::copy_if(around.begin(), around.end(), std::back_inserter(targets),
                 [this](Cell &c) { return c.isConductive() && (pos() - m_lastDir) != c.pos(); });

    if (!targets.empty()) {
        auto target = targets[std::rand() % targets.size()];
        m_dir = (target.get().pos() - pos()).toDir();
        m_moved = true;
    } else {
        m_dir = Dir::NONE;
        m_moved = false;
    }
}

void SignalCell::tick() {
    CellStackMatrix::CellSeq seq;

    if (!cellStackIsConductive()) {
        seq = m_env.at(pos());
    } else if (!m_moved) {
        seq = m_env.at(pos() + m_lastDir);
    } else
        return;

    for (Cell &cell : seq)
        cell.receiveSignal();
    destroySelf();
}

bool SignalCell::cellStackIsConductive() const {
    for (Cell &cell : m_env.at(pos()))
        if (cell.isConductive()) return true;
    return false;
}

void SignalCell::confirmMove() {
    m_lastDir = dir();
    Cell::confirmMove();
}
