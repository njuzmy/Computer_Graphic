#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <GL/gl.h>
#include <QtOpenGL/qgl.h>
#include <GL/glu.h>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QtGui>
#include <QGraphicsView>
#include <QFileDialog>

#include "shape.h"

struct VERTEX{
    float x,y,z;
};

struct OBJECT{
    int verts;
    QList<int> vPoints;
};

class openglwidget : public QGLWidget
{
    Q_OBJECT
public:
    openglwidget(QWidget *parent = nullptr, const char* name = nullptr);
    ~openglwidget();
public slots:
    void getoff(){
        QString filename = QFileDialog::getOpenFileName(this,tr("选择文件"),"","");
        loadObject(filename, &morph);
        if_selected = true;
    }
    void getinvert(){
        qDebug()<<"here";
        if_invert = !if_invert;
        update();
        //
        //glClearColor(1.0,1.0,1.0,1.0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    GLfloat rTri;
    GLfloat xRot,yRot,zRot,translate;
private:
    void loadObject(QString filename,OBJECT *k);
    boolean fullscreen;
    OBJECT morph;
    QList<VERTEX> points;
    QList<OBJECT> mians;
    int point_num;
    int mian_num;
    int bian_num;


    bool if_selected;
    bool if_invert;
};

#endif // SHOWWIDGET_H*/
