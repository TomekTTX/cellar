#ifndef CELL_HPP
#define CELL_HPP

#include <QPainter>

#include "draw_helpers.hpp"
#include "positions.hpp"

constexpr int CELL_SIZE = 40;

class CellStackMatrix;

class Cell {
private:
    bool m_destroyFlag = false;

protected:
    Vec m_pos;
    CellStackMatrix &m_env;
    Dir m_dir = Dir::NONE;

public:
    Cell(Vec pos, CellStackMatrix &env) :
        m_pos(pos),
        m_env(env) {}

    inline Vec pos() const { return m_pos; }
    inline Dir dir() const { return m_dir; }
    inline Vec targetPos() const { return pos() + dir(); }
    inline QRect cellRect() const { return { CELL_SIZE * m_pos.x, CELL_SIZE * m_pos.y, CELL_SIZE, CELL_SIZE }; }

    virtual inline bool isSolid() const { return false; }
    virtual inline bool isMovable() const { return false; }
    virtual inline bool isConductive() const { return false; }
    inline bool isMoving() const { return m_dir != Dir::NONE; }
    inline bool toBeDestroyed() const { return m_destroyFlag; }

    virtual void paint(QPainter &painter) const = 0;

    inline void setPos(Vec newPos) { m_pos = newPos; }
    virtual inline void stageDirection() { m_dir = Dir::NONE; }
    virtual inline void preMove() {}
    virtual inline void tick() {}
    virtual inline bool receiveSignal() { return false; }

    void direct(Dir d);
    void destroySelf();
    virtual void confirmMove();
};

#endif  // CELL_HPP
