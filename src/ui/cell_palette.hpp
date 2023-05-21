#ifndef CELLPALETTE_HPP
#define CELLPALETTE_HPP

#include <vector>

#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "cell_types.hpp"

class CellPalette : public QWidget {
    Q_OBJECT
private:
    std::vector<std::unique_ptr<Cell>> m_cells;
    std::vector<std::vector<char>> m_serializedCells;
    std::vector<QRect> m_rects;
    int m_spacing, m_cellsPerRow = 0, m_cellsPerCol = 0;
    int m_selected = -1, m_page = 0;
    QLabel *m_pageLabel;
    QPushButton *m_prevBtn, *m_nextBtn;
    bool m_finalized = false;

public:
    explicit CellPalette(QWidget *parent = nullptr);

    int pageCount() const;
    std::unique_ptr<Cell> getCell() const;

    void mousePressEvent(QMouseEvent *event) override;

    void connectWidgets(QLabel *label, QPushButton *prev, QPushButton *next);
    void updatePagination();

public slots:
    void prevPage();
    void nextPage();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    template <typename T, typename... Args>
    inline void addCell(Args &&...args) {
        m_cells.push_back(std::make_unique<T>(Vec(), nullptr, args...));
        m_serializedCells.push_back(m_cells.back()->serialize());
    }

    int pointToIndex(QPoint p) const;
};

#endif  // CELLPALETTE_HPP
