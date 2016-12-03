#include "planet.h"
#include <cmath>

using namespace cs40;

Planet::Planet(QString name, float o_rad, float o_tau, float ecc,
               float inc, float rad, float r_tau, float tilt):
    m_name(name), m_orb_radius(o_rad), m_orb_tau(o_tau), m_texture(NULL),
    m_eccentricity(ecc), m_inclination(inc), m_radius(rad),
    m_rot_tau(r_tau), m_axial_tilt(tilt),
    m_has_texture(false), m_theta(0), m_phi(0)
{
    m_phi = 0;
    m_theta = 0;
   /* TODO: Maybe there is something to do here
    *  Maybe there isn't */
}

/* move() increments the current orbital and rotational angles,
 * moving the planet in respect to delHR
 * Note: m_phi and m_theta are in degrees.
 */
void Planet::move(float delHr){
    // int planetCircumference = 2*(m_radius)*M_PI;
	// int planetAngularVelocity = planetCircumference/delHr;
    // int planetVelocity = planetAngularVelocity*m_radius;
    // m_phi += (planetVelocity*delHr)/m_orb_radius;
    m_phi += 360*(delHr/(m_orb_tau*365));
    wrap(m_phi);
	// int solarCircumference = 2*(m_orb_radius)*M_PI;
    // int solarAngularVelocity = solarCircumference/delHr;
    // int solarVelocity = solarAngularVelocity*m_radius;
    // m_theta += (solarVelocity*delHr)/m_radius;
    // 360 (delHR/tau)
    m_theta += 360*(delHr/m_rot_tau);
    wrap(m_theta);
}

float Planet::wrap(float val){
    while(val > 180){val -= 360;}
    while(val < -180){val += 360;}
    return val;
}

/* setTexture() sets the texture and sets the m_has_texture flag to true
 */
void Planet::setTexture(){
  if(m_texture){
    delete m_texture;
  }
  m_texture = new QOpenGLTexture(QImage("data/" + m_name + ".png").mirrored());
  m_texture->bind();
  m_has_texture = true;
}
