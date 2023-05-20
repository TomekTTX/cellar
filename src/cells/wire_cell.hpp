#ifndef WIRECELL_HPP
#define WIRECELL_HPP

#include "cell.hpp"

class WireCell : public Cell {
public:
    WireCell() = default;
    WireCell(Vec pos, CellStackMatrix *env);

    inline Type type() const override { return Type::Wire; }

    void paint(QPainter &painter, const QRect &rect) const override;

    inline bool isConductive() const override { return true; }
    bool receiveSignal() override;
};

#endif  // WIRECELL_HPP
