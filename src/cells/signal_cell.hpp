#ifndef SIGNALCELL_HPP
#define SIGNALCELL_HPP

#include "cell.hpp"

class SignalCell : public Cell {
private:
    Dir m_lastDir = Dir::NONE;
    bool m_moved = true;

public:
    SignalCell() = default;
    SignalCell(Vec pos, CellStackMatrix *env);
    SignalCell(Vec pos, CellStackMatrix *env, Dir orientation);

    inline Type type() const override { return Type::Signal; }

    void paintSelf(QPainter &painter, const QRect &rect) const override;

    void stageDirection() override;
    void tick() override;
    void confirmMove() override;

    uint serialSize() const override;
    std::vector<char> serialize() const override;
    void deserializeFrom(const char **data) override;

private:
    bool cellStackIsConductive() const;
};

#endif  // SIGNALCELL_HPP
