#ifndef CELLPALETTE_HPP
#define CELLPALETTE_HPP

#include <vector>

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "cell.hpp"
#include "cell_viewer.hpp"

class CellPalette : public QWidget {
    Q_OBJECT
private:
    static constexpr int MAX_CELL_COUNT = 10;

    std::array<CellViewer *, MAX_CELL_COUNT * MAX_CELL_COUNT> m_viewers;
    std::vector<std::unique_ptr<Cell>> m_cells;
    std::vector<std::vector<char>> m_serializedCells;
    int m_spacing = 10, m_cellsPerRow = -1, m_cellsPerCol = -1;
    int m_selectionIndex = -1, m_page = 0;
    Cell *m_selected;
    QGridLayout *m_layout;
    QFrame *m_frame;
    QLabel *m_pageLabel;
    QPushButton *m_prevBtn, *m_nextBtn;

public:
    explicit CellPalette(QWidget *parent = nullptr);

    inline int pageSize() const { return m_cellsPerCol * m_cellsPerRow; }
    int pageCount() const;
    std::unique_ptr<Cell> getCell() const;

    void mousePressEvent(QMouseEvent *event) override;

    void connectWidgets(QLabel *label, QPushButton *prev, QPushButton *next, QFrame *frame);
    void updatePagination();

public slots:
    void prevPage();
    void nextPage();
    void select(Cell *cell);

signals:
    void cellSelected(Cell *cell);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    template <typename T, typename... Args>
    inline void addCell(Args &&...args) {
        m_cells.push_back(std::make_unique<T>(Vec(), nullptr, args...));
        m_serializedCells.push_back(m_cells.back()->serialize());
    }
    void updateSpacing();
};

#endif  // CELLPALETTE_HPP
