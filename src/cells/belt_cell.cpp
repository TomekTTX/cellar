#include "belt_cell.hpp"

#include "cell_stack_matrix.hpp"
#include "util_funcs.hpp"

BeltCell::BeltCell(Vec pos, CellStackMatrix *env, Dir moveDir) :
    Cell(pos, env),
    moveDir(moveDir) {}

void BeltCell::paintSelf(QPainter &painter, const QRect &rect) const {
    QRect sliceRect = { rect.x(), rect.y() + CELL_SIZE / 8, CELL_SIZE / 16, rect.height() - CELL_SIZE / 4 };

    painter.setBrush(QColor(70, 70, 70));
    painter.drawRect(rect);

    painter.save();
    painter.setClipRect(rect.adjusted(1, 1, -1, -1));
    painter.setBrush(QColor(130, 130, 130));
    painter.setPen(QColor(130, 130, 130));

    const int tickMult = sliceRect.width() * (moveDir == Dir::RIGHT ? 1 : -1);
    const int tickOffset = m_animationTick * tickMult;
    const int iMult = M_ANIM_LENGTH * sliceRect.width();
    for (int i = -2; i < 10; ++i) {
        const int xOff = tickOffset + i * iMult;
        painter.drawRect(sliceRect.adjusted(xOff, 0, xOff, 0));
    }

    painter.restore();
}

void BeltCell::postStage() {
    for (Cell &cell : m_env->at(pos() + Dir::UP))
        cell.direct(moveDir, M_BELT_FORCE);
}

void BeltCell::preMove() {
    for (Cell &cell : m_env->at(pos() + Dir::DOWN)) {
        if (cell.dir() == Dir::UP) {
            cell.direct(moveDir, M_BELT_FORCE);
        }
    }
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

bool BeltCell::receiveSignal(Dir from) {
    const int y = pos().y;

    moveDir = -moveDir;
    for (int i = pos().x - 1; m_env->posValid(i, y) && invertStackDir((*m_env)(i, y)); --i)
        ;
    for (int i = pos().x + 1; m_env->posValid(i, y) && invertStackDir((*m_env)(i, y)); ++i)
        ;

    return true;
}

uint BeltCell::serialSize() const {
    return Cell::serialSize() + sizeof(m_animationTick) + sizeof(moveDir);
}

std::vector<char> BeltCell::serialize() const {
    auto bytes = Cell::serialize();
    char *data = bytes.data() + Cell::serialSize();

    copyIntoBytes(m_animationTick, &data);
    copyIntoBytes(moveDir, &data);

    return bytes;
}

void BeltCell::deserializeFrom(const char **data) {
    Cell::deserializeFrom(data);
    copyFromBytes(m_animationTick, data);
    copyFromBytes(moveDir, data);
}
