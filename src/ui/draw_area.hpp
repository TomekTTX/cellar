#ifndef DRAWAREA_HPP
#define DRAWAREA_HPP

#include <QObject>
#include <QWidget>

#include "cell_stack_matrix.hpp"
#include "stack_viewer.hpp"

class DrawArea : public QWidget {
    Q_OBJECT
private:
    CellStackMatrix m_mat;
    StackViewer *m_sv;

public:
    explicit DrawArea(QWidget *parent = nullptr);

    inline void setViewer(StackViewer *sv) { m_sv = sv; }
    void tick();

    void mousePressEvent(QMouseEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void testInit();
};

#endif  // DRAWAREA_HPP
