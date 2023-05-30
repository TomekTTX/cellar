#ifndef WALLCELL_HPP
#define WALLCELL_HPP

#include "cell.hpp"

class WallCell : public Cell {
public:
    WallCell() = default;
    WallCell(Vec pos, CellStackMatrix *env);

    inline Type type() const override { return Type::Wall; }

    void paintSelf(QPainter &painter, const QRect &rect) const override;

    inline bool isSolid() const override { return true; }
};

#endif  // WALLCELL_HPP
