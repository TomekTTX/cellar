#include "cell_palette.hpp"

#include <QDebug>
#include <QMouseEvent>

CellPalette::CellPalette(QWidget *parent) :
    m_spacing(CELL_SIZE / 3),
    m_hspacing(m_spacing),
    m_vspacing(m_spacing),
    m_finalized(false),
    QWidget{ parent } {
    addCell<EmptyCell>();
    addCell<WireCell>();
    addCell<SignalCell>();
    addCell<DataCell>();
    addCell<ClockCell>(8);
    addCell<BeltCell>(Dir::LEFT);
}

int CellPalette::pageCount() const {
    const int pageSize = m_cellsPerCol * m_cellsPerRow;
    if (pageSize == 0) return 1;
    auto [pages, leftover] = std::div(m_cells.size(), pageSize);
    return std::max(1, pages + (leftover != 0));
}

std::unique_ptr<Cell> CellPalette::getCell() const {
    if (m_selected < 0 || m_selected >= m_cells.size()) return nullptr;
    return Cell::deserialize(m_serializedCells[m_selected]);
}

const Cell *CellPalette::getLocalCell() const {
    if (m_selected < 0 || m_selected >= m_cells.size()) return nullptr;
    return m_cells[m_selected].get();
}

void CellPalette::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_selected = pointToIndex(event->pos()) + m_cellsPerCol * m_cellsPerRow * m_page;
        emit cellSelected(const_cast<Cell *>(getLocalCell()));
    }
}

void CellPalette::connectWidgets(QLabel *label, QPushButton *prev, QPushButton *next, QFrame *frame) {
    m_pageLabel = label;
    m_prevBtn = prev;
    m_nextBtn = next;
    m_frame = frame;
}

void CellPalette::updatePagination() {
    const int pages = pageCount();

    m_prevBtn->setEnabled(m_page > 0);
    m_nextBtn->setEnabled(m_page < pages - 1);
    m_pageLabel->setText(QString::fromStdString(std::to_string(m_page + 1) + '/' + std::to_string(pages)));
}

void CellPalette::prevPage() {
    --m_page;
    updatePagination();
    repaint();
}

void CellPalette::nextPage() {
    ++m_page;
    updatePagination();
    repaint();
}

void CellPalette::paintEvent(QPaintEvent *event) {
    if (!m_finalized) return;
    const int pageSize = m_cellsPerCol * m_cellsPerRow;
    const int startInd = m_page * pageSize, stopInd = startInd + pageSize;
    QPainter painter{ this };
    int i = 0;
    while (i < pageSize && i + startInd < m_cells.size()) {
        m_cells[i + startInd]->paint(painter, m_rects[i]);
        ++i;
    }
    painter.setBrush(QColor(0, 0, 0, 0));
    painter.setPen(Qt::gray);
    while (i < pageSize) {
        painter.drawRect(m_rects[i++]);
    }
    //    painter.setPen(Qt::black);
    //    painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void CellPalette::resizeEvent(QResizeEvent *event) {
    // const int old_cpr = m_cellsPerRow, old_cpc = m_cellsPerCol;
    updateSpacing();
    // if (old_cpr == m_cellsPerRow && old_cpc == m_cellsPerCol) return;

    const QRect baseRect{ m_spacing, m_spacing, CELL_SIZE, CELL_SIZE };
    const int diffX = CELL_SIZE + m_hspacing;
    const int diffY = CELL_SIZE + m_vspacing;

    m_rects.clear();
    m_rects.reserve(m_cellsPerRow * m_cellsPerCol);
    for (int y = 0; y < m_cellsPerCol; ++y) {
        for (int x = 0; x < m_cellsPerRow; ++x) {
            m_rects.push_back(baseRect.adjusted(x * diffX, y * diffY, x * diffX, y * diffY));
        }
    }
    updatePagination();
    m_finalized = true;
}

int CellPalette::pointToIndex(QPoint p) const {
    auto it = std::find_if(m_rects.begin(), m_rects.end(), [p](const QRect &rect) { return rect.contains(p); });
    if (it == m_rects.end()) return std::numeric_limits<int>::min();
    return static_cast<int>(it - m_rects.begin());
}

void CellPalette::updateSpacing() {
    m_cellsPerRow = (width() - m_spacing) / (CELL_SIZE + m_spacing);
    m_cellsPerCol = (height() - m_spacing) / (CELL_SIZE + m_spacing);

    if (m_cellsPerRow > 1) {
        m_hspacing = (width() - 2 * m_spacing - CELL_SIZE * m_cellsPerRow) / (m_cellsPerRow - 1);
    } else {
        m_hspacing = 1;
    }

    if (m_cellsPerCol > 1) {
        m_vspacing = (height() - 2 * m_spacing - CELL_SIZE * m_cellsPerCol) / (m_cellsPerCol - 1);
    } else {
        m_vspacing = 1;
    }
}
