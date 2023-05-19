#ifndef DRAWAREA_HPP
#define DRAWAREA_HPP

#include <QObject>
#include <QWidget>

#include "cell_stack_matrix.hpp"

class DrawArea : public QWidget {
    Q_OBJECT
private:
    CellStackMatrix m_mat;

public:
    explicit DrawArea(QWidget *parent = nullptr);

    inline void tick() { m_mat.tick(); }

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif  // DRAWAREA_HPP
