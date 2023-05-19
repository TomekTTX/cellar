#include "belt_cell.hpp"

#include "cell_stack_matrix.hpp"

BeltCell::BeltCell(Vec pos, CellStackMatrix &env, Dir moveDir) :
    Cell(pos, env),
    moveDir(moveDir) {}

void BeltCell::paint(QPainter &painter) const {
    QRect rect = cellRect();
    QRect sliceRect = { rect.x(), rect.y() + CELL_SIZE / 8, CELL_SIZE / 16, rect.height() - CELL_SIZE / 4 };

    painter.setBrush(QColor(70, 70, 70));
    painter.drawRect(rect);

    painter.save();
    painter.setClipRect(rect.adjusted(1, 1, -2, -2));
    painter.setBrush(QColor(130, 130, 130));
    painter.setPen(QColor(130, 130, 130));

    const int tickMult = sliceRect.width() * (moveDir == Dir::RIGHT ? 1 : -1);
    const int tickOffset = m_animationTick * tickMult;
    const int iMult = M_ANIM_LENGTH * sliceRect.width();
    for (int i = -1; i < 8; ++i) {
        const int xOff = tickOffset + i * iMult;
        painter.drawRect(sliceRect.adjusted(xOff, 0, xOff, 0));
        // painter.drawRect(rect.x() + tickMult * m_animationTick + 6 * i, rect.y() + 4, 2, rect.height() - 8);
    }

    painter.restore();
}

void BeltCell::preMove() {
    for (Cell &cell : m_env.at(pos() + Dir::UP))
        cell.direct(moveDir);
}

void BeltCell::tick() {
    m_animationTick = (m_animationTick + 1) % M_ANIM_LENGTH;
}

bool invertStackDir(CellStackMatrix::ValueType &stack) {
    int ctr = 0;

    for (auto &cell : stack) {
        if (BeltCell *bcell = dynamic_cast<BeltCell *>(cell.get())) {
            bcell->moveDir = -bcell->moveDir;
            ++ctr;
        }
    }

    return ctr != 0;
}

bool BeltCell::receiveSignal() {
    const int y = pos().y;

    moveDir = -moveDir;
    for (int i = pos().x - 1; m_env.posValid(i, y) && invertStackDir(m_env(i, y)); --i)
        ;
    for (int i = pos().x + 1; m_env.posValid(i, y) && invertStackDir(m_env(i, y)); ++i)
        ;

    return true;
}
