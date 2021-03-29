#include "shape.h"
#include <QGraphicsScene>
#include <QGraphicsView>

Mypixmap::Mypixmap(QPixmap x,QPointF s,QPointF e):
    QGraphicsPixmapItem(x){
    name = "rect";
    If_br_resizing = false;
    If_bl_resizing = false;
    If_bc_resizing = false;
    If_lc_resizing = false;
    If_rc_resizing = false;
    If_tc_resizing = false;
    If_tl_resizing = false;
    If_tr_resizing = false;
    If_rotating = false;
    start = s;
    end = e;
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

Mypixmap::Mypixmap(QPixmap x):
    QGraphicsPixmapItem(x){}

void Mypixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;

    QGraphicsPixmapItem::paint(painter,&op,widget);

    if(option->state & QStyle::State_Selected){
        qreal itemPenWidth = painter->pen().widthF();
        const qreal pad = itemPenWidth / 2;
        const qreal penWidth = 0;

        QColor color = QColor(Qt::red);

        painter->setPen(QPen(color,penWidth,Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(pad, pad, -pad, -pad));

        top_left = boundingRect().topLeft();
        top_right = boundingRect().topRight();
        top_center = QPointF((top_left.x()+top_right.x())/2 , top_left.y());
        bottom_left = boundingRect().bottomLeft();
        bottom_right = boundingRect().bottomRight();
        left_center = QPointF(top_left.x(),(top_left.y() + bottom_left.y())/2);
        right_center = QPointF(top_right.x(),(top_right.y() + bottom_right.y())/2);
        bottom_center = QPointF((bottom_left.x() + bottom_right.x())/2, bottom_left.y());
        center = QPointF((top_left.x() + top_right.x())/2, (top_left.y() + bottom_left.y())/2);

        painter->setPen(Qt::black);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QRectF(top_left.x() - SMALL_CIRCLE_R+2, top_left.y() - SMALL_CIRCLE_R+2, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(top_right.x() - SMALL_CIRCLE_R-2, top_right.y() - SMALL_CIRCLE_R+2, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(bottom_left.x() - SMALL_CIRCLE_R+2, bottom_left.y() - SMALL_CIRCLE_R-2, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(bottom_right.x() - SMALL_CIRCLE_R-2, bottom_right.y() - SMALL_CIRCLE_R-2, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(center.x() - SMALL_CIRCLE_R-2, top_left.y() + 15 - SMALL_CIRCLE_R-2, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));

    }
}

void Mypixmap::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setAcceptHoverEvents(true);
    this->scene()->views().at(0)->setMouseTracking(false);                  //不接受鼠标移动消息
    if(event->button() == Qt::LeftButton){
        if(event->modifiers() == Qt::ShiftModifier){
            setSelected(true);
        }
        else if(sqrt(pow(event->pos().x() - boundingRect().bottomRight().x(), 2) + pow(event->pos().y() - boundingRect().bottomRight().y(), 2)) < 10){
            If_br_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - boundingRect().bottomLeft().x(), 2) + pow(event->pos().y() - boundingRect().bottomLeft().y(), 2)) < 10){
            If_bl_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - boundingRect().topRight().x(), 2) + pow(event->pos().y() - boundingRect().topRight().y(), 2)) < 10){
            If_tr_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() -boundingRect().topLeft().x(), 2) + pow(event->pos().y() - boundingRect().topLeft().y(), 2)) < 10){
            If_tl_resizing = true;
        }
        else if(sqrt(pow(event->pos().x() - (boundingRect().topRight().x() + boundingRect().topLeft().x())/2, 2) + pow(event->pos().y() - (boundingRect().topRight().y() + 15), 2))< 10){
            If_rotating = true;
        }
        else{
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    }
}

void Mypixmap::hoverMoveEvent(QGraphicsSceneHoverEvent *event){
    if((sqrt(pow(event->pos().x() - boundingRect().bottomRight().x(), 2) + pow(event->pos().y() - boundingRect().bottomRight().y(), 2)) < 10 &&boundingRect().contains(event->pos()) && isSelected()) ||
            (sqrt(pow(event->pos().x() -boundingRect().topLeft().x(), 2) + pow(event->pos().y() - boundingRect().topLeft().y(), 2)) < 10 && boundingRect().contains(event->pos())&&isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeFDiagCursor);
    else if((sqrt(pow(event->pos().x() - boundingRect().bottomLeft().x(), 2) + pow(event->pos().y() - boundingRect().bottomLeft().y(), 2)) < 10 && boundingRect().contains(event->pos())&& isSelected()) ||
            (sqrt(pow(event->pos().x() - boundingRect().topRight().x(), 2) + pow(event->pos().y() - boundingRect().topRight().y(), 2)) < 10 && boundingRect().contains(event->pos())&& isSelected()))
        this->scene()->views().at(0)->setCursor(Qt::SizeBDiagCursor);
    else if(sqrt(pow(event->pos().x() - center.x(), 2) + pow(event->pos().y() - top_center.y() - 15, 2)) < 10 && boundingRect().contains(event->pos())&& isSelected())
        this->scene()->views().at(0)->setCursor(Qt::CrossCursor);
    else
        this->scene()->views().at(0)->setCursor(Qt::ArrowCursor);
}

qreal Mypixmap::resizing_factor(qreal t,qreal l){
    m_resizing = t/l;
    return t/l;
}

qreal Mypixmap::rotating(QPointF x, QPointF y, QPointF z){
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

void Mypixmap::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(If_br_resizing){
            qreal l = boundingRect().bottomRight().x() - bottom_left.x();
            //qDebug()<<boundingRect().bottomRight().x()<<event->pos().x();
            setTransformOriginPoint(boundingRect().topLeft());
            setScale(resizing_factor(event->pos().x() - bottom_left.x(),l));
        }
        else if(If_bl_resizing){
            qreal l = boundingRect().bottomLeft().x() - bottom_right.x();
            setTransformOriginPoint(boundingRect().topRight());
            setScale(resizing_factor(event->pos().x() - bottom_right.x(),l));
        }
        else if(If_tl_resizing){
            qreal l = boundingRect().topLeft().x() - top_right.x();
            setTransformOriginPoint(boundingRect().bottomRight());
            setScale(resizing_factor(event->pos().x() - top_right.x(),l));
        }
        else if(If_tr_resizing){
            qreal l = boundingRect().bottomRight().x() - top_left.x();
            setTransformOriginPoint(boundingRect().bottomLeft());
            setScale(resizing_factor(event->pos().x() - top_left.x(),l));
        }
    else if(If_rotating){
        setTransformOriginPoint(center);
        setRotation(rotating(center,top_center,event->pos()));
    }
    else{
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void Mypixmap::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        if(If_br_resizing)
            If_br_resizing = false;
        else if(If_bl_resizing)
            If_bl_resizing = false;
        else if(If_tr_resizing)
            If_tr_resizing = false;
        else if(If_tl_resizing)
            If_tl_resizing = false;
        else if(If_rotating)
            If_rotating = false;
        else
            QGraphicsItem::mouseReleaseEvent(event);
    this->scene()->views().at(0)->setMouseTracking(true);
}

void Mypixmap::startDraw(QGraphicsSceneMouseEvent* event){
    //polygon<<start;
}

void Mypixmap::drawing(QGraphicsSceneMouseEvent* event){
    //setPolygon(getpolygon());
}

void Mypixmap::wheelEvent(QWheelEvent* event){
    qreal zoomInFactor = 1.25;
    qreal zoomOutFactor = 1 / zoomInFactor;
    qreal zoomFactor;
    if (event->angleDelta().y() > 0)
          zoomFactor = zoomInFactor;
    else
          zoomFactor = zoomOutFactor;
    setScale(zoomFactor);
}
