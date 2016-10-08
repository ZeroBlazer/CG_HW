#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget *parent)
{

}

OGLWidget::~OGLWidget()
{

}

void OGLWidget::drawCircle()
{
    glColor3f(0.0, 0.5, 0.0);
    glBegin(GL_POINTS);
        for(int i=0;i<1000;++i) {
            glVertex3f(cos(2*3.14159*i/1000.0),sin(2*3.14159*i/1000.0),0);
        }
    glEnd();
}

void OGLWidget::drawPolygon(int n, float r)
{
    glColor3f(0.0, 0.0, 1.0);
    float   step = 2 * 3.14159 / n,
            it_ang = 0.0,
            p0_x = 0.0,//r * cos(it_ang),
            p0_y = r,//r * sin(it_ang),
            pN_x = 0.0,
            pN_y = 0.0;

    glBegin(GL_LINES);
        for(int i=0; i<n; ++i) {
            it_ang += step;
            pN_x = r * sin(it_ang);
            pN_y = r * cos(it_ang);
            glVertex2f(p0_x, p0_y);
            glVertex2f(pN_x, pN_y);
            p0_x = pN_x;
            p0_y = pN_y;
        }
    glEnd();
}

void OGLWidget::drawLine(float p1_x, float p1_y, float p2_x, float p2_y)
{
//    int a = p2_y - p1_y,
//        b = p1_x - p2_x,
//        c = p1_y * -1 * b;
//    float b_2 = b / 2.0,
//          F = 0.0;
//    GLfloat x = p1_x,
//            y = p1_y,
//            inc = 0.00001;

    GLfloat x = p1_x,
            y = p1_y,
            dx = p2_x - p1_x,
            dy = p2_y - p1_x,
            inc_E = 2 * dy,
            inc_NE = 2 * (dy - dx),
            d = 2*dy - dx,
            inc_x = 0.00001,
            inc_y = dy * inc_x / dx;

    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POINTS);
        while(x < p2_x) {
            if (d <= 0) {
                d = d + inc_E;
                x = x + inc_x;
            }
            else {
                d = d + inc_NE;
                x = x + inc_x;
                y = y + inc_y;
            }

            glVertex2f(x, y);
//            F = (a*x + b*y + c) + a + b_2;
//            y += F > 0 ? inc : 0.0;
//            x += inc;
        }

    glEnd();
    glFlush();

//    //glClear(GL_COLOR_BUFFER_BIT);
//    glColor3f(1,1,1);
////     glLoadIdentity();
//    GLfloat m=(p2_y-p1_y)/(p2_x-p1_x);
//    cout<<"esto es m"<<m<<p1_x<<p1_y<<p2_x<<p2_y<<endl;
//    glBegin(GL_POINTS);
//    GLfloat x,y;
//    for (x=p1_x; x<p2_x; x+=0.000001)
//    {
//       y=(p1_y+m*(x-p1_x));
//       glVertex2f(x,y);
//    }
//    glEnd();
    //    glFlush();
}

void OGLWidget::fillPolygon(int n, float r)
{
    float   step = 2 * 3.14159 / n,
            it_ang = 0.0,
            p0_x = 0.0,
            p0_y = r,
            pN_x = 0.0,
            pN_y = 0.0;

    Polygon pol;

    for(int i=0; i<n; ++i) {
        it_ang += step;
        pN_x = r * sin(it_ang);
        pN_y = r * cos(it_ang);
        pol.addEdge(Edge(p0_x, p0_y, pN_x, pN_y));
        p0_x = pN_x;
        p0_y = pN_y;
    }

    glColor3f(0.3, 0.3, 0.5);
    glBegin(GL_POINTS);
//        for(GLfloat y = pol.y_min; y < pol.y_max; y+=0.001) {
        for(GLfloat y = pol.y_max; y > pol.y_min; y-=0.001) {
            bool parity = true;
            GLfloat x;
            vector<GLfloat> int_at_x;
            for(int i = 0; i < n; ++i)
                if(pol.edges[i].intersectsAt(y, x))// && x > pol.x_min && x < pol.x_max)
                    int_at_x.push_back(x);
            int current = 0;
            std::sort(int_at_x.begin(), int_at_x.end());
            for(GLfloat x = pol.x_min; x < pol.x_max; x+=0.001) {
                if(current >= int_at_x.size())
                    break;
                if(x > int_at_x[current]) {
                    parity = !parity;
                    ++current;
                }
                if(!parity)
                    glVertex2f(x, y);
            }
        }
    glEnd();
    glFlush();
}

void OGLWidget::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5,0,0,0,0,1,0);
}

void OGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/////TRIANGLE
//    glBegin(GL_TRIANGLES);
//        glColor3f(1.0, 0.0, 0.0);
//        glVertex3f(-0.5, -0.5, 0);
//        glColor3f(0.0, 1.0, 0.0);
//        glVertex3f( 0.5, -0.5, 0);
//        glColor3f(0.0, 0.0, 1.0);
//        glVertex3f( 0.0,  0.5, 0);
//    glEnd();

///SIMPLE LINE
//    glLineWidth(2.5);
//    glColor3f(1.0, 0.0, 0.0);
//    glBegin(GL_LINES);
//    glVertex3f(0.0, 0.0, 0.0);
//    glVertex3f(15, 0, 0);
//    glEnd();

    drawCircle();
    drawPolygon(7, 1.01);
//    drawLine(0, 0.5, 1, 1.5);
    fillPolygon(7, 1);

//    Edge A(0, 1, 0.8, 0.2);
//    GLfloat x;
//    A.intersectsAt(0.5, x);
//    cout << "x: " << x << endl;
}

Edge::Edge(GLfloat _x1, GLfloat _y1, GLfloat _x2, GLfloat _y2)
{
    x1 = _x1 < _x2 ? _x1 : _x2;
    y1 = _x1 < _x2 ? _y1 : _y2;
    x2 = _x1 < _x2 ? _x2 : _x1;
    y2 = _x1 < _x2 ? _y2 : _y1;
    dx = _x2 - _x1;
    dy = _y2 - _y1;
    m = dx / dy;
}

bool Edge::intersectsAt(GLfloat y, GLfloat &x)
{
    if(m > 0) {
        if(y < y1 || y > y2)
            return false;
    }
    else {
        if(y < y2 || y > y1)
            return false;
    }

    GLfloat _dy = y - y1;
    x = _dy * m + x1;
    return true;

//    if(y2 > y1) {
//        if(y > y1 && y < y2) {
//            GLfloat _dy = y - y1;
//            x = _dy * m + (x1 < x2 ? x1 : x2);
//            return true;
//        } else if (y > y2 && y < y1) {
//            GLfloat _dy = y - y2;
//            x = _dy * m + (x1 > x2 ? x2 : x1);
//            return true;
//        }
//    }

//    if(y2 < y1) {
//        if(y > y1 && y < y2) {
//            GLfloat _dy = y - y1;
//            x = _dy * m + (x1 < x2 ? x2 : x1);
//            return true;
//        } else if (y > y2 && y < y1) {
//            GLfloat _dy = y - y2;
//            x = _dy * m + (x1 < x2 ? x2 : x1);
//            return true;
//        }
//    }

    return false;

//    if(y > y1 && y < y2) {
//        GLfloat _dy = y - y1;
//        x = _dy * m + (x1 < x2 ? x1 : x2);
//        return true;
//    } else if (y > y2 && y < y1) {
//        GLfloat _dy = y - y2;
//        x = _dy * m + (x1 < x2 ? x1 : x2);
//        return true;
//    } else
//        return false;
}

Polygon::Polygon() :
    x_min(0.0),
    x_max(0.0),
    y_min(0.0),
    y_max(0.0)
{}

void Polygon::addEdge(Edge edge)
{
    if(edge.x1 < x_min)
        x_min = edge.x1;
    else if(edge.x1 > x_max)
        x_max = edge.x1;

    if(edge.x2 < x_min)
        x_min = edge.x2;
    else if(edge.x2 > x_max)
        x_max = edge.x2;

    if(edge.y1 < y_min)
        y_min = edge.y1;
    else if(edge.y1 > y_max)
        y_max = edge.y1;

    if(edge.y2 < y_min)
        y_min = edge.y2;
    else if(edge.y2 > y_max)
        y_max = edge.y2;

    edges.push_back(edge);
}
