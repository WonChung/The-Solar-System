#pragma once

#include <QtWidgets>
#include <QtOpenGL>
#include <QMatrix4x4>
#include "sphere.h"
#include "square.h"
#include "matrixstack.h"
#include "planet.h"
#include "camera.h"



class MyPanelOpenGL : public QOpenGLWidget
{
    Q_OBJECT

    typedef QVector4D point4;
    typedef QVector4D color4;
    typedef QVector3D vec3;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent* event);

public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    virtual ~MyPanelOpenGL();

private:


    /* simple test shapes */
    cs40::Sphere* m_sphere;
    cs40::Square* m_square;

    //Stored QList of Planets, is filled with SolarData and called in
    //PaintGL, describes attributes of planets.
    QList<cs40::Planet*> m_info;

    /* Polygon draw mode
     * 0 : point
     * 1 : line
     * 2 : polygon */
    int m_polymode;
    bool m_cull; /* is Culling enabled? */
    bool m_drawSphere; /* Draw complex sphere scene (true) or simpler test scene (false) */

    /* texture data */
    QOpenGLTexture* m_texture;

    QMatrix4x4 m_model; /* Takes objects coordinates into world coordinates */
    QMatrix4x4 m_view; /* Takes world coordinates into clip coordinates */
    cs40::MatrixStack m_modelStack; /* history of past model matrices */

    //Addition to lab5 when splitting m_view
    QMatrix4x4 m_camera;
    cs40::Camera m_cam;
    int persp_ang;

    /* set this up when you are ready to animate */
    QTimer* m_timer;      /* event timer */

    float m_scaling;  //Keeps track of scaling of zoom for panning

    // Global Variable for Time
    float deltaTime;


    vec3 m_angles; /* Euler angles for rotation */

    /* Shaders and program */
    QOpenGLShader *m_vertexShader;
    QOpenGLShader *m_fragmentShader;
    QOpenGLShaderProgram *m_shaderProgram;

    /* parse planet data make data structure for solar system*/
    void parsePlanets(const QString& fname);

    /* Print message showing keyboard controls */
    void showOptions();

    /* update Polygon draw mode based
     * 0 : point
     * 1 : line
     * 2 : polygon */
    void updatePolyMode(int val);

    void createShaders();
    void destroyShaders();

    /* update Euler angle at index idx by amt
     * idx=0,1,2 -> x,y,z */
    void updateAngles(int idx, qreal amt);

    /* wrap a angle value to the range 0..360*/
    qreal wrap(qreal amt);


signals:

public slots:
    /* trigger update of GL window */
    void updateTime();

};
