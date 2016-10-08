#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    Edge(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

    bool intersectsAt(GLfloat y, GLfloat &x);

    GLfloat x1,
            y1,
            x2,
            y2,
            dx,
            dy,
            m;
};

struct Polygon {
    Polygon();

    void addEdge(Edge Edge);

    vector<Edge> edges;
    GLfloat x_min,
            x_max,
            y_min,
            y_max;
};

class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

    void drawCircle();
    void drawPolygon(int n, float r);
    void drawLine(float p1_x, float p1_y, float p2_x, float p2_y);
    void fillPolygon(int n, float r);

    int n1,
        n2;
    float x1,
          y1,
          x2,
          y2;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // OGLWIDGET_H
