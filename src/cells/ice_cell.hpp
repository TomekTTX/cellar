#ifndef ICECELL_HPP
#define ICECELL_HPP

#include "cell.hpp"

class IceCell : public Cell {
private:
    bool m_skipPreMove = false, m_forceApplied = false;

public:
    IceCell() = default;
    IceCell(Vec pos, CellStackMatrix *env);

    inline Type type() const override { return Type::Ice; }
    inline bool isSolid() const override { return true; }

    void preMove() override;
    void tick() override;
    void applyForces();

protected:
    void paintSelf(QPainter &painter, const QRect &rect) const override;

private:
    std::vector<IceCell *> iceSeq();
};

#endif  // ICECELL_HPP
