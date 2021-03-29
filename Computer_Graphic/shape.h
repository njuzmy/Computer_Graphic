#ifndef SHAPE_H
#define SHAPE_H
#include <QtGui>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QStyle>

#define SMALL_CIRCLE_R 2

class Shape {
public:
    enum Code{
        Line,
        Rect,
        Fill_Rect,
        Polygon,
        Ellipse,
        Curve
    };

    struct Edge{
        qreal ymax;
        qreal x;
        qreal k; // 斜率
        Edge* next;
    };

    QString name;

    virtual void setStart(QPointF s){start = s;}
    virtual void setEnd(QPointF e){end = e;}
    virtual void setColor(QColor c){color = c;}
    virtual QPointF getEnd(){return end;}
    virtual void drawLine(QPainter *painter,QPointF start, QPointF end);
    virtual void drawRect(QPainter *painter,QPointF start, QPointF end);
    virtual void drawdashRect(QPainter *painter,QPointF start, QPointF end);
    virtual void drawPoligon(QPainter *painter, QPointF* pointlist, int num,qreal factor);
    virtual void drawfill_Poligin(QPainter *painter, QPointF* pointlist, int num);
    virtual void drawEllipse(QPainter *painter, QPointF start, QPointF end);
    virtual void drawfill_rect(QPainter *painter, QPointF start,QPointF end);
    virtual void drawCurve(QPainter* painter, QPointF* pointlist, int num);
    Shape();

    virtual void startDraw(QGraphicsSceneMouseEvent*) = 0;
    virtual void drawing(QGraphicsSceneMouseEvent*) = 0;
    virtual QRectF getRect() const;

protected:
    QPointF start;
    QPointF end;

    QPointF top_left;
    QPointF top_center;
    QPointF top_right;
    QPointF left_center;
    QPointF right_center;
    QPointF bottom_left;
    QPointF bottom_center;
    QPointF bottom_right;
    QPointF center;

    QColor color;

    bool If_tl_resizing;
    bool If_tc_resizing;
    bool If_tr_resizing;
    bool If_lc_resizing;
    bool If_rc_resizing;
    bool If_bl_resizing;
    bool If_bc_resizing;
    bool If_br_resizing;

    bool If_rotating;

private:
};

class Line: public Shape, public QGraphicsLineItem{
public:
    Line();

    //void drawLine(QPainter *painter,QPointF start, QPointF end);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    qreal rotating(QPointF x, QPointF y, QPointF z);
    QPainterPath shape()const Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    //QRectF getRect() Q_DECL_OVERRIDE;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;

private:
    QRectF cur_rect;
};

class Small_circle:public QGraphicsEllipseItem{
public:
    Small_circle();
};

class Rect:public Shape, public QGraphicsRectItem{
public:
    Rect();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    qreal rotating(QPointF x, QPointF y, QPointF z);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
    //void keyPressEvent(QKeyEvent *event)Q_DECL_OVERRIDE;

private:
    QRectF cur_rect;

};

class Fill_Rect:public Shape,public QGraphicsRectItem{
public:
    Fill_Rect();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    qreal rotating(QPointF x, QPointF y, QPointF z);
    int type()const Q_DECL_OVERRIDE ;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;

private:
    QRectF cur_rect;
};

class Polygon:public Shape,public QGraphicsPolygonItem{
public:
    Polygon();

    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    QPainterPath shape()const Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    qreal rotating(QPointF x, QPointF y, QPointF z);
    void setPoint(QPointF);
    void setPoint1(QPointF f){
        pointlist[cur_pos] = f;
        pos = cur_pos + 1;
    }
    void setStart(QPointF) Q_DECL_OVERRIDE;
    void debug(){
        for(int i = 0; i< pos;i++)
           qDebug()<<mapToScene(pointlist[i]);
        qDebug()<<"pos"<<pos<<"cur_pos"<<cur_pos;
    }
    QPolygonF getpolygon() const{
        QPolygonF polygon;
        for(int i = 0; i< pos;i++)
            polygon<<pointlist[i];
        return polygon;
    }

    void set_true(){
        if_continue = true;
    }

    void set_false(){
        if_continue = false;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    QRectF getRect() const Q_DECL_OVERRIDE;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;

private:
    qreal resizing_factor(qreal,qreal);
    void zoom(qreal factor);
    QPointF pointlist[10];
    QPointF pointlist_scene[10];
    int cur_pos;
    bool if_resizing;
    int distance;
    int pos;

    int distence(QPointF);
    bool if_continue;                //是否在画多边形
    qreal m_resizing;
};

class Ellipse:public Shape, public QGraphicsEllipseItem{
public:
    Ellipse();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    qreal rotating(QPointF x, QPointF y, QPointF z);
    QPainterPath shape()const Q_DECL_OVERRIDE;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;

private:
    QRectF cur_rect;
};

class Curve:public Shape,public QGraphicsItem{
public:
    Curve();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void setStart(QPointF) Q_DECL_OVERRIDE;
    void setPoint(QPointF f){
        pointlist[cur_pos] = f;
        cur_pos++;
        pos = cur_pos;
    }
    void debug(){
        for(int i = 0; i< pos;i++)
           qDebug()<<(pointlist[i]);
        qDebug()<<"pos"<<pos<<"cur_pos"<<cur_pos;
    }
    void setPoint1(QPointF f){
        pointlist[cur_pos] = f;
        pos = cur_pos + 1;
    }
    void set_true(){
        if_continue = true;
    }

    void set_false(){
        if_continue = false;
    }

    QRectF getRect() const Q_DECL_OVERRIDE;


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;

private:
    QPointF pointlist[10];
    qreal resizing_factor(qreal,qreal);
    //void zoom(qreal factor);
    QPointF pointlist_scene[10];
    int cur_pos;
    bool if_resizing;
    int distance;
    int pos;

    int distence(QPointF);
    bool if_continue;                //是否在画曲线
    qreal m_resizing;
};

class Dashrect:public Shape,public QGraphicsRectItem{
public :
    Dashrect();

    void paint(QPainter* painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void setStart(QPointF) Q_DECL_OVERRIDE;

};

class Mypixmap:public Shape,public QGraphicsPixmapItem{
public:
    Mypixmap(QPixmap x,QPointF s,QPointF e);
    Mypixmap(QPixmap x);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *) Q_DECL_OVERRIDE;
    qreal resizing_factor(qreal t,qreal l);
    qreal m_resizing;
    void startDraw(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    void drawing(QGraphicsSceneMouseEvent*) Q_DECL_OVERRIDE;
    qreal rotating(QPointF x, QPointF y, QPointF z);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event);
};

#endif
