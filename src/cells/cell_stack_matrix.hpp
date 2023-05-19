#ifndef CELLSTACKMATRIX_HPP
#define CELLSTACKMATRIX_HPP

#include <concepts>
#include <cstdint>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "cell.hpp"
#include "positions.hpp"

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

    inline auto begin() { return m_data.begin(); }
    inline auto end() { return m_data.end(); }
    inline auto cbegin() const { return m_data.cbegin(); }
    inline auto cend() const { return m_data.cend(); }

    CellSeq at(Vec pos);
    CellSeq around(Vec pos);
    CellSeq scanline(Vec pos, Dir d);

    const ValueType &operator()(int64_t x, int64_t y) const;
    ValueType &operator()(int64_t x, int64_t y);

    const ValueType &operator[](Vec v) const;
    ValueType &operator[](Vec v);

    bool posValid(int64_t x, int64_t y) const;
    bool posValid(Vec v) const;

    void tick();

    template <typename T, typename... Args>
    inline void pushCell(Vec pos, Args &&...args) {
        (*this)[pos].push_back(std::make_unique<T>(pos, *this, args...));
    }

    std::unique_ptr<Cell> extractCell(Cell &cell);
    std::unique_ptr<Cell> extractCell(std::unique_ptr<Cell> &cell);
    bool moveCell(Cell &cell, Vec targetPos);
    bool moveCell(std::unique_ptr<Cell> &cell, Vec targetPos);


private:
    int performMove();
    bool pushDetachedCell(ValueType::iterator &it, ValueType &source, ValueType &target);

    void applyToAll(const std::function<void(Cell &)> &func);
    static bool seqHasSolid(const CellSeq &seq);
};

/*

struct Iter {
    using iterator_concept [[maybe_unused]] = std::forward_iterator_tag;
    using value_type = Cell &;
    using difference_type = std::ptrdiff_t;

    Iter() = default;
    Iter(CellStackMatrix *mat, Vec pos, Dir dir) :
        m_mat(mat),
        m_pos(pos),
        m_dir(dir) {}
    Iter &operator++() {
        m_pos += m_dir;
        return *this;
    }
    Iter operator++(int) {
        Iter tmp = *this;
        operator++();
        return tmp;
    }
    value_type operator*() const { return (*m_mat)[m_pos]; }
    value_type operator->() { return *m_ptr; }
    bool valid() const { return m_mat->posValid(m_pos); }

private:
    CellStackMatrix *m_mat;
    Vec m_pos;
    Dir m_dir;
};
*/

#endif  // CELLSTACKMATRIX_HPP
