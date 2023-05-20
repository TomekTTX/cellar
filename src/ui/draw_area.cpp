#include "draw_area.hpp"

#include <memory>
#include <sstream>

#include <QDebug>
#include <QMouseEvent>

#include "cell_types.hpp"


DrawArea::DrawArea(QWidget *parent) :
    QWidget{ parent },
    m_mat(8, 8) {
    testInit();
}

void DrawArea::testInit() {
    for (int i = 2; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            if ((i == 2 || i == 3) && j == 1) continue;
            m_mat.emplaceCell<WireCell>({ i, j });
        }
    }
    m_mat.emplaceCell<WireCell>({ 3, 4 });
    m_mat.emplaceCell<WireCell>({ 3, 5 });
    m_mat.emplaceCell<DataCell>({ 3, 0 }, 0x00181818187E7E00);
    m_mat.emplaceCell<DataCell>({ 3, 1 }, Color(240, 170, 30));
    m_mat.emplaceCell<DataCell>({ 4, 2 }, Byte(170));
    m_mat.emplaceCell<DataCell>({ 5, 1 }, Empty());
    m_mat.emplaceCell<DataCell>({ 6, 0 }, "abcdef");
    m_mat.emplaceCell<ClockCell>({ 1, 0 }, 11);

    for (int i = 0; i < 8; ++i)
        m_mat.emplaceCell<BeltCell>({ i, 6 }, Dir::RIGHT);
}


void DrawArea::tick() {
    m_mat.tick();
    m_sv->repaint();
}

void DrawArea::mousePressEvent(QMouseEvent *event) {
    const auto point = event->position().toPoint();
    const Vec cellPos = m_mat.mapPosToCell(point.x(), point.y());

    if (event->button() == Qt::LeftButton) {
        // qDebug() << cellPos.x << cellPos.y;
        if (m_mat.posValid(cellPos)) {
            m_sv->setStack(m_mat[cellPos]);
        } else {
            m_sv->clear();
        }
    } else if (event->button() == Qt::RightButton) {
        if (m_mat.posValid(cellPos)) {
            std::vector<char> bytes;
            std::stringstream ss{};
            for (char c : (bytes = m_mat[cellPos].back()->serialize())) {
                uchar &uc = reinterpret_cast<uchar &>(c);
                ss << (uc <= 0xF ? "0" : "") << std::hex << ushort(uc) << ' ';
            }
            qDebug() << ss.str();

            // auto desCell = Cell::deserialize(bytes);
            // desCell->setPos(desCell->pos() + Dir::RIGHT);
            // m_mat.pushCell(std::move(desCell));
        }
    }
}

void DrawArea::paintEvent(QPaintEvent *event) {
    QPainter painter{ this };

    for (const auto &stack : m_mat)
        stack.back()->paint(painter);
}
