#ifndef CELLITEM_HPP
#define CELLITEM_HPP

#include <QGraphicsItem>
#include <QObject>

#include "cell.hpp"

class CellItem : public QGraphicsItem {
private:
    Cell &m_cell;

public:
    enum { Type = UserType + 1 };

    CellItem(Cell &cell);

    inline int type() const override { return Type; }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif  // CELLITEM_HPP
