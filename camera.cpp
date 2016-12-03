#include "camera.h"
#include "printmatrix.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <tgmath.h>


using namespace cs40;
using std::cout;
using std::endl;

Camera::Camera()
{
    counter = 0;
    // Doesn't Need Anything
}

QMatrix4x4 Camera::lookAts(const vec3& eye, const vec3& at, const vec3& up)
{

    vec3 z_bar = (at - eye);
    vec3 z_negative = (eye - at);
    //vec3 z_hat = z_negative/z_bar.length();
    vec3 x_bar = QVector3D::crossProduct(at-eye, up);
    vec3 u_bar = x_bar/x_bar.length();
    vec3 y_bar = QVector3D::crossProduct(x_bar,z_bar);
    vec3 v_bar = y_bar/y_bar.length();
    vec3 n_bar = z_negative/z_negative.length();

    /* Creation of rotational matrix */
    mat4 rot;
    rot.setRow(0, vec4(u_bar, 0));
    rot.setRow(1, vec4(v_bar, 0));
    rot.setRow(2, vec4(n_bar, 0));
    rot.setRow(3, vec4(0,0,0,1));

    /*Creation of translational matrix */
    mat4 trans;
    trans.translate(-eye);

    /*Combines the two matrices and forms the resultant m_lookAts*/
    m_lookAt = rot*trans;

    /* Stores valid in private method for reset */
    m_hold = m_lookAt;

    return m_lookAt;
}

void Camera::right(float amt)
{
    T.setToIdentity();
    T.translate(amt,0,0);
    m_lookAt = T*m_lookAt;

}

void Camera::up(float amt)
{
    T.setToIdentity();
    T.translate(0,amt,0);
    m_lookAt = T*m_lookAt;

}

void Camera::forward(float amt)
{

    T.setToIdentity();
    T.translate(0,0,-amt);
    m_lookAt = T*m_lookAt;

}

void Camera::roll(float angle)
{
    T.setToIdentity();
    T.rotate(angle, vec3(0,0,1));
    m_lookAt = T*m_lookAt;

}

void Camera::pitch(float angle)
{
    T.setToIdentity();
    T.rotate(angle, vec3(1,0,0));;
    m_lookAt = T*m_lookAt;

}

void Camera::yaw(float angle)
{
    T.setToIdentity();
    T.rotate(angle, vec3(0,1,0));
    m_lookAt = T*m_lookAt;

}

// Resets Camera
void Camera::reset()
{
    m_lookAt = m_hold;
}


//Bounces by a cos counter (sinusoidal movement) while going away from origin.
void Camera::bounce(float amt)
{
    T.setToIdentity();
    counter +=1;
    T.translate(0, (amt/5)*cos(counter),-(amt/2));

    m_lookAt = T*m_lookAt;


}
