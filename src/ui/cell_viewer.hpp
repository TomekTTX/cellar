#ifndef CELLVIEWER_HPP
#define CELLVIEWER_HPP

#include <functional>

#include <QWidget>

#include "cell.hpp"

class CellViewer : public QWidget {
    Q_OBJECT
public:
    using OnClickFunc = std::function<void(QMouseEvent *)>;

private:
    const Cell *m_cell = nullptr;
    QRect m_drawRect;

public:
    explicit CellViewer(QWidget *parent = nullptr);

    inline const Cell *getCell() { return m_cell; }

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(Cell *cell);

public slots:
    inline void showCell(Cell *cell) {
        m_cell = cell;
        repaint();
    }

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif  // CELLVIEWER_HPP
