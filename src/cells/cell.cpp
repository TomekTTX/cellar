#include "cell.hpp"

#include "cell_stack_matrix.hpp"
#include "cell_types.hpp"

void Cell::destroySelf() {
    m_destroyFlag = true;  // m_env->extractCell(*this);
}

QRect Cell::cellRect() const {
    return { CELL_SIZE * m_pos.x, CELL_SIZE * m_pos.y, CELL_SIZE, CELL_SIZE };
}

void Cell::direct(Dir d) {
    if (isMovable()) m_dir = d;
}

void Cell::confirmMove() {
    // m_pos += m_dir;
    m_dir = Dir::NONE;
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
std::unique_ptr<T> makeDeserialized(const char *data) {
    std::unique_ptr<T> cell = std::make_unique<T>();
    cell->deserializeFrom(&data);
    return cell;
}

std::unique_ptr<Cell> Cell::deserialize(const std::vector<char> &bytes) {
    const char *data = bytes.data();
    Type serialType;
    copyFromBytes(serialType, &data);

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
