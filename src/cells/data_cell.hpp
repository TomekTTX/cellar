#ifndef DATACELL_HPP
#define DATACELL_HPP

#include "cell.hpp"
#include "cell_data.hpp"

class DataCell : public Cell {
private:
    CellData m_data;

public:
    DataCell() = default;
    DataCell(Vec pos, CellStackMatrix *env, const CellData::Type &data = Empty());

    inline Type type() const override { return Type::Data; }
    inline bool isSolid() const override { return true; }
    inline bool isMovable() const override { return true; }

    inline void stageDirection() override { direct(Dir::DOWN); }

    void paint(QPainter &painter, const QRect &rect) const override;

    uint serialSize() const override;
    std::vector<char> serialize() const override;
    void deserializeFrom(const char **data) override;
};

#endif  // DATACELL_HPP
