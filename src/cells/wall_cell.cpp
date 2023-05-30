#include "wall_cell.hpp"

WallCell::WallCell(Vec pos, CellStackMatrix *env) :
    Cell(pos, env) {}

void WallCell::paintSelf(QPainter &painter, const QRect &rect) const {
    painter.save();

    painter.setBrush(QColor(230, 30, 30));
    painter.drawRect(rect);
    painter.setClipRect(rect);
    painter.setPen(Qt::white);


    const QPoint origin = rect.topLeft();
    const int size = rect.width();
    const int interval = size / 4;
    for (int i = 1; i < 4; ++i)
        painter.drawLine(origin + QPoint(1, i * interval), origin + QPoint(size, i * interval));

    const QLine vline{ origin + QPoint(0, 1), origin + QPoint(0, size / 4) };
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 2; ++col) {
            painter.drawLine(vline.translated((1 + 2 * col + (row & 1)) * interval, row * interval));
        }
    }

    painter.restore();
}
