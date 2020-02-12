#include "Camera.h"
#include "ngl/Util.h"
#include <ctime>

Camera::Camera(PhysicsWorld *_physics)
{
  m_camDistance = 15.0*2;
  m_camAngle = 0;
  m_camHeight = 50;
  m_physics = _physics;


  ngl::Vec3 from(1,0,0);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  // now load to our new camera
  m_camera = new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_camera->setShape(60,(float)720.0/576.0,0.05,350);

  m_tempDistance = m_camDistance;
}

Camera::~Camera()
{
  delete m_camera;
}

void Camera::updatePos(std::string _name)
{

  //calculate the camera's position based on a horizontal circle around the target
  m_camVel = m_physics->getLinVelocity(_name);
  ngl::Vec4 centre(m_physics->getPosition(_name));
  ngl::Vec4 result(centre.m_x + m_tempDistance * cos(m_camAngle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(m_camAngle)),0);
  ngl::Vec3 a = centre.toVec3();
  ngl::Vec4 dir(result - centre);
  ngl::Vec3 p_out;
  ngl::Vec4 eye = m_physics->getPosition("camSphere");
  ngl::Vec3 p(eye.m_x, eye.m_y - 2 ,eye.m_z);
  ngl::Vec3 vel = m_camVel;

  // Ray test to see if the camera can see the target
  if(m_physics->rayTest(a, p, p_out))
    {
      // If not, bring the camera to a point above the target
      if(m_tempDistance >= 1.3)
        {
          m_tempDistance -= 0.1*vel.length();
          m_camHeight += 0.2*vel.length();
          result = ngl::Vec4(centre.m_x + m_tempDistance * cos(m_camAngle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(m_camAngle)),0);
          dir = ngl::Vec4(result - centre);
        }

    }
  else
    {
      // If there is a line of sight, move the camera back to the usual position.
      // Temporary result is calculated but not set yet
      ngl::Vec4 tempResult(centre.m_x + (m_tempDistance+0.1*vel.length()) * cos(m_camAngle),centre.m_y+m_camHeight-0.1*vel.length(),(centre.m_z + (m_tempDistance+0.1) * sin(m_camAngle)),0);
      p = ngl::Vec3(tempResult.m_x, tempResult.m_y - 2 ,tempResult.m_z);

      // if there are no obstructions between the temporary result and the object,
      // set it as the actual result and change the distance and height values.
      if(m_physics->rayTest(a, p) == false)
        {
          if(m_tempDistance < m_camDistance)
            {
              m_tempDistance += 0.1*vel.length();
            }

          result = ngl::Vec3(tempResult.m_x,tempResult.m_y+2,tempResult.m_z);
          dir = ngl::Vec4(result - centre);
          if(m_camHeight > 10)
            {
              m_camHeight -= 0.1*vel.length();
            }

        }

    }

  // set the camera's ideal position
  m_camIdealPos = result.toVec3();

  // accelerate the camera's collision sphere towards the ideal point
  ngl::Vec3 velocity = m_camIdealPos - m_physics->getPosition("camSphere");
  velocity *= 4;
  m_physics->setLinVelocity("camSphere",velocity);

  // set the eye inside the collision sphere
  eye = m_physics->getPosition("camSphere");
  m_camera ->setEye(eye);

  // Set the focus position and add the velocity to compensate
  m_camera ->setLook(centre);
  m_camera ->moveLook(m_camVel[0]/60.0f,m_camVel[1]/60.0f,m_camVel[2]/60.0f);
}

void Camera::snapPos(std::string _name)
{
  //calculate the camera's position as usual
  m_camVel = m_physics->getLinVelocity(_name);
  m_camAngle = 0;
  ngl::Vec4 centre(m_physics->getPosition(_name));
  ngl::Vec4 result(centre.m_x + m_tempDistance * cos(m_camAngle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(m_camAngle)),0);
  ngl::Vec4 dir(result - centre);
  ngl::Vec4 eye = m_physics->getPosition("camSphere");

  dir.normalize();
  dir.m_y=0;
  m_camIdealPos = result.toVec3();

  //recreate the camera's collision sphere to hard reset its position
  m_physics->removeCollisionObj("camSphere");
  m_physics->addSphere("camSphere", result.toVec3(), 0.5, ngl::Vec3(0,0,0), false);
  m_physics->setGravityForBody("camSphere", ngl::Vec3(0,0,0));

  //set eye and look as before
  eye = m_physics->getPosition("camSphere");
  m_camera ->setEye(eye);
  m_camera ->setLook(centre);
  m_camera ->moveLook(m_camVel[0]/60.0f,m_camVel[1]/60.0f,m_camVel[2]/60.0f);
}

void Camera::snapPos(std::string _name, float _angle)
{

  //calculate the camera's position as usual
  m_camVel = m_physics->getLinVelocity(_name);

  //set camera angle to specified value
  m_camAngle = _angle;
  ngl::Vec4 centre(m_physics->getPosition(_name));
  ngl::Vec4 result(centre.m_x + m_tempDistance * cos(m_camAngle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(m_camAngle)),0);
  ngl::Vec4 dir(result - centre);
  ngl::Vec4 eye = m_physics->getPosition("camSphere");

  dir.normalize();
  dir.m_y=0;
  m_camIdealPos = result.toVec3();
  m_physics->removeCollisionObj("camSphere");

  m_physics->addSphere("camSphere", result.toVec3(), 0.5, ngl::Vec3(0,0,0), false);
  m_physics->setGravityForBody("camSphere", ngl::Vec3(0,0,0));
  eye = m_physics->getPosition("camSphere");
  m_camera ->setEye(eye);
  m_camera ->setLook(centre);
  m_camera ->moveLook(m_camVel[0]/60.0f,m_camVel[1]/60.0f,m_camVel[2]/60.0f);
}

void Camera::forcePos(std::string _name, float _angle)
{
  // As updatePos, except:
  //set angle based on input _angle
  m_camAngle = _angle;

  m_camVel = m_physics->getLinVelocity(_name);
  ngl::Vec4 centre(m_physics->getPosition(_name));
  ngl::Vec4 result(centre.m_x + m_tempDistance * cos(m_camAngle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(m_camAngle)),0);
  ngl::Vec3 a = centre.toVec3();
  ngl::Vec4 dir(result - centre);
  ngl::Vec3 p_out;
  ngl::Vec4 eye = m_physics->getPosition("camSphere");
  ngl::Vec3 p(eye.m_x, eye.m_y - 2 ,eye.m_z);
  ngl::Vec3 vel = m_camVel;

  //line of sight test
  if(m_physics->rayTest(a, p, p_out))
    {
      if(m_tempDistance >= 1.3)
        {
          m_tempDistance -= 0.1*vel.length();
          m_camHeight += 0.1*vel.length();
          result = ngl::Vec4(centre.m_x + m_tempDistance * cos(_angle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(_angle)),0);
          dir = ngl::Vec4(result - centre);
        }

    }
  else
    {
      ngl::Vec4 tempResult(centre.m_x + (m_tempDistance+0.1*vel.length()) * cos(_angle),centre.m_y+m_camHeight-0.1*vel.length(),(centre.m_z + (m_tempDistance+0.1) * sin(_angle)),0);
      p = ngl::Vec3(tempResult.m_x, tempResult.m_y - 2 ,tempResult.m_z);
      if(m_physics->rayTest(a, p) == false)
        {
          if(m_tempDistance < m_camDistance)
            {
              m_tempDistance += 0.1*vel.length();
            }

          result = ngl::Vec3(tempResult.m_x,tempResult.m_y+2,tempResult.m_z);
          dir = ngl::Vec4(result - centre);
          if(m_camHeight > 10)
            {
              m_camHeight -= 0.1*vel.length();
            }

        }

    }


  dir.normalize();
  dir.m_y=0;
  m_camIdealPos = result.toVec3();
  ngl::Vec3 velocity = m_camIdealPos - m_physics->getPosition("camSphere");
  velocity *= 4;
  m_physics->setLinVelocity("camSphere",velocity);
  eye = m_physics->getPosition("camSphere");
  m_camera ->setEye(eye);
  m_camera ->setLook(centre);
  m_camera ->moveLook(m_camVel[0]/60.0f,m_camVel[1]/60.0f,m_camVel[2]/60.0f);
}

void Camera::forcePos(std::string _name, float _angle, ngl::Vec3 _focus)
{


  // As updatePos, except:
  // set centre and angle based on input parameters
  ngl::Vec4 centre(_focus.m_x, _focus.m_y, _focus.m_z, 0);
  m_camAngle = _angle;

  m_camVel = m_physics->getLinVelocity(_name);
  ngl::Vec4 result(centre.m_x + m_tempDistance * cos(m_camAngle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(m_camAngle)),0);
  ngl::Vec3 a = centre.toVec3();
  ngl::Vec4 dir(result - centre);
  ngl::Vec3 p_out;
  ngl::Vec4 eye = m_physics->getPosition("camSphere");
  ngl::Vec3 p(eye.m_x, eye.m_y - 2 ,eye.m_z);
  ngl::Vec3 vel = m_camVel;

  //line of sight test
  if(m_physics->rayTest(a, p, p_out))
    {

      //time_t t = time(0);   // get time now
      //    struct tm * now = localtime( & t );
      //std::cout<<"No line of sight, time = "<< now->tm_hour << ':' << now->tm_min << ':'<<  now->tm_sec <<std::endl;
      if(m_tempDistance >= 1.3)
        {
          m_tempDistance -= 0.1*vel.length();
          m_camHeight += 0.1*vel.length();
          result = ngl::Vec4(centre.m_x + m_tempDistance * cos(_angle),centre.m_y+m_camHeight,(centre.m_z + m_tempDistance * sin(_angle)),0);
          dir = ngl::Vec4(result - centre);
        }

    }
  else
    {
      ngl::Vec4 tempResult(centre.m_x + (m_tempDistance+0.1*vel.length()) * cos(_angle),centre.m_y+m_camHeight-0.1*vel.length(),(centre.m_z + (m_tempDistance+0.1) * sin(_angle)),0);
      p = ngl::Vec3(tempResult.m_x, tempResult.m_y - 2 ,tempResult.m_z);
      if(m_physics->rayTest(a, p) == false)
        {
          if(m_tempDistance < m_camDistance)
            {
              m_tempDistance += 0.1*vel.length();
            }

          result = ngl::Vec3(tempResult.m_x,tempResult.m_y+2,tempResult.m_z);
          dir = ngl::Vec4(result - centre);
          if(m_camHeight > 10)
            {
              m_camHeight -= 0.1*vel.length();
            }

        }

    }


  dir.normalize();
  dir.m_y=0;
  m_camIdealPos = result.toVec3();
  ngl::Vec3 velocity = m_camIdealPos - m_physics->getPosition("camSphere");
  velocity *= 4;
  m_physics->setLinVelocity("camSphere",velocity);
  eye = m_physics->getPosition("camSphere");
  m_camera ->setEye(eye);
  m_camera ->setLook(centre);
  m_camera ->moveLook(m_camVel[0]/60.0f,m_camVel[1]/60.0f,m_camVel[2]/60.0f);
}

ngl::Vec4 Camera::getEye()
{
  return m_camera->getEye();
}

void Camera::setEye(ngl::Vec4 _eye)
{
  return m_camera->setEye(_eye);
}

ngl::Vec4 Camera::getLook()
{
  return m_camera->getLook();
}

void Camera::setLook(ngl::Vec4 _look)
{
  return m_camera->setLook(_look);
}

ngl::Mat4 Camera::getViewMatrix()
{
  return m_camera->getViewMatrix();
}

ngl::Mat4 Camera::getVPMatrix()
{
  return m_camera->getVPMatrix();
}

ngl::Mat4 Camera::getProjectionMatrix()
{
  return m_camera->getProjectionMatrix();
}

float Camera::getDistance()
{
  return m_tempDistance;
}

void Camera::setDistance(float _distance)
{
  m_tempDistance = _distance;
}

float Camera::getAngle()
{
  return m_camAngle;
}

void Camera::setAngle(float _angle)
{
  m_camAngle = _angle;
}

ngl::Vec3 Camera::getIdealPos()
{
  return m_camIdealPos;
}

void Camera::setIdealPos(ngl::Vec3 _pos)
{
  m_camIdealPos = _pos;
}

void Camera::setShape(int _angle, float _aspect, float _near, float _far)
{
  m_camera->setShape(_angle,_aspect,_near,_far);
}
