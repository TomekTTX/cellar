#ifndef DATACELL_HPP
#define DATACELL_HPP

#include "cell.hpp"
#include "cell_data.hpp"

class DataCell : public Cell {
private:
    CellData m_data;

public:
    DataCell(Vec pos, CellStackMatrix *env, const CellData::Type &data = Empty());

    inline bool isSolid() const override { return true; }
    inline bool isMovable() const override { return true; }

    inline void stageDirection() override { direct(Dir::DOWN); }

    void paint(QPainter &painter, const QRect &rect) const override;
};

#endif  // DATACELL_HPP
