#ifndef BELTCELL_HPP
#define BELTCELL_HPP

#include "cell.hpp"

class BeltCell : public Cell {
private:
    static constexpr int M_ANIM_LENGTH = 3;
    int m_animationTick = 0;

public:
    Dir moveDir;

    BeltCell(Vec pos, CellStackMatrix &env, Dir moveDir);

    inline bool isSolid() const override { return true; }

    void paint(QPainter &painter) const override;

    void preMove() override;
    void tick() override;
    bool receiveSignal() override;
};

#endif  // BELTCELL_HPP
