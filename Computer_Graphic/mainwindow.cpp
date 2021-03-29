#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include <QWidget>
#include <QPainter>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMessageBox mb( "Drawing Board", "请选择绘图模式？", QMessageBox::Information, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No, QMessageBox::Cancel | QMessageBox::Escape );
    mb.setButtonText( QMessageBox::Yes, "2D模式" );
    mb.setButtonText( QMessageBox::No, "3D模式" );
    mb.setButtonText( QMessageBox::Cancel, "取消" );
    switch(mb.exec()){
    case QMessageBox::Yes:
        scene = new Scene(this);
        scene->setSceneRect(0,0,1000,1000);
        view = new View(scene, this);
        this->setCentralWidget(view);
        this->setWindowTitle("Drawing Board - 2D");
        ui->statusBar->showMessage("就绪");
        if_widget = false;
        connect(this,SIGNAL(changeCurrentShape(Shape::Code)),scene,SLOT(setCurrentShape(Shape::Code)));
        connect(this,SIGNAL(changeCurrentColor()),scene,SLOT(setCurrentColor()));
        connect(this,SIGNAL(Delete(int)),scene,SLOT(getDelete(int)));
        connect(this,SIGNAL(resize(int)),scene,SLOT(getresizing(int)));
        connect(this,SIGNAL(save()),scene,SLOT(getSave()));
        connect(this,SIGNAL(open()),scene,SLOT(getOpen()));
        connect(this,SIGNAL(newfile()),scene,SLOT(getnewfile()));
        connect(this,SIGNAL(crop()),scene,SLOT(getcrop()));
        ui->actioninvert->setDisabled(true);
        connect(this,SIGNAL(rotate()),scene,SLOT(getRotate()));
        break;
    case QMessageBox::No:
        glwidget = new openglwidget(this,nullptr);
        this->setCentralWidget(glwidget);
        glwidget->setFocusPolicy(Qt::StrongFocus);
        this->setWindowTitle("Drawing Board - 3D");
        ui->statusBar->showMessage("就绪");
        if_widget = true;
        ui->actionDrawCircle->setDisabled(true);
        ui->actionDrawEllipse->setDisabled(true);
        ui->actionDrawStraight->setDisabled(true);
        ui->actionempty_rect->setDisabled(true);
        ui->actionfull_rect->setDisabled(true);
        ui->actioncurve->setDisabled(true);
        ui->actionzoom->setDisabled(true);
        ui->actionlager->setDisabled(true);
        ui->actionfill->setDisabled(true);
        ui->actioncrop->setDisabled(true);
        ui->actionpaint->setDisabled(true);
        ui->actiondelete_one->setDisabled(true);
        ui->actiondelete->setDisabled(true);
        connect(this,SIGNAL(off()),glwidget,SLOT(getoff()));
        connect(this,SIGNAL(invert()),glwidget,SLOT(getinvert()));
        break;
    case QMessageBox::Cancel:
        exit(0);
    default:
        exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionDrawStraight_triggered()
{

    emit(changeCurrentShape(Shape::Line));
}

void MainWindow::on_actionempty_rect_triggered()
{
    emit(changeCurrentShape(Shape::Rect));
}

void MainWindow::on_actionfull_rect_triggered()
{

    emit(changeCurrentShape(Shape::Fill_Rect));
}

void MainWindow::on_actionfill_triggered()
{
    emit(changeCurrentColor());
}

void MainWindow::on_actionpaint_triggered()
{
    emit(rotate());
}

void MainWindow::on_actiondelete_one_triggered()
{
    emit(Delete(1));
}

void MainWindow::on_actiondelete_triggered()
{
    emit(Delete(2));
}

void MainWindow::on_actionDrawCircle_triggered()
{
    emit(changeCurrentShape(Shape::Polygon));
}

void MainWindow::on_actionzoom_triggered()
{
    emit(resize(1));
}

void MainWindow::on_actionlager_triggered()
{
    emit(resize(0));
}

void MainWindow::on_actionsave_triggered()
{
    emit(save());
}

void MainWindow::on_actionopen_triggered()
{
    emit(open());
}

void MainWindow::on_actionDrawEllipse_triggered()
{
    emit(changeCurrentShape(Shape::Ellipse));
}

void MainWindow::on_actionnew_triggered()
{
    emit(newfile());
}

void MainWindow::on_actioncurve_triggered()
{
    emit(changeCurrentShape(Shape::Curve));
}

void MainWindow::on_actioncrop_triggered()
{
    emit(crop());
}

void MainWindow::on_actionoff_triggered()
{
    emit(off());
}

void MainWindow::on_actionquit_triggered()
{
    exit(0);
}

void MainWindow::on_actionswitch_triggered()
{
    if(if_widget){
        if_widget = false;
        scene = new Scene(this);
        scene->setSceneRect(0,0,1000,1000);
        view = new View(scene, this);
        this->setCentralWidget(view);
        this->setWindowTitle("Drawing Board - 2D");
        ui->statusBar->showMessage("就绪");
        if_widget = false;
        ui->actionDrawCircle->setDisabled(false);
        ui->actionDrawEllipse->setDisabled(false);
        ui->actionDrawStraight->setDisabled(false);
        ui->actionempty_rect->setDisabled(false);
        ui->actionfull_rect->setDisabled(false);
        ui->actioncurve->setDisabled(false);
        ui->actionzoom->setDisabled(false);
        ui->actionlager->setDisabled(false);
        ui->actionfill->setDisabled(false);
        ui->actioncrop->setDisabled(false);
        ui->actionpaint->setDisabled(false);
        ui->actiondelete_one->setDisabled(false);
        ui->actiondelete->setDisabled(false);
        ui->actioninvert->setDisabled(true);
        connect(this,SIGNAL(changeCurrentShape(Shape::Code)),scene,SLOT(setCurrentShape(Shape::Code)));
        connect(this,SIGNAL(changeCurrentColor()),scene,SLOT(setCurrentColor()));
        connect(this,SIGNAL(Delete(int)),scene,SLOT(getDelete(int)));
        connect(this,SIGNAL(resize(int)),scene,SLOT(getresizing(int)));
        connect(this,SIGNAL(save()),scene,SLOT(getSave()));
        connect(this,SIGNAL(open()),scene,SLOT(getOpen()));
        connect(this,SIGNAL(newfile()),scene,SLOT(getnewfile()));
        connect(this,SIGNAL(crop()),scene,SLOT(getcrop()));
        connect(this,SIGNAL(rotate()),scene,SLOT(getRotate()));
    }
    else{
        if_widget = true;
        glwidget = new openglwidget(this,nullptr);
        this->setCentralWidget(glwidget);
        glwidget->setFocusPolicy(Qt::StrongFocus);
        this->setWindowTitle("Drawing Board - 3D");
        ui->statusBar->showMessage("就绪");
        if_widget = true;
        ui->actionDrawCircle->setDisabled(true);
        ui->actionDrawEllipse->setDisabled(true);
        ui->actionDrawStraight->setDisabled(true);
        ui->actionempty_rect->setDisabled(true);
        ui->actionfull_rect->setDisabled(true);
        ui->actioncurve->setDisabled(true);
        ui->actionzoom->setDisabled(true);
        ui->actionlager->setDisabled(true);
        ui->actionfill->setDisabled(true);
        ui->actioncrop->setDisabled(true);
        ui->actionpaint->setDisabled(true);
        ui->actiondelete_one->setDisabled(true);
        ui->actiondelete->setDisabled(true);
        ui->actioninvert->setDisabled(false);
        connect(this,SIGNAL(off()),glwidget,SLOT(getoff()));
        connect(this,SIGNAL(invert()),glwidget,SLOT(getinvert()));
    }
}

void MainWindow::on_actioninvert_triggered()
{
    emit(invert());
}
