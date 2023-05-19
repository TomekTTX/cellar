#ifndef SIGNALCELL_HPP
#define SIGNALCELL_HPP

#include "cell.hpp"

class SignalCell : public Cell {
private:
    Dir m_lastDir = Dir::NONE;
    bool m_moved = true;

public:
    SignalCell(Vec pos, CellStackMatrix &env);

    void paint(QPainter &painter) const override;

    void stageDirection() override;
    void tick() override;
    void confirmMove() override;

private:
    bool cellStackIsConductive() const;
};

#endif  // SIGNALCELL_HPP
