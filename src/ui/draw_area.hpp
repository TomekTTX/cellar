#ifndef DRAWAREA_HPP
#define DRAWAREA_HPP

#include <QObject>
#include <QWidget>

#include "cell_palette.hpp"
#include "cell_stack_matrix.hpp"
#include "stack_viewer.hpp"

class DrawArea : public QWidget {
    Q_OBJECT
private:
    CellStackMatrix m_mat;
    StackViewer *m_sv;
    CellPalette *m_palette;
    Cell *m_selectedCell = nullptr;
    Cell::Serialized m_cellCopy;

public:
    explicit DrawArea(QWidget *parent = nullptr);

    inline void setViewer(StackViewer *sv) { m_sv = sv; }
    inline void setPalette(CellPalette *p) { m_palette = p; }
    void tick();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

public slots:
    void select(Cell *cell);
    void deleteSelected();
    void moveSelectedUp();
    void moveSelectedDown();
    void cut();
    void copy();
    void paste();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void testInit();
};

#endif  // DRAWAREA_HPP
