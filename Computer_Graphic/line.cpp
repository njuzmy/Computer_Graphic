#include "shape.h"
#include <QGraphicsScene>
#include <QGraphicsView>

Line::Line(){
    name = "line";
    If_br_resizing = false;
    If_bl_resizing = false;
    If_bc_resizing = false;
    If_lc_resizing = false;
    If_rc_resizing = false;
    If_tc_resizing = false;
    If_tl_resizing = false;
    If_tr_resizing = false;
    If_rotating = false;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;


    //QGraphicsLineItem::paint(painter,&op,widget);
    drawLine(painter,start,end);
    //painter->drawPoint(start);


    if(option->state & QStyle::State_Selected){
        qreal itemPenWidth = pen().widthF();
        const qreal pad = itemPenWidth/2;
        const qreal penWidth = 2;

        QColor color = QColor(Qt::red);

        painter->setPen(QPen(color,penWidth,Qt::DashLine));
        painter->drawRect(getRect().adjusted(pad,pad,-pad,-pad));

        top_left = getRect().topLeft();
        top_right = getRect().topRight();
        top_center = QPointF((top_left.x()+top_right.x())/2 , top_left.y());
        bottom_left = getRect().bottomLeft();
        bottom_right = getRect().bottomRight();
        left_center = QPointF(top_left.x(),(top_left.y() + bottom_left.y())/2);
        right_center = QPointF(top_right.x(),(top_right.y() + bottom_right.y())/2);
        bottom_center = QPointF((bottom_left.x() + bottom_right.x())/2, bottom_left.y());
        center = QPointF((top_left.x() + top_right.x())/2, (top_left.y() + bottom_left.y())/2);

        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QRectF(start.x() - SMALL_CIRCLE_R, start.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(end.x() - SMALL_CIRCLE_R, end.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(center.x() - SMALL_CIRCLE_R, top_left.y() + 15 - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
    }
}

void Line::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setAcceptHoverEvents(true);
    this->scene()->views().at(0)->setMouseTracking(false);                  //不接受鼠标移动消息
    if(event->button() == Qt::LeftButton){
        if(event->modifiers() == Qt::ShiftModifier){
            setSelected(true);
        }
        else if(sqrt(pow(event->pos().x() - end.x(), 2) + pow(event->pos().y() - end.y(), 2)) < 10){
            If_br_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - start.x(), 2) + pow(event->pos().y() - start.y(), 2)) < 10){
            If_tl_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - (getRect().topRight().x() + getRect().topLeft().x())/2, 2) + pow(event->pos().y() - (getRect().topRight().y() + 15), 2))< 10){
            If_rotating = true;
        }
        else{
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    }
}

void Line::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    if((sqrt(pow(event->pos().x() - end.x(), 2) + pow(event->pos().y() - end.y(), 2)) < 10 && getRect().contains(event->pos())) ||
            (sqrt(pow(event->pos().x() - start.x(), 2) + pow(event->pos().y() - start.y(), 2)) < 10 && getRect().contains(event->pos()))){
        if(start == getRect().topLeft() || start == getRect().bottomRight())
            this->scene()->views().at(0)->setCursor(Qt::SizeFDiagCursor);
        else
            this->scene()->views().at(0)->setCursor(Qt::SizeBDiagCursor);
    }
    else if((sqrt(pow(event->pos().x() - center.x(), 2) + pow(event->pos().y() - top_center.y() - 15, 2)) < 10 && getRect().contains(event->pos())))
        this->scene()->views().at(0)->setCursor(Qt::CrossCursor);
    else
        this->scene()->views().at(0)->setCursor(Qt::ArrowCursor);
}

void Line::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(If_br_resizing){
        setEnd(event->pos());
        setLine(QLineF(start,end));
    }
    else if(If_tl_resizing){
        setStart(event->pos());
        setLine(QLineF(start,end));
    }
    else if(If_rotating){
        setTransformOriginPoint(center);
        setRotation(rotating(center,top_center,event->scenePos()));
    }
    else{
        QGraphicsItem::mouseMoveEvent(event);
        //this->scene()->views().at(0)->setCursor(Qt::SizeFDiagCursor);
    }
}

void Line::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    //cur_rect = getRect();
    if(If_br_resizing){
        If_br_resizing = false;
        //setStart(cur_rect.topLeft());
        //setEnd(cur_rect.bottomRight());
    }
    else if(If_tl_resizing){
        If_tl_resizing = false;
        //setStart(cur_rect.topLeft());
        //setEnd(cur_rect.bottomRight());
    }
    else if(If_rotating){
        If_rotating = false;
    }
    else
        QGraphicsItem::mouseReleaseEvent(event);
    this->scene()->views().at(0)->setMouseTracking(true);
}

qreal Line::rotating(QPointF x, QPointF y, QPointF z){
    qreal cosA;
    qreal c = sqrt(pow(x.x() - y.x(),2) + pow(x.y() - y.y(),2));
    qreal b = sqrt(pow(x.x() - z.x(),2) + pow(x.y() - z.y(),2));
    qreal a = sqrt(pow(y.x() - z.x(),2) + pow(y.y() - z.y(),2));
    cosA = (b*b + c*c - a*a)/(2*b*c);
    //qDebug()<<a<<b<<c;
    qreal A = acos(cosA) * 57.3;
    //qDebug()<<cosA<<A;
    if(x.x()<z.x())
        return A;
    else
        return 360-A;
}

void Line::startDraw(QGraphicsSceneMouseEvent* event){
    const qreal penWidth = 3;
    QColor color = QColor(Qt::black);
    setPen(QPen(color,penWidth,Qt::SolidLine));
    setLine(QLineF(event->scenePos(),event->scenePos()));
}

void Line::drawing(QGraphicsSceneMouseEvent* event){
    QLineF newLine(line().p1(), end);
    setLine(newLine);
}
QRectF Line::boundingRect() const{
    if(start.x()<end.x() && start.y()<end.y()){
        return QRectF(start.x() - 4,start.y() - 4,end.x()-start.x() + 8,end.y()-start.y() + 8);
    }
    else if(start.x()>= end.x() && start.y()<end.y()){
        return QRectF(end.x() - 4,start.y() - 4,start.x()-end.x() + 8,end.y()-start.y() + 8);
    }
    else if(start.x() < end.x() && start.y() >= end.y()){
        return QRectF(start.x() - 4,end.y() - 4,end.x()-start.x() + 8,start.y()-end.y() + 8);
    }
    else{
        return QRectF(end.x() - 4,end.y() - 4,start.x()-end.x() + 8,start.y()-end.y() + 8);
    }
}

QPainterPath Line::shape() const{
    QPainterPath mypath;
    mypath.addRect(getRect());
    return mypath;
}
