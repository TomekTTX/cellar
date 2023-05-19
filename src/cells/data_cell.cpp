#include "data_cell.hpp"

#include <bitset>
#include <variant>

struct DataPaintStruct {
    QRect rect;
    QPainter &painter;

    void placeholder() {
        painter.setBrush(Qt::gray);
        painter.drawRect(rect);
    }

    void operator()(const Empty &) {
        QRect smallRect{ rect.x() + 5, rect.y() + 5, rect.width() - 10, rect.height() - 10 };
        painter.setBrush(QColor(50, 50, 50));
        painter.drawRect(rect);
        painter.setBrush(QColor(150, 150, 150));
        painter.drawRect(smallRect);
    }

    void operator()(Byte b) {
        painter.setBrush(QColor(b, b, b));
        painter.drawRect(rect);
    }

    void operator()(Color c) {
        painter.setBrush(QColor(c.r, c.g, c.b));
        painter.drawRect(rect);
    }

    void operator()(int64_t x) {
        const QRect br{ rect.x(), rect.y(), rect.width() / 8, rect.height() / 8 };

        for (int i = 0; i < 64; ++i) {
            bool bit = (x >> (63 - i)) & 1;
            painter.setBrush(bit ? Qt::white : QColor(40, 40, 40));
            painter.drawRect(br.x() + (i % 8) * br.width(), br.y() + (i / 8) * br.height(), br.width(), br.height());
        }
    }

    void operator()(const std::string &str) {
        QRect inner{ rect.x() + 3, rect.y() + 3, rect.width() - 6, rect.height() - 6 };

        if (str.length() == 0)
            painter.setBrush(QColor(200, 200, 200));
        else if (str.length() <= 3)
            painter.setBrush(QColor(255, 255, 190));
        else
            painter.setBrush(QColor(255, 190, 190));
        painter.drawRect(rect);
        painter.setBrush(Qt::white);
        painter.drawRect(inner);
        painter.setBrush(Qt::black);

        painter.drawText(rect, Qt::AlignCenter | Qt::AlignHCenter, QString::fromStdString(str.substr(0, 3)));
    }
};

DataCell::DataCell(Vec pos, CellStackMatrix &env, const CellData::Type &data) :
    Cell(pos, env) {
    m_data = CellData(data);
}

void DataCell::paint(QPainter &painter) const {
    DataPaintStruct dps{ cellRect(), painter };
    std::visit(dps, m_data.val);
}
