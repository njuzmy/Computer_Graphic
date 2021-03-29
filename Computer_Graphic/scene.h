#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsView>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <GL/gl.h>
#include <QtOpenGL/qgl.h>
#include <GL/glu.h>

#include "shape.h"

class Scene:public QGraphicsScene{
    Q_OBJECT
public:
    Scene(QObject *parent);//Item = new Line}
    QColor getColor(){return color;}
    void zoom(QGraphicsItem* item, qreal scaleFactor);
    void save();
    void open();
    void newfile();
    void rotate(QGraphicsItem* item);
    //~Scene();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void keyPressEvent(QKeyEvent *);
public slots:
    void setCurrentShape(Shape::Code s){
        if_select = true;
        if(s!=currentShape){
            currentShape = s;
        }
        if_color = false;              //填充键只有在点击其他键时切换（仿照windows画图软件）
        cur_shape = nullptr;
        if_continue = false;
    }

    void setCurrentColor(){
        color = QColorDialog::getColor(color);
        if_color = true;
        cur_shape = nullptr;
        if_continue = false;
    }

    void getresizing(int i){
        if (i == 1){
            m_resizing += 0.02;
            foreach(QGraphicsItem* item,selectedItems()){
                zoom(item,m_resizing);
            }
        }
        else{
            m_resizing -= 0.02;
            foreach(QGraphicsItem* item,selectedItems()){
                zoom(item,m_resizing);
            }
        }                               //实现缩放功能
        cur_shape = nullptr;
        if_continue = false;
    }

    void getDelete(int i){
        if(i == 1)
            removeItem(items()[0]);
        else{
            foreach(QGraphicsItem *item, items())
                removeItem(item);
        }
        if_color = false;
    }

    void getRotate(){
        m_rotating = (m_rotating + 90)%360;
        foreach(QGraphicsItem* item,selectedItems()){
            rotate(item);
        }
    }

    void getSave(){
        save();
    }

    void getOpen(){
        open();
    }

    void getnewfile(){
        newfile();
    }

    void getcrop(){
        if_crop = true;
        rect = new Dashrect();
        if_color = false;              //填充键只有在点击其他键时切换（仿照windows画图软件）
        cur_shape = nullptr;
        if_continue = false;
    }
private:
    bool if_trigger;
    bool if_select;
    bool if_color;
    bool if_deleteall;
    bool if_continue;
    bool if_crop;

    qreal m_resizing;
    int m_rotating;
    QPointF topleft;
    QPointF bottomright;

    Shape::Code currentShape;
    Shape* Item;
    Shape* cur_shape;
    Dashrect* rect;

    QColor color;

    QCursor cursor;
};

class View:public QGraphicsView{
    Q_OBJECT
public:
    View(QGraphicsScene *scene, QWidget *parent);

protected:
    //void mousePressEvent(QMouseEvent *)Q_DECL_OVERRIDE;
    //void mouseMoveEvent(QMouseEvent *)Q_DECL_OVERRIDE;
    //void mouseReleaseEvent(QMouseEvent *)Q_DECL_OVERRIDE;
    //void keyPressEvent(QKeyEvent *);
};

#endif // SCENE_H
