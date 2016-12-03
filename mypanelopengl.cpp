#include "mypanelopengl.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "planet.h"
#include "camera.h"
#include "printmatrix.h"

using std::cout;
using std::endl;
using cs40::Sphere;
using cs40::Square;

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QOpenGLWidget(parent), m_angles(0,0.,0.) {

    m_shaderProgram=NULL;
    m_vertexShader=NULL;
    m_fragmentShader=NULL;

    m_sphere = NULL;
    m_square = NULL;
    m_texture = NULL;

    m_timer = NULL;

    m_drawSphere = true;
    m_polymode = 2;
    m_cull = true;

    deltaTime = 0;
    m_scaling = 1;  //Tracks scaling of zoom, so it can pan


    showOptions();
}

MyPanelOpenGL::~MyPanelOpenGL(){
    m_shaderProgram->release();
    if(m_texture){ delete m_texture; m_texture = NULL; }
    if(m_sphere){ delete m_sphere; m_sphere=NULL; }
    if(m_square){ delete m_square; m_square=NULL; }
    if(m_timer) {delete m_timer; m_timer = NULL; }
    destroyShaders();
}

void MyPanelOpenGL::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    createShaders();

    parsePlanets("solarData.txt");
    m_sphere = new Sphere(1,30,30);
    for (int i = 0; i < m_info.size(); i++) {
        m_info[i]->setTexture();
    }

    m_square = new Square(10.);


    //m_view.ortho(-2000,2000,-2000,2000,-2000,2000); /* set up orthographic world projection */
    //m_view.ortho(-200,200,-200,200,-20000,20000); /* set up orthographic world projection */
    //m_view.ortho(-20,20,-20,20,-20,20); /* set up orthographic world projection */


    m_shaderProgram->bind();

    /* Adjustable settings for viewing */
    // vec3 eye = vec3(0,0,600);
    // vec3 at = vec3(0,0,0);
    // vec3 up = vec3(0,1,0);
    vec3 eye = vec3(0,0,600);
    vec3 at = vec3(0,0,0);
    vec3 up = vec3(0,1,0);
    persp_ang = 45;

    //Checking that the lookAt method and our 'lookAts' method return the same matrix.
    // m_camera.lookAt(eye, at, up);
    // cout << m_camera << endl;

    /* Our lookAts */
    m_camera.setToIdentity();
    m_camera = m_cam.lookAts(eye, at, up);
    // cout << m_camera << endl;
    m_view.perspective(persp_ang, 1, 1, 20000);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    m_timer->start(10);

}

void MyPanelOpenGL::resizeGL(int w, int h)
{
    glViewport(0,0,w, h);
}

void MyPanelOpenGL::paintGL(){
    /* clear both color and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updatePolyMode(m_polymode);
    if(m_cull){ glEnable(GL_CULL_FACE); }
    else{glDisable(GL_CULL_FACE); }

    if(!m_shaderProgram){return;}


    m_shaderProgram->bind();
    m_shaderProgram->setUniformValue("Tex0",0);
    m_shaderProgram->setUniformValue("model", m_model);
    //This takes place for the m_projection
    m_view.setToIdentity();
    m_view.perspective(persp_ang, 1, 1, 20000);
    m_shaderProgram->setUniformValue("view",m_view);
    m_shaderProgram->setUniformValue("camera",m_camera);

    // Draw Each Planet
    if(m_drawSphere){
        for(int i = 0; i < m_info.size()-1; i++){
            m_modelStack.push();
            m_modelStack.rotateZ(m_info[i]->getInclination());
            m_modelStack.rotateY(m_info[i]->getRotPos());
            m_modelStack.translate(m_info[i]->getDistToSun(),0,0);


            //Addition of Mercury moon (it seemed lonely)
            if (i == 1) {
                m_modelStack.push();
                m_texture = m_info[10]->getTexture();
                m_texture->bind();
                m_modelStack.rotateY(m_info[10]->getRotPos());
                m_modelStack.translate(m_info[10]->getDistToSun(),0,0);
                m_modelStack.rotateZ(m_info[10]->getInclination());
                m_modelStack.scale(m_info[10]->getRadius());
                m_modelStack.rotateX(-90);
                m_modelStack.rotateY(m_info[10]->getTilt());
                m_modelStack.rotateZ(m_info[10]->getOrbPos());
                m_shaderProgram->setUniformValue("model", m_modelStack.top());
                m_sphere->draw(m_shaderProgram);
                m_modelStack.pop();
            }

            m_modelStack.rotateX(-90);  //Get so we have a side-view of planets
            m_modelStack.rotateY(m_info[i]->getTilt());
            m_modelStack.rotateZ(m_info[i]->getOrbPos());

            m_modelStack.scale(m_info[i]->getRadius());
            if (m_info[i]->getRadius() < 10){
                m_modelStack.scale(3);
            }
            //For easy viewing, specifically cause Sun is too large...
            //Scaled down 35 times to be ~20 units.
            if (i==0){
                m_modelStack.scale(1./35);
            }
            m_shaderProgram->setUniformValue("model", m_modelStack.top());

            // Set Texture
            m_texture = m_info[i]->getTexture();
            m_texture->bind();
            m_sphere->draw(m_shaderProgram);
            m_modelStack.pop();
        }
    }
    else{
      m_square->draw(m_shaderProgram);
    }
    glFlush();
}

void MyPanelOpenGL::showOptions(){

  /* TODO: expand as needed */
  printf("Application controls: \n");
  printf("c: Toggle Culling\n");
  printf("p: Toggle Polygon Mode\n");
  // printf("s: Toggle Sphere/Square Mode\n");
  printf("1: Rotate X\n");
  printf("2: Rotate Y\n");
  printf("3: Rotate Z\n");
  printf("5: Increase DeltaHr\n");
  printf("6: Decrease DeltaHr\n");
  //printf("9: Zoom in (2x)\n");
  //printf("0: Zoom out (2x)\n");
  //printf("Use arrow keys to pan through the viewing frame\n\n");
  printf("w: Pan Up\n");
  printf("s: Pan Down\n");
  printf("a: Pan Left\n");
  printf("d: Pan Right\n");
  printf("q: Move Forward\n");
  printf("e: Move Back\n");
  printf("u: Roll Camera\n");
  printf("i: Pitch Camera\n");
  printf("o: Yaw Camera\n");
  printf("r: Reset Camera\n");
  printf("b: Bounce Camera while going forward\n");
  printf("n: Bounce Camera while going backwards\n");
  printf("z: Increase perspective angle by 1 degree\n");
  printf("x: Decrease perspective angle by 1 degree\n");
}

void MyPanelOpenGL::keyPressEvent(QKeyEvent *event)
{
    // qreal step=2;
    /*Enable strong Focus on GL Widget to process key events*/
    switch(event->key()){
    // case Qt::Key_X:
    //     if (event->text()=="x"){updateAngles(0,step);}
    //     else{updateAngles(0,-step);}
    //     break;
    // case Qt::Key_Y:
    //     if (event->text()=="y"){updateAngles(1,step);}
    //     else{ updateAngles(1,-step);}
    //     break;
    // case Qt::Key_Z:
    //     if (event->text()=="z"){updateAngles(2,step);}
    //     else{updateAngles(2,-step);}
    //     break;
    case Qt::Key_C:
        m_cull = !m_cull;
        break;
    case Qt::Key_P:
        m_polymode = (m_polymode+1)%3;
        break;
    // case Qt::Key_S:
    //     m_drawSphere = !m_drawSphere;
    //     break;

    /* Old ortho commands */
    // case Qt::Key_9:
    //     m_view.scale(2.0);
    //     m_scaling = m_scaling*0.5;
    //     break;
    // case Qt::Key_0:
    //     m_view.scale(0.5);
    //     m_scaling = m_scaling*2;
    //     break;
    //Added keys to pan and zoom
    // case Qt::Key_Up:
    //     m_view.translate(0,10*m_scaling,0);
    //     break;
    // case Qt::Key_Down:
    //     m_view.translate(0,-10*m_scaling,0);
    //     break;
    // case Qt::Key_Left:
    //     m_view.translate(-10*m_scaling,0,0);
    //     break;
    // case Qt::Key_Right:
    //     m_view.translate(10*m_scaling,0,0);
    //     break;
    //Keys to test rotation on x, y, z
    // case Qt::Key_1:
    //     m_modelStack.rotateX(3);
    //     break;
    // case Qt::Key_2:
    //     m_modelStack.rotateY(3);
    //     break;
    // case Qt::Key_3:
    //     m_modelStack.rotateZ(3);
    //     break;
    case Qt::Key_5:
        deltaTime += 1./10;
        break;
    // Increment and Decrement deltaTime
    case Qt::Key_6:
        if (deltaTime > .1)
        {
        deltaTime -= 1./10;
        } else
        {
            deltaTime = 0;
        }
    // case Qt::Key_M:
    //     m_expMerc = !m_expMerc;
    //     break;
        break;
    // /* New Keyboard Perspective Controls: */
    case Qt::Key_W:
        m_cam.up(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_S:
        m_cam.down(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_A:
        m_cam.left(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_D:
        m_cam.right(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_Q:
        m_cam.forward(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_E:
        m_cam.back(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_U:
        m_cam.roll(2);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_I:
        m_cam.pitch(2);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_O:
        m_cam.yaw(2);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_R:
        m_cam.reset();
        m_camera = m_cam.matrix();
        persp_ang = 45;
        break;
    case Qt::Key_B:
        m_cam.bounce(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_N:
        m_cam.bbackboys(10);
        m_camera = m_cam.matrix();
        break;
    case Qt::Key_Z:
        persp_ang += 1;
        break;
    case Qt::Key_X:
        persp_ang -= 1;
        break;

    default:
        QWidget::keyPressEvent(event); /* pass to base class */
    }
    update();
}

void MyPanelOpenGL::parsePlanets(const QString &fname){
    std::ifstream infile;
    std::string name;
    QString qname;
    float rad, a, tau, ecc, incl, day, tilt;
    char buf[1024];
    infile.open(fname.toStdString().c_str());
    //read header lines
    infile.getline(buf,1024);
    cout << "    "  << buf << endl;
    infile.getline(buf,1024);
    cout << "     " << buf << endl;
    name = "";
    while(!infile.eof()){
        infile >> name >> rad >> a >> tau >> ecc >> incl >> day >> tilt;
        if(!infile.eof()){
            qname = QString(name.c_str()).toLower();
            // Add the information of each planet to the m_info, list of planets
            cs40::Planet* newplanet = new cs40::Planet(qname,a,tau,ecc,incl,rad,day,tilt);
            m_info.append(newplanet);

            printf("%10s %8.2f %6.0f %7.2f %5.2f %5.2f %8.2f %8.2f\n",
                   name.c_str(), rad, a, tau, ecc, incl, day, tilt);
            // printf("%5s %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n", m_info.last()->getDistToSun());
        }
    }
    // cs40::Planet* moon = new cs40::Planet("galaxy2", 8, 0.10, 0, 0, 2, 27.3, 15);
    // m_info.append(moon);
    //added moon into solarData
    cout << endl;

    //Doesn't do what we think it does.
    // //Creation of spheres for m_planets.
    // for (int i = 0; i < m_info.size(); i++) {
    //     float rad = m_info[i]->getRadius();
    //     float dist = m_info[i]->getDistToSun();
    //     QString name = m_info[i]->getName();
    // }
}

/* updateTime() calls on move() to update the current orbital and rotational
 * angles, based on the time (deltaTime)
 */
void MyPanelOpenGL::updateTime(){
    //Move All Planets by delHr
    for(int i = 0; i < m_info.size(); i++)
    {
        m_info[i]->move(deltaTime);
        // m_rotpos = m_info[i]->getRotPos();
        // m_orbpos = m_info[i]->getOrbPos();
        // m_modelStack.rotateY(rotpos);
        //m_modelStack.rotateX(orbpos-m_orbpos);
        //m_modelStack.rotateY(1);
    }
    update();
}

void MyPanelOpenGL::updatePolyMode(int val){
    GLenum mode=GL_NONE;
    if(val==0){mode=GL_POINT;}
    else if(val==1){mode=GL_LINE;}
    else if(val==2){mode=GL_FILL;}

    if(mode != GL_NONE){
        glPolygonMode(GL_FRONT_AND_BACK, mode);
    }
}

void MyPanelOpenGL::createShaders(){

    destroyShaders(); //get rid of any old shaders

    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    if (!m_vertexShader->compileSourceFile("shaders/vshader.glsl")){
        qWarning() << m_vertexShader->log();
    }

    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if(!m_fragmentShader->compileSourceFile("shaders/fshader.glsl")){
        qWarning() << m_fragmentShader->log();
    }

    m_shaderProgram = new QOpenGLShaderProgram();
    m_shaderProgram->addShader(m_vertexShader);
    m_shaderProgram->addShader(m_fragmentShader);

    if(!m_shaderProgram->link()){
        qWarning() << m_shaderProgram->log() << endl;
    }
}

void MyPanelOpenGL::destroyShaders(){

    delete m_vertexShader; m_vertexShader=NULL;
    delete m_fragmentShader; m_fragmentShader=NULL;

    if(m_shaderProgram){
        m_shaderProgram->release();
        delete m_shaderProgram; m_shaderProgram=NULL;
    }
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt){
    if(idx == 0){
        m_angles.setX(m_angles.x()+amt);
    }else if(idx == 1){
        m_angles.setY(m_angles.y()+amt);
    }else if(idx == 2){
        m_angles.setZ(m_angles.z()+amt);
    }
}


qreal MyPanelOpenGL::wrap(qreal amt){
    if (amt > 360.){ return amt - 360.; }
    else if (amt < 0.){ return amt + 360.; }
    return amt;
}
