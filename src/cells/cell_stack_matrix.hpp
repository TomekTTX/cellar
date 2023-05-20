#ifndef CELLSTACKMATRIX_HPP
#define CELLSTACKMATRIX_HPP

#include <cstdint>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "cell.hpp"
#include "positions.hpp"

// stores a matrix of "stacks" (actually vectors) of cells
class CellStackMatrix {
public:
    using DataType = std::vector<std::vector<std::unique_ptr<Cell>>>;
    using ValueType = DataType::value_type;
    using CellRef = std::reference_wrapper<Cell>;
    using CellSeq = std::vector<CellRef>;

private:
    DataType m_data;
    int64_t m_w, m_h;

public:
    CellStackMatrix(int64_t w, int64_t h);

    // iterator funcs
    inline auto begin() { return m_data.begin(); }
    inline auto end() { return m_data.end(); }
    inline auto cbegin() const { return m_data.cbegin(); }
    inline auto cend() const { return m_data.cend(); }

    // returns a sequence of cells from the stack at pos
    CellSeq at(Vec pos);
    // returns a sequence of cells from the stacks around a pos
    CellSeq around(Vec pos);
    // returns a sequence of cells from the stack at pos and following
    // stacks in the direction d until the border of the matrix is hit
    CellSeq scanline(Vec pos, Dir d);

    // returns reference to stack at (x, y)
    const ValueType &operator()(int64_t x, int64_t y) const;
    ValueType &operator()(int64_t x, int64_t y);

    // returns reference to stack at (v.x, v.y)
    const ValueType &operator[](Vec v) const;
    ValueType &operator[](Vec v);

    // checks if position (x, y) exists in the matrix
    bool posValid(int64_t x, int64_t y) const;
    bool posValid(Vec v) const;

    static Vec mapPosToCell(int x, int y);

    // fires all events on cells
    void tick();

    // constructs and places a cell in the matrix
    template <typename T, typename... Args>
    inline void pushCell(Vec pos, Args &&...args) {
        (*this)[pos].push_back(std::make_unique<T>(pos, this, args...));
    }

    // finds and detaches the given cell from the matrix
    std::unique_ptr<Cell> extractCell(Cell &cell);
    std::unique_ptr<Cell> extractCell(std::unique_ptr<Cell> &cell);

    // finds the given cell and moves it to targetPos
    bool moveCell(Cell &cell, Vec targetPos);
    bool moveCell(std::unique_ptr<Cell> &cell, Vec targetPos);

private:
    int performMove();

    void applyToAll(const std::function<void(Cell &)> &func);
    static bool seqHasSolid(const CellSeq &seq);
};

#endif  // CELLSTACKMATRIX_HPP
