#ifndef CELL_HPP
#define CELL_HPP

#include <QPainter>

#include "positions.hpp"

constexpr int CELL_SIZE = 40;

class CellStackMatrix;

class Cell {
private:
    // if this flag is set, the cell should not be accessed anymore
    // and should be destroyed after the current tick
    bool m_destroyFlag = false;
    // mutable bool m_noDrawOffsetFlag = false;

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

    // paint self using the given painter
    virtual void paint(QPainter &painter, const QRect &rect) const = 0;
    inline void paint(QPainter &painter) const { paint(painter, cellRect()); }

    // position setter
    inline void setPos(Vec newPos) { m_pos = newPos; }
    // environment setter
    inline void setEnv(CellStackMatrix *mat) { m_env = mat; }
    // events phase 1: choose move direction
    virtual inline void stageDirection() { m_dir = Dir::NONE; }
    // events phase 2: re-evaluate move direction of this and/or other cells
    virtual inline void preMove() {}
    // events phase 4: do various cell-specific actions
    virtual inline void tick() {}
    // event to run when a signal dies and activates the cell
    virtual inline bool receiveSignal() { return false; }

    void direct(Dir d);
    void destroySelf();
    virtual void confirmMove();

    virtual uint serialSize() const;
    virtual std::vector<char> serialize() const;
    virtual void deserializeFrom(const char **data);
    static std::unique_ptr<Cell> deserialize(const char **data);
    static std::unique_ptr<Cell> deserialize(const std::vector<char> &bytes);

    // serialization helper
    template <typename T>
    static void copyIntoBytes(const T &val, char **dst) {
        const char *data = reinterpret_cast<const char *>(&val);
        for (int i = 0; i < sizeof(T); ++i)
            *((*dst)++) = data[i];
    }

    // deserialization helper
    template <typename T>
    static void copyFromBytes(T &val, const char **src) {
        char *data = reinterpret_cast<char *>(&val);
        for (int i = 0; i < sizeof(T); ++i)
            data[i] = *((*src)++);
    }
};

#endif  // CELL_HPP
