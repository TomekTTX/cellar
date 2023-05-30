#include "signal_cell.hpp"

#include "cell_stack_matrix.hpp"
#include "util_funcs.hpp"

SignalCell::SignalCell(Vec pos, CellStackMatrix *env) :
    Cell(pos, env) {}

SignalCell::SignalCell(Vec pos, CellStackMatrix *env, Dir orientation) :
    SignalCell(pos, env) {
    m_lastDir = orientation;
}

void SignalCell::paintSelf(QPainter &painter, const QRect &rect) const {
    painter.setBrush(Qt::red);
    painter.drawRect(rect);
}

void SignalCell::stageDirection() {
    CellStackMatrix::CellSeq targets, around = m_env->around(pos());

    std::copy_if(around.begin(), around.end(), std::back_inserter(targets),
                 [this](Cell &c) { return c.isConductive() && (pos() - m_lastDir) != c.pos(); });

    if (!targets.empty()) {
        auto target = targets[std::rand() % targets.size()];
        m_dir = (target.get().pos() - pos()).toDir();
        m_moved = true;
    } else {
        if (m_lastDir == Dir::NONE) {
            m_dir = DIRS[std::rand() % DIRS.size()];
            m_moved = true;
        } else {
            m_dir = Dir::NONE;
            m_moved = false;
        }
    }
}

void SignalCell::tick() {
    CellStackMatrix::CellSeq seq;

    if (!cellStackIsConductive()) {
        seq = m_env->at(pos());
    } else if (!m_moved) {
        seq = m_env->at(pos() + m_lastDir);
    } else
        return;

    for (Cell &cell : seq)
        cell.receiveSignal(-m_lastDir);
    destroySelf();
}

bool SignalCell::cellStackIsConductive() const {
    for (Cell &cell : m_env->at(pos()))
        if (cell.isConductive()) return true;
    return false;
}

void SignalCell::confirmMove() {
    m_lastDir = dir();
    Cell::confirmMove();
}

uint SignalCell::serialSize() const {
    return Cell::serialSize() + sizeof(m_lastDir) + sizeof(m_moved);
}

std::vector<char> SignalCell::serialize() const {
    auto bytes = Cell::serialize();
    char *data = bytes.data() + Cell::serialSize();

    copyIntoBytes(m_lastDir, &data);
    copyIntoBytes(m_moved, &data);

    return bytes;
}

void SignalCell::deserializeFrom(const char **data) {
    Cell::deserializeFrom(data);
    copyFromBytes(m_lastDir, data);
    copyFromBytes(m_moved, data);
}
