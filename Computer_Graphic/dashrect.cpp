#include "shape.h"
#include <QGraphicsScene>
#include <QGraphicsView>

Dashrect::Dashrect(){
    name = "dashrect";
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void Dashrect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget){
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    if(option->state & QStyle::State_Selected)
        op.state = QStyle::State_None;


    painter->setPen(QPen(color,2,Qt::DashLine));
    painter->drawRect(start.x(),start.y(),end.x() - start.x(),end.y() - start.y());
   // drawdashRect(painter,start,end);
}

void Dashrect::startDraw(QGraphicsSceneMouseEvent* event){
    setRect(QRectF(event->scenePos(),QSizeF(0,0)));
}
void Dashrect::drawing(QGraphicsSceneMouseEvent* event){
    setRect(getRect());
}

void Dashrect::setStart(QPointF p){
    start = end = p;
}
