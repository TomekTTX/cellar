#ifndef BELTCELL_HPP
#define BELTCELL_HPP

#include "cell.hpp"

class BeltCell : public Cell {
private:
    static constexpr int M_ANIM_LENGTH = 3;
    int m_animationTick = 0;

public:
    Dir moveDir;

    BeltCell() = default;
    BeltCell(Vec pos, CellStackMatrix *env, Dir moveDir);

    inline Type type() const override { return Type::Belt; }
    inline bool isSolid() const override { return true; }

    void paint(QPainter &painter, const QRect &rect) const override;

    void preMove() override;
    void tick() override;
    bool receiveSignal() override;

    uint serialSize() const override;
    std::vector<char> serialize() const override;
    void deserializeFrom(const char **data) override;
};

#endif  // BELTCELL_HPP
