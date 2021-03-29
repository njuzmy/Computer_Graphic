#include "shape.h"
#include <QGraphicsScene>
#include <QGraphicsView>

Ellipse::Ellipse(){
    //setMouseTracking(true);
    name = "ellipse";
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

void Ellipse::paint(QPainter* painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;

    //QGraphicsEllipseItem::paint(painter,&op,widget);
    drawEllipse(painter,start,end);

    if(option->state & QStyle::State_Selected){
        qreal itemPenWidth = pen().widthF();
        const qreal pad = itemPenWidth/2;
        const qreal penWidth = 2;

        QColor color = QColor(Qt::red);

        painter->setBrush(QBrush(Qt::NoBrush));
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
        painter->drawEllipse(QRectF(top_left.x() - SMALL_CIRCLE_R, top_left.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(top_right.x() - SMALL_CIRCLE_R, top_right.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(top_center.x() - SMALL_CIRCLE_R, top_center.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(left_center.x() - SMALL_CIRCLE_R, left_center.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(bottom_left.x() - SMALL_CIRCLE_R, bottom_left.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(bottom_right.x() - SMALL_CIRCLE_R, bottom_right.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(bottom_center.x() - SMALL_CIRCLE_R, bottom_center.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(right_center.x() - SMALL_CIRCLE_R, right_center.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(center.x() - SMALL_CIRCLE_R, top_left.y() + 15 - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
    }
}

void Ellipse::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setAcceptHoverEvents(true);
    this->scene()->views().at(0)->setMouseTracking(false);                  //不接受鼠标移动消息
    if(event->button() == Qt::LeftButton){
        if(event->modifiers() == Qt::ShiftModifier){
            setSelected(true);
        }
        else if(sqrt(pow(event->pos().x() - getRect().bottomRight().x(), 2) + pow(event->pos().y() - getRect().bottomRight().y(), 2)) < 10){
            If_br_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().bottomLeft().x(), 2) + pow(event->pos().y() - getRect().bottomLeft().y(), 2)) < 10){
            If_bl_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - (getRect().bottomLeft().x() + getRect().bottomRight().x())/2, 2) + pow(event->pos().y() - getRect().bottomLeft().y(), 2)) < 10){
            If_bc_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().topRight().x(), 2) + pow(event->pos().y() - getRect().topRight().y(), 2)) < 10){
            If_tr_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().topLeft().x(), 2) + pow(event->pos().y() - getRect().topLeft().y(), 2)) < 10){
            If_tl_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - (getRect().topRight().x() + getRect().topLeft().x())/2, 2) + pow(event->pos().y() - getRect().topLeft().y(), 2)) < 10){
            If_tc_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().bottomLeft().x(), 2) + pow(event->pos().y() - (getRect().bottomLeft().y() + getRect().topLeft().y())/2, 2)) < 10){
            If_lc_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - getRect().bottomRight().x(), 2) + pow(event->pos().y() - (getRect().bottomRight().y() + getRect().topRight().y())/2, 2)) < 10){
            If_rc_resizing = true;
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

void Ellipse::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    //qDebug()<<"here";
    if((sqrt(pow(event->pos().x() - getRect().bottomRight().x(), 2) + pow(event->pos().y() - getRect().bottomRight().y(), 2)) < 10 && getRect().contains(event->pos()) && isSelected()) ||
            (sqrt(pow(event->pos().x() - getRect().topLeft().x(), 2) + pow(event->pos().y() - getRect().topLeft().y(), 2)) < 10 && getRect().contains(event->pos())&&isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeFDiagCursor);
    else if((sqrt(pow(event->pos().x() - getRect().bottomLeft().x(), 2) + pow(event->pos().y() - getRect().bottomLeft().y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()) ||
            (sqrt(pow(event->pos().x() - getRect().topRight().x(), 2) + pow(event->pos().y() - getRect().topRight().y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeBDiagCursor);
    else if((sqrt(pow(event->pos().x() - left_center.x(), 2) + pow(event->pos().y() - left_center.y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()) ||
            (sqrt(pow(event->pos().x() - right_center.x(), 2) + pow(event->pos().y() - right_center.y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeHorCursor);
    else if((sqrt(pow(event->pos().x() - top_center.x(), 2) + pow(event->pos().y() - top_center.y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()) ||
            (sqrt(pow(event->pos().x() - bottom_center.x(), 2) + pow(event->pos().y() - bottom_center.y(), 2)) < 10 && getRect().contains(event->pos())&& isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeVerCursor);
    else if(sqrt(pow(event->pos().x() - center.x(), 2) + pow(event->pos().y() - top_center.y() - 15, 2)) < 10 && getRect().contains(event->pos())&& isSelected())
        this->scene()->views().at(0)->setCursor(Qt::CrossCursor);
    else
        this->scene()->views().at(0)->setCursor(Qt::ArrowCursor);
}

void Ellipse::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(If_br_resizing){
        setEnd(event->pos());
        setRect(getRect());
    }
    else if(If_bl_resizing){
        setEnd(QPointF(end.x(),event->pos().y()));
        setStart(QPointF(event->pos().x(),start.y()));
        setRect(getRect());
    }
    else if(If_bc_resizing){
        setEnd(QPointF(end.x(),event->pos().y()));
        setRect(getRect());
    }
    else if(If_tl_resizing){
        setStart(event->pos());
        setRect(getRect());
    }
    else if(If_tr_resizing){
        setEnd(QPointF(event->pos().x(),end.y()));
        setStart(QPointF(start.x(),event->pos().y()));
        setRect(getRect());
    }
    else if(If_tc_resizing){
        setStart(QPointF(start.x(),event->pos().y()));
        setRect(getRect());
    }
    else if(If_lc_resizing){
        setStart(QPointF(event->pos().x(),start.y()));
        setRect(getRect());
    }
    else if(If_rc_resizing){
        setEnd(QPointF(event->pos().x(),end.y()));
        setRect(getRect());
    }
    else if(If_rotating){
        setTransformOriginPoint(center);
        setRotation(rotating(center,top_center,event->scenePos()));
    }
    else{
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void Ellipse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    cur_rect = getRect();
    if(If_br_resizing){
        If_br_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_bl_resizing){
        If_bl_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_bc_resizing){
        If_bc_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_lc_resizing){
        If_lc_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_rc_resizing){
        If_rc_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_tc_resizing){
        If_tc_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_tl_resizing){
        If_tl_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_tr_resizing){
        If_tr_resizing = false;
        setStart(cur_rect.topLeft());
        setEnd(cur_rect.bottomRight());
    }
    else if(If_rotating){
        If_rotating = false;
    }
    else
        QGraphicsItem::mouseReleaseEvent(event);
    this->scene()->views().at(0)->setMouseTracking(true);
}

QRectF Ellipse::boundingRect() const{
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
qreal Ellipse::rotating(QPointF x, QPointF y, QPointF z){
    qreal cosA;
    qreal c = sqrt(pow(x.x() - y.x(),2) + pow(x.y() - y.y(),2));
    qreal b = sqrt(pow(x.x() - z.x(),2) + pow(x.y() - z.y(),2));
    qreal a = sqrt(pow(y.x() - z.x(),2) + pow(y.y() - z.y(),2));
    cosA = (b*b + c*c - a*a)/(2*b*c);
    qreal A = acos(cosA) * 57.3;
    if(x.x()<z.x())
        return A;
    else
        return 360-A;
}

void Ellipse::startDraw(QGraphicsSceneMouseEvent* event){
    setRect(QRectF(event->scenePos(),QSizeF(0,0)));

}
void Ellipse::drawing(QGraphicsSceneMouseEvent* event){
    setRect(getRect());
}

QPainterPath Ellipse::shape()const{
    QPainterPath mypath;
    mypath.addRect(getRect());
    return mypath;
}
