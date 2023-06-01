#ifndef FANCELL_HPP
#define FANCELL_HPP

#include "cell.hpp"

class FanCell : public Cell {
private:
    constexpr static int8_t ANIM_LENGTH = 10;

    bool m_active = true;
    Dir m_facing;
    int8_t m_force;
    int8_t m_animationTick = ANIM_LENGTH / 3;
    int16_t m_reach;

public:
    FanCell() = default;
    FanCell(Vec pos, CellStackMatrix *env, Dir facing = Dir::UP, int8_t force = 1, int16_t reach = 4);

    inline Type type() const override { return Type::Fan; }
    inline bool isSolid() const override { return true; }

    void postStage() override;
    bool receiveSignal(Dir from = Dir::NONE) override;

    uint serialSize() const override;
    std::vector<char> serialize() const override;
    void deserializeFrom(const char **data) override;

protected:
    void paintSelf(QPainter &painter, const QRect &rect) const override;
};

#endif  // FANCELL_HPP
