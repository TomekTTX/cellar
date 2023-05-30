#include "cell.hpp"

#include "cell_stack_matrix.hpp"
#include "cell_types.hpp"
#include "util_funcs.hpp"

void Cell::destroySelf() {
    m_destroyFlag = true;  // m_env->extractCell(*this);
}

QRect Cell::cellRect() const {
    return { CELL_SIZE * m_pos.x, CELL_SIZE * m_pos.y, CELL_SIZE, CELL_SIZE };
}

std::array<QPolygon, 4> polygons(QPoint origin) {
    constexpr float T0 = 0.0F, T1 = 0.1F, T2 = 0.3F, T3 = 1 - T2, T4 = 1 - T1, T5 = 1 - T0;
    const auto pt = [origin](float x, float y) -> QPoint {
        return {
            origin.x() + static_cast<int>(CELL_SIZE * x),
            origin.y() + static_cast<int>(CELL_SIZE * y),
        };
    };

    return {
        QPolygon{ pt(T0, T0), pt(T2, T0), pt(T2, T1), pt(T1, T1), pt(T1, T2), pt(T0, T2) },
        QPolygon{ pt(T3, T0), pt(T5, T0), pt(T5, T2), pt(T4, T2), pt(T4, T1), pt(T3, T1) },
        QPolygon{ pt(T4, T3), pt(T5, T3), pt(T5, T5), pt(T3, T5), pt(T3, T4), pt(T4, T4) },
        QPolygon{ pt(T0, T3), pt(T1, T3), pt(T1, T4), pt(T2, T4), pt(T2, T5), pt(T0, T5) },
    };
}

void Cell::paint(QPainter &painter) const {
    paint(painter, cellRect());
}

void Cell::paint(QPainter &painter, const QRect &rect) const {
    paintSelf(painter, rect);

    if (m_selected) {
        const QPoint offset{ rect.x(), rect.y() };
        painter.setPen(Qt::black);
        painter.setBrush(QColor(255, 160, 10));
        for (const QPolygon &poly : polygons(offset))
            painter.drawPolygon(poly);
    }
}


void Cell::direct(Dir d, int8_t force) {
    if (!isMovable()) return;

    if (d == m_dir) {
        m_force += force;
    } else if (d == -m_dir) {
        m_force -= force;
    } else if (force >= m_force) {
        m_dir = d;
        m_force = force;
    }

    if (m_force == 0) {
        m_dir = Dir::NONE;
    } else if (m_force < 0) {
        m_dir = -m_dir;
        m_force = -m_force;
    }
}

void Cell::confirmMove() {
    // m_pos += m_dir;
    m_dir = Dir::NONE;
    m_force = 0;
}

uint Cell::serialSize() const {
    return sizeof(Type) + sizeof(m_pos) + sizeof(m_dir) + sizeof(m_destroyFlag);
}

std::vector<char> Cell::serialize() const {
    std::vector<char> bytes(serialSize());
    char *data = bytes.data();

    copyIntoBytes(type(), &data);
    copyIntoBytes(m_pos.x, &data);
    copyIntoBytes(m_pos.y, &data);
    copyIntoBytes(m_dir, &data);
    copyIntoBytes(m_destroyFlag, &data);

    return bytes;
}

void Cell::deserializeFrom(const char **data) {
    copyFromBytes(m_pos.x, data);
    copyFromBytes(m_pos.y, data);
    copyFromBytes(m_dir, data);
    copyFromBytes(m_destroyFlag, data);
}

template <typename T>
std::unique_ptr<T> Cell::makeDeserialized(const char **data) {
    std::unique_ptr<T> cell = std::make_unique<T>();
    cell->deserializeFrom(data);
    return cell;
}

std::unique_ptr<Cell> Cell::deserialize(const char **data) {
    Type serialType;
    copyFromBytes(serialType, data);

    switch (serialType) {
    case Type::Empty: return makeDeserialized<EmptyCell>(data);
    case Type::Wire: return makeDeserialized<WireCell>(data);
    case Type::Signal: return makeDeserialized<SignalCell>(data);
    case Type::Data: return makeDeserialized<DataCell>(data);
    case Type::Belt: return makeDeserialized<BeltCell>(data);
    case Type::Clock: return makeDeserialized<ClockCell>(data);
    default: return nullptr;
    }
}

std::unique_ptr<Cell> Cell::deserialize(const std::vector<char> &bytes) {
    if (bytes.empty()) return nullptr;
    const char *data = bytes.data();
    return deserialize(&data);
}
