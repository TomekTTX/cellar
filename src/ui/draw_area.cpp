#include "draw_area.hpp"

#include <memory>

#include "cell_types.hpp"

DrawArea::DrawArea(QWidget *parent) :
    QWidget{ parent },
    m_mat(8, 8) {
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ((i == 2 || i == 3) && j == 1) continue;
            m_mat.pushCell<WireCell>({ i, j });
        }
    }
    m_mat.pushCell<WireCell>({ 3, 4 });
    m_mat.pushCell<WireCell>({ 3, 5 });
    m_mat.pushCell<DataCell>({ 3, 0 }, 0x00181818187E7E00);
    m_mat.pushCell<ClockCell>({ 1, 0 }, 11);

    for (int i = 0; i < 8; ++i)
        m_mat.pushCell<BeltCell>({ i, 6 }, Dir::RIGHT);
}

void DrawArea::paintEvent(QPaintEvent *event) {
    QPainter painter{ this };

    for (const auto &stack : m_mat)
        stack.back()->paint(painter);
}
