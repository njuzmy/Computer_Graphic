#include "shape.h"
#include <QGraphicsScene>
#include <QGraphicsView>

Curve::Curve(){
    if_resizing = false;
    name = "curve";
    cur_pos = 0;
    pos = 0;
    if_continue = false;                    //一开始不接受对item的鼠标信息
    If_br_resizing = false;
    If_bl_resizing = false;
    If_bc_resizing = false;
    If_lc_resizing = false;
    If_rc_resizing = false;
    If_tc_resizing = false;
    If_tl_resizing = false;
    If_tr_resizing = false;
    If_rotating = false;
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void Curve::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;


    //QGraphicsLineItem::paint(painter,&op,widget);
    //debug();
    drawCurve(painter,pointlist,pos);
    //painter->drawPoint(start);


    if(option->state & QStyle::State_Selected){
        qreal itemPenWidth = painter->pen().widthF();
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

void Curve::startDraw(QGraphicsSceneMouseEvent* event){
    //polygon<<start;
}

void Curve::drawing(QGraphicsSceneMouseEvent* event){
    prepareGeometryChange();                     //调用paint函数
}

void Curve::setStart(QPointF f){
    start = f;
    pointlist[cur_pos] = f;
    cur_pos ++;
    pos = cur_pos;
}

QRectF Curve::boundingRect() const{
    qreal minx = pointlist[0].x();
    qreal miny = pointlist[0].y();
    qreal maxx = pointlist[0].x();
    qreal maxy = pointlist[0].y();
    for(int i = 0;i<pos;i++){
        if(pointlist[i].x()<minx)
            minx = pointlist[i].x();
        if(pointlist[i].x()>maxx)
            maxx = pointlist[i].x();
        if(pointlist[i].y()<miny)
            miny = pointlist[i].y();
        if(pointlist[i].y()>maxy)
            maxy = pointlist[i].y();
    }
    return QRectF(QPointF(minx - 2,miny - 2),QPointF(maxx + 2,maxy + 2));
}

QRectF Curve::getRect() const{
    qreal minx = pointlist[0].x();
    qreal miny = pointlist[0].y();
    qreal maxx = pointlist[0].x();
    qreal maxy = pointlist[0].y();
    for(int i = 0;i<pos;i++){
        if(pointlist[i].x()<minx)
            minx = pointlist[i].x();
        if(pointlist[i].x()>maxx)
            maxx = pointlist[i].x();
        if(pointlist[i].y()<miny)
            miny = pointlist[i].y();
        if(pointlist[i].y()>maxy)
            maxy = pointlist[i].y();
    }
    return QRectF(QPointF(minx,miny),QPointF(maxx,maxy));
}

int Curve::distence(QPointF f){
    for(int i = 0; i < pos;i++){
        if(sqrt(pow(f.x() - pointlist[i].x(), 2) + pow(f.y() - pointlist[i].y(), 2)) < 10)
            return i;
    }
    return -1;
}

void Curve::mousePressEvent(QGraphicsSceneMouseEvent *event){
    this->scene()->views().at(0)->setMouseTracking(false);                  //不接受鼠标移动消息
    distance = distence(event->pos());
    if(event->button() == Qt::LeftButton){
        if(event->modifiers() == Qt::ShiftModifier){
            setSelected(true);
        }
        else if(if_continue && distance >= 0){
            if_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().bottomRight().x(), 2) + pow(event->pos().y() - getRect().bottomRight().y(), 2)) < 10){
            If_br_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().bottomLeft().x(), 2) + pow(event->pos().y() - getRect().bottomLeft().y(), 2)) < 10){
            If_bl_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().topRight().x(), 2) + pow(event->pos().y() - getRect().topRight().y(), 2)) < 10){
            If_tr_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().topLeft().x(), 2) + pow(event->pos().y() - getRect().topLeft().y(), 2)) < 10){
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

void Curve::hoverMoveEvent(QGraphicsSceneHoverEvent *event){

    if((sqrt(pow(event->pos().x() - getRect().bottomRight().x(), 2) + pow(event->pos().y() - getRect().bottomRight().y(), 2)) < 10 && getRect().contains(event->pos()) && isSelected()) ||
            (sqrt(pow(event->pos().x() - getRect().topLeft().x(), 2) + pow(event->pos().y() - getRect().topLeft().y(), 2)) < 10 && getRect().contains(event->pos())&&isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeFDiagCursor);
    else if((sqrt(pow(event->pos().x() - getRect().bottomLeft().x(), 2) + pow(event->pos().y() - getRect().bottomLeft().y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()) ||
            (sqrt(pow(event->pos().x() - getRect().topRight().x(), 2) + pow(event->pos().y() - getRect().topRight().y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeBDiagCursor);
    else if(sqrt(pow(event->pos().x() - center.x(), 2) + pow(event->pos().y() - top_center.y() - 15, 2)) < 10 && getRect().contains(event->pos())&& isSelected())
        this->scene()->views().at(0)->setCursor(Qt::CrossCursor);
    else
        this->scene()->views().at(0)->setCursor(Qt::ArrowCursor);
}

void Curve::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(if_resizing){
        pointlist[distance] = event->pos();
        prepareGeometryChange();
    }
    else if(If_br_resizing){
        qreal l = getRect().bottomRight().x() - bottom_left.x();
        setTransformOriginPoint(getRect().topLeft());
        setScale(resizing_factor(event->scenePos().x() - bottom_left.x(),l));
    }
    else if(If_bl_resizing){
        qreal l = getRect().bottomLeft().x() - bottom_right.x();
        setTransformOriginPoint(getRect().topRight());
        setScale(resizing_factor(event->scenePos().x() - bottom_right.x(),l));
    }
    else if(If_tl_resizing){
        qreal l = getRect().topLeft().x() - top_right.x();
        setTransformOriginPoint(getRect().bottomRight());
        setScale(resizing_factor(event->scenePos().x() - top_right.x(),l));
    }
    else if(If_tr_resizing){
        qreal l = getRect().bottomRight().x() - top_left.x();
        setTransformOriginPoint(getRect().bottomLeft());
        setScale(resizing_factor(event->scenePos().x() - top_left.x(),l));
    }
    /*else if(If_rotating){
        setTransformOriginPoint(center);
        setRotation(rotating(center,top_center,event->scenePos()));
    }*/
    else{
        QGraphicsItem::mouseMoveEvent(event);
    }
    for(int i = 0;i < pos;i++){
    }
}

qreal Curve::resizing_factor(qreal t,qreal l){
    m_resizing = t/l;
    return t/l;
}

void Curve::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(if_resizing){
        pointlist[distance] = event->pos();
        prepareGeometryChange();
        if_resizing = false;
    }
    else if(If_br_resizing)
        If_br_resizing = false;
    else if(If_bl_resizing)
        If_bl_resizing = false;
    else if(If_tr_resizing)
        If_tr_resizing = false;
    else if(If_tl_resizing)
        If_tl_resizing = false;
    else if(If_rotating){
        If_rotating = false;
    }
    else{
        QGraphicsItem::mouseReleaseEvent(event);
    }
    this->scene()->views().at(0)->setMouseTracking(true);
}
