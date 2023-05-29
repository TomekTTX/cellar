#include "clock_cell.hpp"

#include <QDebug>

#include "cell_stack_matrix.hpp"

ClockCell::ClockCell(Vec pos, CellStackMatrix *env, uint delay) :
    Cell(pos, env),
    m_delay(delay) {}

void ClockCell::paintSelf(QPainter &painter, const QRect &rect) const {
    constexpr int MARGIN = 1 + CELL_SIZE / 12;
    constexpr int DMARGIN = 1 + CELL_SIZE / 7;
    const int angle = 5760 * m_ctr / m_delay;

    painter.setBrush(QColor(100, 100, 100));
    painter.drawRect(rect);
    painter.setBrush(QColor(240, 180, 20));
    painter.drawEllipse(rect.adjusted(MARGIN, MARGIN, -MARGIN, -MARGIN));
    painter.setBrush(QColor(50, 70, 230));
    painter.drawPie(rect.adjusted(DMARGIN, DMARGIN, -DMARGIN, -DMARGIN), 90 * 16, -angle);
}

void ClockCell::tick() {
    if (++m_ctr < m_delay) return;
    m_ctr = 0;
    for (Cell &cell : m_env->around(pos()))
        cell.receiveSignal((pos() - cell.pos()).toDir());
}

bool ClockCell::receiveSignal(Dir from) {
    return false;
}

uint ClockCell::serialSize() const {
    return Cell::serialSize() + sizeof(m_delay) + sizeof(m_ctr);
}

std::vector<char> ClockCell::serialize() const {
    auto bytes = Cell::serialize();
    char *data = bytes.data() + Cell::serialSize();

    copyIntoBytes(m_delay, &data);
    copyIntoBytes(m_ctr, &data);

    return bytes;
}

void ClockCell::deserializeFrom(const char **data) {
    Cell::deserializeFrom(data);
    copyFromBytes(m_delay, data);
    copyFromBytes(m_ctr, data);
}
