#ifndef CELLVIEWER_HPP
#define CELLVIEWER_HPP

#include <QWidget>

#include "cell.hpp"

class CellViewer : public QWidget {
    Q_OBJECT
private:
    const Cell *m_cell = nullptr;
    QRect m_drawRect;

public:
    explicit CellViewer(QWidget *parent = nullptr);

public slots:
    inline void showCell(Cell *cell) {
        m_cell = cell;
        repaint();
    }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif  // CELLVIEWER_HPP
