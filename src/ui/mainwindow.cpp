#include "mainwindow.hpp"

#include <QShortcut>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_drawarea = m_ui->drawarea;
    m_stackviewer = m_ui->stackviewer;
    m_drawarea->setViewer(m_stackviewer);
    m_drawarea->setPalette(m_ui->palette);
    m_stackviewer->setMinimumHeight(CELL_SIZE + 1);
    m_stackviewer->setLabel(m_ui->stacklabel);
    m_ui->palette->connectWidgets(m_ui->pageLabel, m_ui->prevButton, m_ui->nextButton);

    new QShortcut(Qt::Key_Space, this, SLOT(tick()));
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::tick() {
    m_drawarea->tick();
    m_drawarea->repaint();
}
