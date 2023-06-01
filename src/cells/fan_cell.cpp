#include "fan_cell.hpp"

#include <numeric>

#include "cell_stack_matrix.hpp"
#include "util_funcs.hpp"

FanCell::FanCell(Vec pos, CellStackMatrix *env, Dir facing, int8_t force, int16_t reach) :
    Cell(pos, env),
    m_facing(facing),
    m_force(force),
    m_reach(reach) {}

void FanCell::postStage() {
    if (!m_active) return;
    m_animationTick = (m_animationTick + 1) % ANIM_LENGTH;

    Vec curPos = pos();
    for (int16_t i = 0; i < m_reach; ++i) {
        bool boostForce =
            ((m_facing == Dir::UP && m_force == 1) || (m_facing == Dir::DOWN && m_force == -1) && i != m_reach - 1);
        Dir pushDir = m_force < 0 ? -m_facing : m_facing;
        curPos += m_facing;
        for (Cell &cell : m_env->at(curPos)) {
            if (!cell.isMovable() && cell.isSolid()) return;
            cell.direct(pushDir, std::abs(m_force) + boostForce);
        }
    }
}

bool FanCell::receiveSignal(Dir from) {
    m_active ^= true;
    return true;
}

uint FanCell::serialSize() const {
    return Cell::serialSize() + sizeof(m_active) + sizeof(m_force) + sizeof(m_reach) + sizeof(m_animationTick) +
           sizeof(m_facing);
}

std::vector<char> FanCell::serialize() const {
    auto bytes = Cell::serialize();
    char *data = bytes.data() + Cell::serialSize();

    copyIntoBytes(m_active, &data);
    copyIntoBytes(m_force, &data);
    copyIntoBytes(m_reach, &data);
    copyIntoBytes(m_animationTick, &data);
    copyIntoBytes(m_facing, &data);

    return bytes;
}

void FanCell::paintSelf(QPainter &painter, const QRect &rect) const {
    const int size = rect.width();
    const int diff = size / 2;
    const int smalldiff = std::max(size / 20, 1);
    const float angle = (2 * 3.1415926f) * m_animationTick / ANIM_LENGTH;
    QRectF sideRect;

    switch (m_facing) {
    case Dir::UP: sideRect = { rect.topLeft(), rect.topRight() + QPoint(0, diff) }; break;
    case Dir::RIGHT: sideRect = { rect.topRight() - QPoint(diff, 0), rect.bottomRight() }; break;
    case Dir::DOWN: sideRect = { rect.bottomLeft() - QPoint(0, diff), rect.bottomRight() }; break;
    case Dir::LEFT: sideRect = { rect.topLeft(), rect.bottomLeft() + QPoint(diff, 0) }; break;
    default: sideRect = {}; break;
    }
    sideRect.adjust(smalldiff, smalldiff, -smalldiff, -smalldiff);

    painter.save();
    painter.setBrush(QColor(130, 140, 140));
    painter.drawRect(rect);
    painter.setBrush(QColor(170, 170, 170));
    painter.drawEllipse(sideRect);

    sideRect.adjust(2 * smalldiff, 2 * smalldiff, -2 * smalldiff, -2 * smalldiff);
    QPointF offset = (QPointF(sideRect.width() * std::cos(angle), sideRect.height() * std::sin(angle)) / 2);
    painter.setPen(QPen(QColor(230, 230, 230), smalldiff));
    painter.drawLine(sideRect.center() + offset, sideRect.center() - offset);
    QPointF middleOffset = QPointF(sideRect.width(), sideRect.height()) / 7;
    painter.setPen(Qt::black);
    painter.setBrush(QColor(230, 230, 230));
    painter.drawEllipse(QRectF(sideRect.center() - middleOffset, sideRect.center() + middleOffset));

    painter.restore();
}

void FanCell::deserializeFrom(const char **data) {
    Cell::deserializeFrom(data);
    copyFromBytes(m_active, data);
    copyFromBytes(m_force, data);
    copyFromBytes(m_reach, data);
    copyFromBytes(m_animationTick, data);
    copyFromBytes(m_facing, data);
}
