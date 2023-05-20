#ifndef EMPTYCELL_HPP
#define EMPTYCELL_HPP

#include "cell.hpp"

class EmptyCell : public Cell {
public:
    EmptyCell(Vec pos, CellStackMatrix &env);

    void paint(QPainter &painter, const QRect &rect) const override;
};

#endif  // EMPTYCELL_HPP
