#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "openglwidget.h"
#include "shape.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void changeCurrentShape(Shape::Code newShape);
    void changeCurrentColor();
    void Delete(int i);                  //i用来表示是撤回还是全部删除
    void resize(int i);                  //i用来表示放大还是缩小
    void save();
    void open();
    void newfile();
    void crop();                         //裁剪
    void off();
    void invert();                       //反选
    void rotate();                       //旋转

private slots:

    void on_actionDrawStraight_triggered();

    void on_actionempty_rect_triggered();

    void on_actionfull_rect_triggered();

    void on_actionfill_triggered();

    void on_actionpaint_triggered();

    void on_actiondelete_one_triggered();

    void on_actiondelete_triggered();

    void on_actionDrawCircle_triggered();

    void on_actionzoom_triggered();

    void on_actionlager_triggered();

    void on_actionsave_triggered();

    void on_actionopen_triggered();

    void on_actionDrawEllipse_triggered();

    void on_actionnew_triggered();

    void on_actioncurve_triggered();

    void on_actioncrop_triggered();

    void on_actionoff_triggered();

    void on_actionquit_triggered();

    void on_actionswitch_triggered();

    void on_actioninvert_triggered();

private:
    Ui::MainWindow *ui;
    QPaintEvent *event;

    QGraphicsScene* scene = nullptr;
    QGraphicsView* view;

    openglwidget* glwidget = nullptr;

    bool if_widget;
    //PixmapScene *scene;

};

#endif // MAINWINDOW_H
