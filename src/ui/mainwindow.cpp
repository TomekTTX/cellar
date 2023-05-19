#include "mainwindow.hpp"

#include <QShortcut>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_drawarea = m_ui->drawarea;
    new QShortcut(Qt::Key_Space, this, SLOT(tick()));
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::tick() {
    // qDebug() << "TICK";
    m_drawarea->tick();
    m_drawarea->repaint();
}
