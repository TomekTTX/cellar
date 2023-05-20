#ifndef STACKVIEWER_HPP
#define STACKVIEWER_HPP

#include <QLabel>
#include <QWidget>

#include "cell_stack_matrix.hpp"

class StackViewer : public QWidget {
    Q_OBJECT
private:
    const CellStackMatrix::ValueType *m_cells = nullptr;
    QLabel *m_label;

public:
    explicit StackViewer(QWidget *parent = nullptr);

    void clear();
    void setStack(const CellStackMatrix::ValueType &stack);
    inline void setLabel(QLabel *label) { m_label = label; }

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif  // STACKVIEWER_HPP
