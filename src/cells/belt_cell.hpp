#ifndef BELTCELL_HPP
#define BELTCELL_HPP

#include "cell.hpp"

class BeltCell : public Cell {
private:
    static constexpr int M_ANIM_LENGTH = 3;
    static constexpr int8_t M_BELT_FORCE = 2;
    int m_animationTick = 0;

public:
    Dir moveDir;

    BeltCell() = default;
    BeltCell(Vec pos, CellStackMatrix *env, Dir moveDir);

    inline Type type() const override { return Type::Belt; }
    inline bool isSolid() const override { return true; }

    void paintSelf(QPainter &painter, const QRect &rect) const override;

    void postStage() override;
    void preMove() override;
    void tick() override;
    bool receiveSignal(Dir from = Dir::NONE) override;

    uint serialSize() const override;
    std::vector<char> serialize() const override;
    void deserializeFrom(const char **data) override;
};

#endif  // BELTCELL_HPP
