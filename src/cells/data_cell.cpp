#include "data_cell.hpp"

#include <bitset>
#include <variant>

enum class DataType : int8_t { EMPTY, BYTE, COLOR, INTEGER, STRING, COUNT };

struct DataPainter {
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

struct DataSizeChecker {
    uint operator()(const Empty &) { return 0; }
    uint operator()(Byte b) { return 1; }
    uint operator()(Color c) { return 3; }
    uint operator()(int64_t x) { return sizeof(x); }
    uint operator()(const std::string &str) { return sizeof(uint) + static_cast<uint>(str.length()); }
};

struct DataSerializer {
    char **dst;

    void operator()(const Empty &) { copyIntoBytes(DataType::EMPTY, dst); }
    void operator()(Byte b) {
        const uint8_t v = b;
        copyIntoBytes(DataType::BYTE, dst);
        copyIntoBytes(v, dst);
    }
    void operator()(Color c) {
        const uint8_t v[] = { c.r, c.g, c.b };
        copyIntoBytes(DataType::COLOR, dst);
        copyIntoBytes(v[0], dst);
        copyIntoBytes(v[1], dst);
        copyIntoBytes(v[2], dst);
    }
    void operator()(int64_t x) {
        copyIntoBytes(DataType::INTEGER, dst);
        copyIntoBytes(x, dst);
    }
    void operator()(const std::string &str) {
        const uint len = static_cast<uint>(str.length());
        copyIntoBytes(DataType::STRING, dst);
        copyIntoBytes(len, dst);
        for (char c : str)
            copyIntoBytes(c, dst);
    }
};

DataCell::DataCell(Vec pos, CellStackMatrix *env, const CellData::Type &data) :
    Cell(pos, env) {
    m_data = CellData(data);
}

void DataCell::paintSelf(QPainter &painter, const QRect &rect) const {
    DataPainter dp{ rect, painter };
    std::visit(dp, m_data.val);
}

uint DataCell::serialSize() const {
    return Cell::serialSize() + sizeof(DataType) + std::visit(DataSizeChecker(), m_data.val);
}

std::vector<char> DataCell::serialize() const {
    auto bytes = Cell::serialize();
    char *data = bytes.data() + Cell::serialSize();
    DataSerializer ds{ &data };

    std::visit(ds, m_data.val);

    return bytes;
}

void DataCell::deserializeFrom(const char **data) {
    Cell::deserializeFrom(data);

    DataType type;
    copyFromBytes(type, data);

    switch (type) {
    case DataType::BYTE: {
        uint8_t v;
        copyFromBytes(v, data);
        m_data.val = Byte(v);
    } break;
    case DataType::COLOR: {
        uint8_t v[3];
        copyFromBytes(v[0], data);
        copyFromBytes(v[1], data);
        copyFromBytes(v[2], data);
        m_data.val = Color(v[0], v[1], v[2]);
    } break;
    case DataType::INTEGER: {
        int64_t x;
        copyFromBytes(x, data);
        m_data.val = x;
    } break;
    case DataType::STRING: {
        uint strLen;
        std::string str;
        copyFromBytes(strLen, data);
        str.resize(strLen);
        for (uint i = 0; i < strLen; ++i)
            copyFromBytes(str[i], data);
        m_data.val = str;
    } break;
    case DataType::EMPTY: m_data.val = Empty();
    default: break;
    }
}
