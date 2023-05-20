#ifndef EMPTYCELL_HPP
#define EMPTYCELL_HPP

#include "cell.hpp"

class EmptyCell : public Cell {
public:
    EmptyCell() = default;
    EmptyCell(Vec pos, CellStackMatrix *env);

    inline Type type() const override { return Type::Empty; }

    void paint(QPainter &painter, const QRect &rect) const override;

    // std::vector<char> serialize() const override;
    // void deserializeFrom(const char *data) override;
};

#endif  // EMPTYCELL_HPP
