#include "openglwidget.h"

openglwidget::openglwidget(QWidget* parent,const char* name):QGLWidget(parent){
    setGeometry(0,0,640,480);
    fullscreen = false;
    if_selected = false;
    if_invert = false;
    translate = -6.0;
    xRot = zRot = 0.0;
    yRot = -30.0;
}

openglwidget::~openglwidget(){}

void openglwidget::initializeGL(){
    glShadeModel(GL_SMOOTH);
    //qDebug()<<"this";
    glClearColor(0.0,0,0.0,0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}

void openglwidget::paintGL(){
    qDebug()<<if_invert;
    if(if_invert){
        glClearColor(1.0,1.0,1.0,0.0);
    }
    else{
        glClearColor(0.0,0,0.0,0.0);
    }
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glTranslatef( 0.0,  0.0, -6.0 );
    glRotatef(xRot,1,0,0);
    glRotatef(yRot,0,1,0);
    glRotatef(zRot,0,0,1);
    if(if_selected == true){
        for(int i = 0; i<mian_num;i++){
            glBegin(GL_POLYGON);
            for(int j = 0 ; j < mians[i].verts; j++){
                int k = (int)points[mians[i].vPoints[j]].x;
                float k1 = points[mians[i].vPoints[j]].x - (float)k;
                int p = (int)points[mians[i].vPoints[j]].y;
                float p1 = points[mians[i].vPoints[j]].y - (float)k;
                int q = (int)points[mians[i].vPoints[j]].z;
                float q1 = points[mians[i].vPoints[j]].z - (float)k;
                glColor3f(k1,p1,q1);
                glVertex3f(points[mians[i].vPoints[j]].x,points[mians[i].vPoints[j]].y,points[mians[i].vPoints[j]].z);
            }
         glEnd();
        }
        glLoadIdentity();
    }
}

void openglwidget::resizeGL(int width,int height){
    if(height == 0)
        height = 1;
    glViewport(0,0,(GLint)width,(GLint)height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void openglwidget::keyPressEvent(QKeyEvent *e){
    switch (e->key()) {
        case Qt::Key_Up:
            xRot += 10;
            break;
        case Qt::Key_Left:
            yRot += 10;
            break;
        case Qt::Key_Right:
            zRot += 10;
            break;
        case Qt::Key_Down:
            translate -= 1;
            break;
        case Qt::Key_Space:
            translate += 1;
            break;
        default:
            break;
        }
        update();
    QGLWidget::keyPressEvent(e);
}

void openglwidget::loadObject(QString filename, OBJECT* k){
    QFile fp(filename);
    fp.open(QIODevice::ReadOnly);
    QTextStream in(&fp);
    QString line = in.readLine();
    if(line!= "OFF")
        qDebug()<<"not off";
    else{
        line = in.readLine();
        sscanf(line.toUtf8(),"%d %d %d",&point_num,&mian_num,&bian_num);
        for(int i = 0;i<point_num;i++){
            line = in.readLine();
            VERTEX vertex;
            sscanf(line.toUtf8(),"%f %f %f",&vertex.x,&vertex.y,&vertex.z);
            points.push_back(vertex);
        }
        for(int i = 0;i<mian_num;i++){
            line = in.readLine();
            QTextStream inLine(&line);
            OBJECT mian;
            inLine>>mian.verts;
            for(int j = 0; j < mian.verts;j++){
                int k;
                inLine>>k;
                mian.vPoints.push_back(k);
            }
            mians.push_back(mian);
        }
    }
}
