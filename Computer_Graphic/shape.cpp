#include "shape.h"
#include <QGraphicsView>

Shape::Shape(){}

QRectF Shape::getRect() const{
    QRectF rect1;
    if(start.x()<end.x() && start.y()<end.y()){
        rect1 = QRectF(start.x(),start.y(),end.x()-start.x(),end.y()-start.y());
    }
    else if(start.x()>= end.x() && start.y()<end.y()){
        rect1 = QRectF(end.x(),start.y(),start.x()-end.x(),end.y()-start.y());
    }
    else if(start.x() < end.x() && start.y() >= end.y()){
        rect1 = QRectF(start.x(),end.y(),end.x()-start.x(),start.y()-end.y());
    }
    else{
        rect1 = QRectF(end.x(),end.y(),start.x()-end.x(),start.y()-end.y());
    }
    return rect1;
}


qreal Abs(qreal x){
    if(x > 0)
        return x;
    else
        return -x;
}

void Shape::drawLine(QPainter* painter,QPointF start, QPointF end){
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(Qt::black, 2));
    qreal dx = (start.x() - end.x());
    qreal dy = (start.y() - end.y());
    qreal k = dy/dx;
    if(Abs(k) > 1){
        if(start.y()>end.y()){

            qreal x = end.x();
            for(qreal i = end.y();i<start.y();i++){
                painter->drawPoint(QPointF(x,i));
                x += 1/k;
            }
        }
        else{
            qreal x = start.x();
            for(qreal i = start.y();i<end.y();i++){
                painter->drawPoint(QPointF(x,i));
                x += 1/k;
            }
        }
    }
    else{
        if(start.x()>end.x()){
            qreal y = end.y();
            for(qreal i = end.x();i<start.x();i++){
                painter->drawPoint(QPointF(i,y));
                y += k;
            }
        }
        else{
            qreal y = start.y();
            for(qreal i = start.x();i<end.x();i++){
                painter->drawPoint(QPointF(i,y));
                y += k;
            }
        }
    }
}

void Shape::drawRect(QPainter* painter,QPointF start, QPointF end){
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(Qt::black, 2));
    QPointF top_right = QPointF(start.x(),end.y());
    QPointF bottom_left = QPointF(end.x(),start.y());
    drawLine(painter,start,top_right);
    drawLine(painter,start,bottom_left);
    drawLine(painter,top_right,end);
    drawLine(painter,bottom_left,end);
}

void Shape::drawdashRect(QPainter* painter,QPointF start, QPointF end){
    //painter->setRenderHint(QPainter::Antialiasing,true);
    //painter->setPen(QPen(Qt::black, 2,Qt::DashLine));
    //qDebug()<<"here";
    QPointF top_right = QPointF(start.x(),end.y());
    QPointF bottom_left = QPointF(end.x(),start.y());
    drawLine(painter,start,top_right);
    drawLine(painter,start,bottom_left);
    drawLine(painter,top_right,end);
    drawLine(painter,bottom_left,end);
}

void Shape::drawPoligon(QPainter* painter,QPointF* pointlist, int num, qreal factor){
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(Qt::black, 2));
    for(int i = 0; i<num;i++){
        drawLine(painter,pointlist[i],pointlist[i+1]);
    }
    drawLine(painter,pointlist[num],pointlist[0]);
}

void Shape::drawfill_Poligin(QPainter *painter, QPointF* pointlist, int num){
        if(num >= 2){
        qreal maxy = pointlist[0].y();
        qreal miny = maxy;

        for(int i = 0; i<=num;i++){
            //qDebug()<<pointlist[i].y();
            if(pointlist[i].y() > maxy)
                maxy = pointlist[i].y();
            if(pointlist[i].y() < miny)
                miny = pointlist[i].y();
        }
        painter->setPen(painter->brush().color());

        int height = maxy;
        Edge *AET;
        Edge *ET[height];
        for(int i = 0;i<=maxy;i++){
            ET[i] = new Edge;
            ET[i]->next = nullptr;
        }                                //边表一开始为空
        AET = new Edge;
        AET->next = nullptr;
        for(int i = 0; i<=num;i++){
            qreal x1 = pointlist[i].x();
            qreal x2 = pointlist[(i+1) % (num + 1)].x();
            qreal y0 = pointlist[(i + num) % (num + 1)].y();
            qreal y1 = pointlist[i].y();
            qreal y2 = pointlist[(i+1) % (num + 1)].y();
            qreal y3 = pointlist[(i+2)%(num + 1)].y();
            qreal ymin = y1 > y2 ? y2:y1;
            qreal ymax = y1 > y2 ? y1:y2;
            if((int)ymin == (int)ymax)
                continue;
            qreal x = y1>y2?x2:x1;
            qreal dx = (x1 - x2)/(y1 - y2);
            if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3))){ //奇点
                   ymin++;
                   x += dx;
            }
            Edge *p = new Edge;
            p->ymax = ymax;
            p->k = dx;
            p->x = x;
            p->next = ET[(int)ymin]->next;
            ET[(int)ymin]->next = p;
        }
        for(int i = 0; i < maxy;i++){
            while(ET[i]->next){
                Edge *pInsert = ET[i]->next;
                Edge *p = AET;
                while(p->next){
                    if(pInsert->x > p->next->x){
                        p = p->next;
                        continue;
                    }
                    if((int)pInsert->x == (int)p->next->x && pInsert->k > p->next->k){
                        p = p->next;
                        continue;
                    }
                    break;
                }
                ET[i]->next = pInsert->next;
                pInsert->next = p->next;
                p->next = pInsert;           //将pInsert插入到AET活动边表中
            }
            //qDebug()<<"here3";
            Edge *p = AET;
            while(p->next && p->next->next){
                for(qreal x = p->next->x + 1;x<p->next->next->x;x++){
                    painter->drawPoint(QPointF(x,i));
                }
                p = p->next->next;
            }
            //qDebug()<<"here4";
            p = AET;
            while(p->next){
                if((int)p->next->ymax == i){
                    Edge *pdelete = p->next;
                    p->next = pdelete->next;
                    pdelete->next = nullptr;
                    delete pdelete;
                }
                else
                    p = p->next;
            }
            //qDebug()<<"here5";
            p = AET;
            while(p->next){
                p->next->x += p->next->k;
                p = p->next;
            }
            //qDebug()<<"here6";
        }
    }
}

void Shape::drawEllipse(QPainter* painter, QPointF start, QPointF end){
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(Qt::black, 2));
    qreal x0 = (start.x() + end.x())/2;
    qreal y0 = (start.y() + end.y())/2;
    qreal a = Abs(start.x() - end.x())/2;
    qreal b = Abs(start.y() - end.y())/2;

    qreal sqa = a*a;
    qreal sqb = b*b;
    qreal d = sqb + sqa*(0.25 - b);
    int x = 0;
        qreal y = b;
        painter->drawPoint(QPointF((x0 + x), (y0 + y)));
        painter->drawPoint(QPointF((x0 + x), (y0 - y)));
        painter->drawPoint(QPointF((x0 - x), (y0 - y)));
        painter->drawPoint(QPointF((x0 - x), (y0 + y)));
        while (sqb*(x + 1) < sqa*(y - 0.5))
        {
            if (d < 0)
                d += sqb*(2 * x + 3);
            else
            {
                d += (sqb*(2 * x + 3) + sqa*((-2)*y + 2));
                --y;
            }
            ++x;
            painter->drawPoint(QPointF((x0 + x), (y0 + y)));
            painter->drawPoint(QPointF((x0 + x), (y0 - y)));
            painter->drawPoint(QPointF((x0 - x), (y0 - y)));
            painter->drawPoint(QPointF((x0 - x), (y0 + y)));
        }
        d = (b * (x + 0.5)) * 2 + (a * (y - 1)) * 2 - (a * b) * 2;
        while (y > 0)
        {
            if (d < 0)
            {
                d += sqb * (2 * x + 2) + sqa * ((-2) * y + 3);
                ++x;
            }
            else
                d += sqa * ((-2) * y + 3);
            --y;
            painter->drawPoint(QPointF((x0 + x), (y0 + y)));
            painter->drawPoint(QPointF((x0 + x), (y0 - y)));
            painter->drawPoint(QPointF((x0 - x), (y0 - y)));
            painter->drawPoint(QPointF((x0 - x), (y0 + y)));
        }
}

void Shape::drawfill_rect(QPainter* painter,QPointF start,QPointF end){
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(Qt::black, 2));
    qreal minx = (start.x() < end.x())?start.x():end.x();
    qreal maxx = (start.x() < end.x())?end.x():start.x();
    qreal miny = (start.y() < end.y())?start.y():end.y();
    qreal maxy = (start.y() < end.y())?end.y():start.y();
    for(qreal i = minx + 1; i < maxx; i++)
        for(qreal j = miny + 1; j < maxy; j++){
            painter->setPen(painter->brush().color());
            painter->drawPoint(i,j);
        }
}

void Shape::drawCurve(QPainter* painter,QPointF* pointlist, int num){
    QPainterPath path(pointlist[0]);
    for(int i = 0; i < num - 1; i++){
        QPointF sp = pointlist[i];
        QPointF ep = pointlist[i + 1];
        QPointF c1 = QPointF((sp.x() + ep.x())/2,sp.y());
        QPointF c2 = QPointF((sp.x() + ep.x())/2,ep.y());
        path.cubicTo(c1,c2,ep);
    }

    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawPath(path);
}
