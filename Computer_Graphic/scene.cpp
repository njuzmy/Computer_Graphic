#include "scene.h"
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

Scene::Scene(QObject* parent):QGraphicsScene(parent){
    parent = nullptr;
    if_trigger = true;
    cur_shape = nullptr;

    if_color = false;                //判断是否选择填充
    if_select = false;               //判断是否选择画图形
    if_continue = false;             //判断多边形是否画完
    if_crop = false;                 //判断是否是裁剪

    m_resizing = 1;                  //初始化缩放因子1
    m_rotating = 0;
    color = QColor(Qt::white);
    if(this->views().size()>0)
        this->views().at(0)->setMouseTracking(true);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(if_select){
            if(this->views().size() != 0)
                this->views().at(0)->setMouseTracking(false);         //当画图的时候不用捕捉鼠标移动信息
            switch(currentShape){
            case Shape::Line:{
                Line *line = new Line();
                cur_shape = line;
                addItem(line);
                break;
               }
            case Shape::Rect:{
                Rect *rect = new Rect();
                addItem(rect);
                cur_shape = rect;
                break;
            }
            case Shape::Fill_Rect:{
                Fill_Rect* fill_rect = new Fill_Rect();
                fill_rect->setBrush(QBrush(color));
                addItem(fill_rect);
                cur_shape = fill_rect;
                break;
            }
            case Shape::Polygon:{
                class Polygon *polygon = new (class Polygon)();
                addItem(polygon);
                cur_shape = polygon;
                //polygon->setBrush(color);
                break;
            }
            case Shape::Ellipse:{
                class Ellipse *ellipse = new (class Ellipse)();
                addItem(ellipse);
                cur_shape = ellipse;
                break;
            }
            case Shape::Curve:{
                class Curve *curve = new Curve();
                addItem(curve);
                cur_shape = curve;
                break;
            }
            }
            if(cur_shape && cur_shape->name != "polygon" && cur_shape->name != "curve"){
                cur_shape->startDraw(event);
                cur_shape->setStart(event->scenePos());
                cur_shape->setEnd(event->scenePos());
                if_trigger = false;
                if_select = false;
            }
            else if(cur_shape->name == "polygon"){
                cur_shape->setStart(event->scenePos());
                if_trigger = false;
                if_select = false;
                if_continue = true;
            }
            else if(cur_shape->name == "curve"){
                cur_shape->setStart(event->scenePos());
                if_trigger = false;
                if_select = false;
                if_continue = true;
            }
    }

    else if(if_continue){
        if(!if_color){
            if(cur_shape->name == "polygon"){
                class Polygon *polygon = dynamic_cast<class Polygon*>(cur_shape);
                if(sqrt(pow(event->scenePos().x() - cur_shape->getEnd().x(),2) + pow(event->scenePos().y() - cur_shape->getEnd().y(),2)) > 10){
                     if_continue = false;
                     if_trigger = true;
                     cur_shape = nullptr;
                     polygon->drawing(event);
                     polygon->set_true();
                }
                else{
                    if_trigger = false;                                            //可以获得鼠标位置   所以为了使多边形可编辑，在画完多边形后，必须在空白部分点一下，不能直接点控件按钮
                }
            }
            else if(cur_shape->name == "curve"){
                class Curve *curve = dynamic_cast<Curve*>(cur_shape);
                if(sqrt(pow(event->scenePos().x() - cur_shape->getEnd().x(),2) + pow(event->scenePos().y() - cur_shape->getEnd().y(),2)) > 10){
                    //qDebug()<<"here";
                     if_continue = false;
                     if_trigger = true;
                     cur_shape = nullptr;
                     curve->drawing(event);
                     curve->set_true();
                }
                else{
                    if_trigger = false;
                }
            }
        }
    }

    else if(if_crop){
        topleft = event->scenePos();
        addItem(rect);
        rect->setStart(topleft);
        if_trigger = false;
    }

    else{
        if(if_color){
            if(items(event->scenePos()).size()!=0){                         //鼠标点击位置上有图形时
                QGraphicsItem *item = items(event->scenePos())[0];          //将鼠标点击的图像设置填充背景
                if(item->type() == QGraphicsRectItem::Type){
                    Rect* rect = dynamic_cast<Rect*>(item);
                    rect->setBrush(color);
                    rect->drawing(event);
                }
                else if(item->type() == QGraphicsItem::UserType + 1){
                    Fill_Rect* fill_rect = dynamic_cast<Fill_Rect*>(item);
                    fill_rect->setBrush(color);
                    fill_rect->drawing(event);
                }
                else if(item->type() == QGraphicsLineItem::Type){
                    Line* line = dynamic_cast<Line*>(item);
                    line->setPen(color);
                    line->drawing(event);
                }
                else if(item->type() == QGraphicsPolygonItem::Type){
                    class Polygon *polygon = dynamic_cast<class Polygon*>(item);
                    polygon->setBrush(color);
                    polygon->drawing(event);
                }
                else if(item->type() == QGraphicsEllipseItem::Type){
                    class Ellipse *ellipse = dynamic_cast<class Ellipse*>(item);
                    ellipse->setBrush(color);
                    ellipse->drawing(event);
                }
            }
            else
                setBackgroundBrush(QBrush(color));
        }
        cur_shape = nullptr;
        if_color = false;
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(cur_shape&& cur_shape->name != "polygon"&& cur_shape->name != "curve" && !if_trigger){
        cur_shape->drawing(event);
        cur_shape->setEnd(event->scenePos());
    }
    else if(cur_shape&& cur_shape->name == "polygon" && cur_shape->name != "curve" && !if_trigger){                 //如果多边形，在鼠标移动的时候不停的改变端点值
        class Polygon *polygon = dynamic_cast<class Polygon*>(cur_shape);
        polygon->set_false();
        polygon->drawing(event);
        polygon->setPoint1(event->scenePos());
    }
    else if(cur_shape&& cur_shape->name != "polygon" && cur_shape->name == "curve" && !if_trigger){
        //qDebug()<<"here";
        class Curve *curve = dynamic_cast<Curve*>(cur_shape);
        curve->set_false();
        curve->drawing(event);
        curve->setPoint1(event->scenePos());
    }
    else if(if_crop && !if_trigger){
        bottomright = event->scenePos();
        rect->setEnd(bottomright);
        rect->drawing(event);
    }
    else{
        if(items(event->scenePos()).size() == 0 && this->views().size()>0)             //防止一开始没有view时做判断
            this->views().at(0)->setCursor(Qt::ArrowCursor);
        QGraphicsScene::mouseMoveEvent(event);
    }
    if(this->views().size()>0)
        this->views().at(0)->setMouseTracking(true);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
        if(cur_shape && cur_shape->name != "polygon" && cur_shape->name != "curve"){
            cur_shape->setEnd(event->scenePos());
            if_trigger = true;
        }
        else if(cur_shape && cur_shape->name == "polygon"){
            class Polygon *polygon = dynamic_cast<class Polygon*>(cur_shape);
            polygon->setPoint(event->scenePos());
            polygon->setEnd(event->scenePos());
            if_trigger = true;
        }
        else if(cur_shape && cur_shape->name == "curve"){
            class Curve *curve = dynamic_cast<Curve*>(cur_shape);
            curve->setPoint(event->scenePos());
            curve->setEnd(event->scenePos());
            if_trigger = true;
        }
        else if(if_crop){
            bottomright = event->scenePos();
            removeItem(rect);
            if_crop = false;
            if_trigger = true;
            QPoint tl = this->views().at(0)->mapFromScene(topleft);
            QPoint br = this->views().at(0)->mapFromScene(bottomright);
            QPixmap pixmap = this->views().at(0)->grab(QRect(QPoint(tl.x(),tl.y()),QPoint(br.x(),br.y())));
            Mypixmap* crop = new Mypixmap(pixmap,topleft,bottomright);//new QGraphicsPixmapItem(pixmap);
            crop->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
            crop->setPos(topleft.x(),topleft.y());
            addItem(crop);
        }
        QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::zoom(QGraphicsItem* item,qreal scaleFactor){
    /*qreal factor = item->transform().scale(scaleFactor,scaleFactor).mapRect(QRectF(0,0,1,1)).width();
    if(factor < 0.07 || factor > 100)
        return;*/
    item->setTransformOriginPoint(item->boundingRect().center());
    item->setScale(scaleFactor);
}

void Scene::rotate(QGraphicsItem* item){
    //qDebug()<<"here";
    item->setTransformOriginPoint(item->boundingRect().center());
    item->setRotation(m_rotating);
}

void Scene::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Backspace){
        foreach(QGraphicsItem* item,selectedItems()){
                removeItem(item);
        }
    }
    else if(event->key() == Qt::Key_Plus){
        m_resizing += 0.02;
        foreach(QGraphicsItem* item,selectedItems()){
            zoom(item,m_resizing);
        }
    }
    else if(event->key() == Qt::Key_Minus){
        m_resizing -= 0.02;
        foreach(QGraphicsItem* item,selectedItems()){
            zoom(item,m_resizing);
        }
    }
}

void Scene::save(){
    QPixmap pixmap = this->views().at(0)->grab(QRect(QPoint(0,0),QPoint(this->views().at(0)->width() - 20,this->views().at(0)->height() - 20)));
    QString filename1 = QFileDialog::getSaveFileName(this->views().at(0),tr("Save Image"),"",tr("Images(*.png *.bmp *.jpg"));
    pixmap.save(filename1);
}

void Scene::open(){
    QString filename = QFileDialog::getOpenFileName(this->views().at(0),tr("选择图像"),"",tr("Images(*.png *.bmp *.jpg)"));
    if(filename.isEmpty())
        return;
    else{
        QPixmap pixmap;
        if(!(pixmap.load(filename))){
            QMessageBox::information(nullptr,tr("打开图像失败"),tr("打开图像失败！"));
            return;
        }
        Mypixmap* background = new Mypixmap(QPixmap(filename));
        background->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
        background->setScale(0.5);
        background->setPos(0,0);
        addItem(background);
        //QPainter painter( this->views().at(0));
        //painter.drawPixmap(QRect(QPoint(0,0),QPoint(this->width(),this->height())),pixmap);

    }
}

void Scene::newfile(){
    foreach(QGraphicsItem* item,items())
        removeItem(item);
}

View::View(QGraphicsScene *scene, QWidget *parent):QGraphicsView(scene,parent){
    this->setMouseTracking(true);
}

//void View::mousePressEvent(QMouseEvent *event){
    //QGraphicsView:mousePressEvent(event);
//}

/*void View::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Plus){
        qDebug()<<"here";
        scale(2,2);
        setSceneRect(0,0,width()*2, height()*2);
    }
}*/

