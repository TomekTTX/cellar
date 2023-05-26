#include "cell_palette.hpp"

#include <QDebug>
#include <QMouseEvent>

#include "cell_types.hpp"

CellPalette::CellPalette(QWidget *parent) :
    m_spacing(CELL_SIZE / 3),
    m_finalized(false),
    QWidget{ parent } {
    setLayout(m_layout = new QGridLayout(this));
    setMinimumSize(CELL_SIZE + 2 * m_spacing + 1, CELL_SIZE + 2 * m_spacing + 1);

    for (auto &viewer : m_viewers) {
        viewer = new CellViewer(this);
        viewer->showCell(nullptr);
    }

    for (int i = 0; i < MAX_CELL_COUNT; ++i) {
        for (int j = 0; j < MAX_CELL_COUNT; ++j) {
            const int arrIndex = MAX_CELL_COUNT * i + j;
            m_viewers[arrIndex]->hide();
            m_layout->addWidget(m_viewers[arrIndex], i, j);
            // clang-format off
            connect(m_viewers[arrIndex], SIGNAL(clicked(Cell*)), SLOT(select(Cell*)));
            // clang-format on
        }
    }

    addCell<EmptyCell>();
    addCell<WireCell>();
    addCell<SignalCell>();
    addCell<DataCell>();
    addCell<ClockCell>(8);
    addCell<BeltCell>(Dir::LEFT);
}

int CellPalette::pageCount() const {
    if (pageSize() == 0) return 1;
    auto [pages, leftover] = std::div(m_cells.size(), pageSize());
    return std::max(1, pages + (leftover != 0));
}

std::unique_ptr<Cell> CellPalette::getCell() const {
    if (!m_selected || m_selectionIndex < 0 || m_selectionIndex >= m_cells.size()) return nullptr;
    return Cell::deserialize(m_serializedCells[m_selectionIndex]);
}

void CellPalette::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        select(nullptr);
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
    const int ps = pageSize();
    const int startInd = m_page * ps;

    m_prevBtn->setEnabled(m_page > 0);
    m_nextBtn->setEnabled(m_page < pages - 1);
    m_pageLabel->setText(QString::fromStdString(std::to_string(m_page + 1) + '/' + std::to_string(pages)));

    int cellInd = 0, viewerInd = -1;
    while (++viewerInd < m_viewers.size() && cellInd < ps && cellInd + startInd < m_cells.size()) {
        if (!m_viewers[viewerInd]->isVisible()) continue;
        m_viewers[viewerInd]->showCell(m_cells[cellInd + startInd].get());
        ++cellInd;
    }

    while (viewerInd < MAX_CELL_COUNT) {
        m_viewers[viewerInd++]->showCell(nullptr);
    }
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

void CellPalette::select(Cell *cell) {
    auto it =
        std::find_if(m_viewers.begin(), m_viewers.end(), [cell](auto &viewer) { return viewer->getCell() == cell; });
    if (it == m_viewers.end()) {
        m_selected = nullptr;
        m_selectionIndex = -1;
    } else {
        m_selected = cell;
        m_selectionIndex = static_cast<int>(it - m_viewers.begin()) + m_page * pageSize();
    }
    emit cellSelected(m_selected);
}

// TODO this still produces a rare crash on resize for some reason
void CellPalette::resizeEvent(QResizeEvent *event) {
    const int old_cpr = m_cellsPerRow, old_cpc = m_cellsPerCol;
    updateSpacing();
    if (old_cpr == m_cellsPerRow && old_cpc == m_cellsPerCol) return;

    for (int y = 0; y < MAX_CELL_COUNT; ++y) {
        for (int x = 0; x < MAX_CELL_COUNT; ++x) {
            const int arrIndex = MAX_CELL_COUNT * y + x;

            m_viewers[arrIndex]->setVisible(x < m_cellsPerRow && y < m_cellsPerCol);
        }
    }
    updatePagination();
}

int CellPalette::pointToIndex(QPoint p) const {
    return -1;
}

void CellPalette::updateSpacing() {
    m_cellsPerRow = (width() - m_spacing) / (CELL_SIZE + m_spacing);
    m_cellsPerCol = (height() - m_spacing) / (CELL_SIZE + m_spacing);
}
