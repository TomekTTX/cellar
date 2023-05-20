#include "cell_stack_matrix.hpp"

#include <QDebug>

#include "empty_cell.hpp"

CellStackMatrix::CellStackMatrix(int64_t w, int64_t h) :
    m_w(w),
    m_h(h),
    m_data(w * h) {
    for (int y = 0; y < m_h; ++y)
        for (int x = 0; x < m_w; ++x)
            operator()(x, y).push_back(std::make_unique<EmptyCell>(Vec{ x, y }, *this));
}

CellStackMatrix::CellSeq CellStackMatrix::at(Vec pos) {
    CellSeq seq;

    if (posValid(pos)) {
        auto &stack = (*this)[pos];
        for (auto it = stack.rbegin(); it != stack.rend(); ++it)
            seq.push_back(**it);
    }

    return seq;
}

CellStackMatrix::CellSeq CellStackMatrix::around(Vec pos) {
    CellSeq seq;

    for (Dir d : DIRS) {
        auto stack = at(pos + d);
        std::copy(stack.begin(), stack.end(), std::back_inserter(seq));
    }

    return seq;
}

CellStackMatrix::CellSeq CellStackMatrix::scanline(Vec pos, Dir d) {
    CellSeq line;

    while (posValid(pos)) {
        auto seq = at(pos);
        std::copy(seq.begin(), seq.end(), std::back_inserter(line));
        pos += d;
    }

    return line;
}

const CellStackMatrix::DataType::value_type &CellStackMatrix::operator()(int64_t x, int64_t y) const {
    return m_data[x + m_w * y];
}

CellStackMatrix::DataType::value_type &CellStackMatrix::operator()(int64_t x, int64_t y) {
    return m_data[x + m_w * y];
}

const CellStackMatrix::DataType::value_type &CellStackMatrix::operator[](Vec v) const {
    return operator()(v.x, v.y);
}

CellStackMatrix::DataType::value_type &CellStackMatrix::operator[](Vec v) {
    return operator()(v.x, v.y);
}

bool CellStackMatrix::posValid(int64_t x, int64_t y) const {
    return x >= 0 && y >= 0 && x < m_w && y < m_h;
}

bool CellStackMatrix::posValid(Vec v) const {
    return posValid(v.x, v.y);
}

void CellStackMatrix::tick() {
    constexpr auto stageFunc = [](Cell &cell) { cell.stageDirection(); };
    constexpr auto preMoveFunc = [](Cell &cell) { cell.preMove(); };
    constexpr auto tickFunc = [](Cell &cell) { cell.tick(); };

    // Events phase 1 (stage)
    applyToAll(stageFunc);

    // Events phase 2 (pre-move)
    applyToAll(preMoveFunc);

    // Events phase 3 (move)
    while (performMove()) {
    }

    // Events phase 4 (tick)
    applyToAll(tickFunc);

    // Events phase 5 (destroy)
    for (ValueType &stack : *this) {
        for (int i = 0; i < stack.size(); ++i) {
            if (stack[i]->toBeDestroyed()) {
                extractCell(stack[i--]);
            }
        }
    }
}

std::unique_ptr<Cell> CellStackMatrix::extractCell(Cell &cell) {
    ValueType &stack = (*this)[cell.pos()];

    auto it = std::find_if(stack.begin(), stack.end(), [&cell](auto &ptr) { return ptr.get() == &cell; });
    if (it == stack.end()) return nullptr;

    std::unique_ptr<Cell> extractedCell = std::move(*it);
    stack.erase(it);
    return extractedCell;
}

std::unique_ptr<Cell> CellStackMatrix::extractCell(std::unique_ptr<Cell> &cell) {
    return extractCell(*cell);
}

bool CellStackMatrix::moveCell(Cell &cell, Vec targetPos) {
    auto cellPtr = extractCell(cell);
    if (!cellPtr) return false;

    cellPtr->setPos(targetPos);
    (*this)[targetPos].push_back(std::move(cellPtr));
    (*this)[targetPos].back()->confirmMove();
    return true;
}

bool CellStackMatrix::moveCell(std::unique_ptr<Cell> &cell, Vec targetPos) {
    return moveCell(*cell, targetPos);
}

int CellStackMatrix::performMove() {
    int moveCtr = 0;

    for (DataType::value_type &stack : *this) {
        for (std::size_t i = 0; i < stack.size(); ++i) {
            std::unique_ptr<Cell> &cell = stack[i];
            Vec newPos = cell->targetPos();

            if (!cell->isMoving() || !posValid(newPos)) continue;
            if (cell->isSolid() && seqHasSolid(at(newPos))) continue;

            moveCell(cell, newPos);
            ++moveCtr;
            --i;
        }
    }

    return moveCtr;
}

void CellStackMatrix::applyToAll(const std::function<void(Cell &)> &func) {
    for (auto &stack : *this)
        for (auto &cell : stack)
            func(*cell);
}

bool CellStackMatrix::seqHasSolid(const CellSeq &seq) {
    for (Cell &cell : seq)
        if (cell.isSolid()) return true;
    return false;
}

Vec CellStackMatrix::mapPosToCell(int x, int y) {
    return { x / CELL_SIZE, y / CELL_SIZE };
}
