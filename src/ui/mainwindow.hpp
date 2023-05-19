#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include "draw_area.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void tick();

private:
    Ui::MainWindow *m_ui;
    DrawArea *m_drawarea;
};
#endif  // MAINWINDOW_HPP
