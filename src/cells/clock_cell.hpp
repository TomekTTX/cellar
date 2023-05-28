#ifndef CLOCKCELL_HPP
#define CLOCKCELL_HPP

#include "cell.hpp"

class ClockCell : public Cell {
private:
    uint m_delay, m_ctr = 0;

public:
    ClockCell() = default;
    ClockCell(Vec pos, CellStackMatrix *env, uint delay = 8);

    inline Type type() const override { return Type::Clock; }
    inline bool isSolid() const override { return true; }

    void paint(QPainter &painter, const QRect &rect) const override;

    void tick() override;
    bool receiveSignal(Dir from = Dir::NONE) override;

    uint serialSize() const override;
    std::vector<char> serialize() const override;
    void deserializeFrom(const char **data) override;
};

#endif  // CLOCKCELL_HPP
