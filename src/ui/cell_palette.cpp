#include "cell_palette.hpp"

#include <QDebug>
#include <QMouseEvent>

CellPalette::CellPalette(QWidget *parent) :
    m_spacing(CELL_SIZE / 3),
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

void CellPalette::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_selected = pointToIndex(event->pos());
    }
}

void CellPalette::connectWidgets(QLabel *label, QPushButton *prev, QPushButton *next) {
    m_pageLabel = label;
    m_prevBtn = prev;
    m_nextBtn = next;
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
}

void CellPalette::resizeEvent(QResizeEvent *event) {
    const int old_cpr = m_cellsPerRow, old_cpc = m_cellsPerCol;
    m_cellsPerRow = (width() - m_spacing) / (CELL_SIZE + m_spacing);
    m_cellsPerCol = (height() - m_spacing) / (CELL_SIZE + m_spacing);
    if (old_cpr == m_cellsPerRow && old_cpc == m_cellsPerCol) return;

    const QRect baseRect{ m_spacing, m_spacing, CELL_SIZE, CELL_SIZE };
    const int diff = CELL_SIZE + m_spacing;

    m_rects.clear();
    m_rects.reserve(m_cellsPerRow * m_cellsPerCol);
    for (int y = 0; y < m_cellsPerCol; ++y) {
        for (int x = 0; x < m_cellsPerRow; ++x) {
            m_rects.push_back(baseRect.adjusted(x * diff, y * diff, x * diff, y * diff));
        }
    }
    updatePagination();
    m_finalized = true;
}

int CellPalette::pointToIndex(QPoint p) const {
    auto it = std::find_if(m_rects.begin(), m_rects.end(), [p](const QRect &rect) { return rect.contains(p); });
    if (it == m_rects.end()) return -1;
    return static_cast<int>(it - m_rects.begin());
}
