#ifndef CELL_HPP
#define CELL_HPP

#include <QPainter>

#include "positions.hpp"
#include "util_funcs.hpp"

constexpr int CELL_SIZE = 40;

class CellStackMatrix;

class Cell {
private:
    // if this flag is set, the cell should not be accessed anymore
    // and should be destroyed after the current tick
    bool m_destroyFlag = false;
    // flag for showing the selection mark on the cell
    bool m_selected = false;
    // used for calculating the sum of all forces that affect a movable cell
    int16_t m_force = 0;

protected:
    // cell position
    Vec m_pos;
    // pointer to matrix the cell is placed in
    CellStackMatrix *m_env;
    // direction the cell is planning to move
    Dir m_dir = Dir::NONE;

public:
    enum class Type : int16_t { Empty, Wire, Signal, Data, Belt, Clock };

    Cell() = default;
    Cell(Vec pos, CellStackMatrix *env) :
        m_pos(pos),
        m_env(env) {}

    virtual Type type() const = 0;  // inline Type type() const override { return Type::; }
    inline Vec pos() const { return m_pos; }
    inline Dir dir() const { return m_dir; }
    inline Vec targetPos() const { return pos() + dir(); }
    QRect cellRect() const;

    // check if the cell acts solid to other cells
    virtual inline bool isSolid() const { return false; }
    // check if the cell can be moved by gravity and/or other cells
    virtual inline bool isMovable() const { return false; }
    // check if the cell can carry signal cells
    virtual inline bool isConductive() const { return false; }
    // check if the cell is planning to move
    inline bool isMoving() const { return m_dir != Dir::NONE; }
    // check if the cell should be removed in events phase 5
    inline bool toBeDestroyed() const { return m_destroyFlag; }

    void paint(QPainter &painter) const;
    void paint(QPainter &painter, const QRect &rect) const;

    // position setter
    inline void setPos(Vec newPos) { m_pos = newPos; }
    // environment setter
    inline void setEnv(CellStackMatrix *mat) { m_env = mat; }
    // selected setter
    inline void setSelected(bool v) { m_selected = v; }
    // events phase 1: choose move direction
    virtual inline void stageDirection() { m_dir = Dir::NONE; }
    // events phase 2: re-evaluate move direction of this and/or other cells
    virtual inline void preMove() {}
    // events phase 4: do various cell-specific actions
    virtual inline void tick() {}
    // event to run when a signal dies and activates the cell
    virtual inline bool receiveSignal(Dir from = Dir::NONE) { return false; }

    void direct(Dir d, int8_t force = 1);
    void destroySelf();
    virtual void confirmMove();

    virtual uint serialSize() const;
    virtual std::vector<char> serialize() const;
    virtual void deserializeFrom(const char **data);
    static std::unique_ptr<Cell> deserialize(const char **data);
    static std::unique_ptr<Cell> deserialize(const std::vector<char> &bytes);

protected:
    virtual void paintSelf(QPainter &painter, const QRect &rect) const = 0;
};

#endif  // CELL_HPP
