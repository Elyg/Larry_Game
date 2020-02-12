#include "Trigger.h"

Trigger::Trigger(std::string _id, ngl::Vec3 _pos1, ngl::Vec3 _pos2, PhysicsWorld * _physics)
{

  // set the id
  m_triggerID = _id;


  // make sure that m_pos1 has all the smallest values
  if(_pos1.m_x < _pos2.m_x)
    {
      m_pos1.m_x = _pos1.m_x;
      m_pos2.m_x = _pos2.m_x;
    }
  else
    {
      m_pos1.m_x = _pos2.m_x;
      m_pos2.m_x = _pos1.m_x;
    }

  if(_pos1.m_y < _pos2.m_y)
    {
      m_pos1.m_y = _pos1.m_y;
      m_pos2.m_y = _pos2.m_y;
    }
  else
    {
      m_pos1.m_y=_pos2.m_y;
      m_pos2.m_y=_pos1.m_y;
    }

  if(_pos1.m_z < _pos2.m_z)
    {
      m_pos1.m_z = _pos1.m_z;
      m_pos2.m_z = _pos2.m_z;
    }
  else
    {
      m_pos1.m_z = _pos2.m_z;
      m_pos2.m_z = _pos1.m_z;
    }

  // store the pointer to physics
  m_physics = _physics;
}
Trigger::~Trigger()
{
  //delete m_physics;
}
bool Trigger::isHit(std::string _name)
{
  //get the position for the specified physics body
  ngl::Vec3 pos = m_physics->getCentreOfMass(_name);

  //test to see if it is inside the box's bounds
  bool largerThanPos1 = pos.m_x > m_pos1.m_x
      && pos.m_y > m_pos1.m_y
      && pos.m_z > m_pos1.m_z;

  bool smallerThanPos2 = pos.m_x < m_pos2.m_x
      && pos.m_y < m_pos2.m_y
      && pos.m_z < m_pos2.m_z;

  if(largerThanPos1 && smallerThanPos2)
    {
      return true;
    }
  else return false;
}

ngl::Transformation Trigger::getTransform()
{
  //calculate centre point
  ngl::Vec3 centrePoint = m_pos1 + (m_pos2 - m_pos1)/2;
  float x = m_pos2.m_x - m_pos1.m_x;
  float y = m_pos2.m_y - m_pos1.m_y;
  float z = m_pos2.m_z - m_pos1.m_z;

  //set scale to the dimensions of the box
  ngl::Transformation out;
  out.setPosition(centrePoint.m_x, centrePoint.m_y, centrePoint.m_z);
  out.setScale(x, y, z);

  return out;
}


