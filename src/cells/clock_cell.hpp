#ifndef CLOCKCELL_HPP
#define CLOCKCELL_HPP

#include "cell.hpp"

class ClockCell : public Cell {
private:
    uint m_delay, m_ctr = 0;

public:
    ClockCell(Vec pos, CellStackMatrix *env, uint delay = 8);

    inline bool isSolid() const override { return true; }

    void paint(QPainter &painter, const QRect &rect) const override;

    void tick() override;
    bool receiveSignal() override;
};

#endif  // CLOCKCELL_HPP
