#include "shape.h"
#include "QGraphicsScene"
#include "QGraphicsView"

Polygon::Polygon(){
    if_resizing = false;
    name = "polygon";
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

void Polygon::startDraw(QGraphicsSceneMouseEvent* event){
    //polygon<<start;
}

void Polygon::drawing(QGraphicsSceneMouseEvent* event){
    setPolygon(getpolygon());
}

void Polygon::setStart(QPointF f){
    pointlist[cur_pos] = f;
    start = f;
    cur_pos++;
    pos = cur_pos;
}

void Polygon::setPoint(QPointF f){
    pointlist[cur_pos] = f;
    cur_pos++;
    pos = cur_pos;
}

int Polygon::distence(QPointF f){
    for(int i = 0; i < pos;i++){
        if(sqrt(pow(f.x() - pointlist[i].x(), 2) + pow(f.y() - pointlist[i].y(), 2)) < 10)
            return i;
    }
    return -1;
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;

    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(color,2));
    QGraphicsPolygonItem::paint(painter,&op,widget);
    //qDebug()<<pos;

    //painter->setBrush(brush());
    //painter->setPen(QPen(color,2/m_resizing,Qt::DashLine));
    //drawPoligon(painter,pointlist,pos - 1,m_resizing);
    //qDebug()<<brush().color()<<painter->brush().color();
    if(brush().color() != painter->brush().color()){
        //qDebug()<<brush().color()<<Qt::white;
        painter->setBrush(brush());
        drawfill_Poligin(painter,pointlist,pos -1);
    }
    //qDebug()<<top_left<< bottom_right;
    if(option->state & QStyle::State_Selected){
        qreal itemPenWidth = pen().widthF();
        const qreal pad = itemPenWidth/2;
        const qreal penWidth = 2;

        QColor color = QColor(Qt::red);

        painter->setBrush(QBrush(Qt::NoBrush));
        painter->setPen(QPen(color,penWidth,Qt::DashLine));
        painter->drawRect(getRect().adjusted(pad,pad,-pad,-pad));

        // = getRect().topLeft();
        //end = getRect().bottomRight();
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
        painter->drawEllipse(QRectF(bottom_left.x() - SMALL_CIRCLE_R, bottom_left.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(bottom_right.x() - SMALL_CIRCLE_R, bottom_right.y() - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
        painter->drawEllipse(QRectF(center.x() - SMALL_CIRCLE_R, top_left.y() + 15 - SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R, 2*SMALL_CIRCLE_R));
    }
}

QRectF Polygon::boundingRect() const{
    return QRectF(QPointF(getRect().topLeft().x() - 4,getRect().topLeft().y()-4),QPointF(getRect().bottomRight().x() + 4,getRect().bottomRight().y() + 4));
}

void Polygon::mousePressEvent(QGraphicsSceneMouseEvent *event){
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

QRectF Polygon::getRect() const{
    qreal start_x,start_y,end_x,end_y;
    start_x = end_x = pointlist[0].x();
    start_y = end_y = pointlist[0].y();
    for(int i = 0; i<pos;i++){
        if(pointlist[i].x()<start_x)
            start_x = pointlist[i].x();
        if(pointlist[i].x()>end_x)
            end_x = pointlist[i].x();
        if(pointlist[i].y()<start_y)
            start_y = pointlist[i].y();
        if(pointlist[i].y()>end_y)
            end_y = pointlist[i].y();
    }
    QPointF startt = QPointF(start_x ,start_y );
    QPointF endd = QPointF(end_x ,end_y );
    return QRectF(startt,endd);
}

void Polygon::hoverMoveEvent(QGraphicsSceneHoverEvent *event){

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

/*qreal Abs(qreal t){
    if(t>=0)
        return t;
    else
        return -t;
}*/

qreal Polygon::resizing_factor(qreal t,qreal l){
    m_resizing = t/l;
    return t/l;
}


void Polygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(if_resizing){
        pointlist[distance] = event->pos();
        setPolygon(getpolygon());
    }
    else if(If_br_resizing){
        qreal l = getRect().bottomRight().x() - bottom_left.x();
        setTransformOriginPoint(getRect().topLeft());
        setScale(resizing_factor(event->scenePos().x() - bottom_left.x(),l));
        /*qreal l = mapToScene(getRect().bottomRight()).x() -  mapToScene(getRect().bottomLeft()).x();
        setTransformOriginPoint(mapToScene(getRect().topLeft()));
        setScale(resizing_factor(event->scenePos().x() -  mapToScene(getRect().bottomLeft()).x(),l));*/
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
    else if(If_rotating){
        setTransformOriginPoint(center);
        setRotation(rotating(center,top_center,event->scenePos()));
    }
    else{
        QGraphicsItem::mouseMoveEvent(event);
    }
    for(int i = 0;i < pos;i++){
        //pointlist[i] = mapToScene(pointlist[i]);
    }
}

qreal Polygon::rotating(QPointF x, QPointF y, QPointF z){
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

void Polygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(if_resizing){
        pointlist[distance] = event->pos();
        setPolygon(getpolygon());
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

QPainterPath Polygon::shape() const{
    QPainterPath mypath;
    mypath.addRect(getRect());
    return mypath;
}
