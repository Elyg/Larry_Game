#include "Platform.h"


Platform::Platform(ngl::Vec3 _pos, std::string _platName,PlatType _platType, PhysicsWorld *_physics)
{
  // initialize variables
  m_pos = _pos;
  m_platName = _platName;
  m_physics = _physics;
  m_platformDirection = 1;
  m_platType = _platType;
  m_rot = 0;
  m_deg = 0;


  // import mesh as collision shape based on which type is chosen
  if(m_platType == PlatType::MOVE_PLATE)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName,"models/platform_mesh.obj");
    }

  else if(m_platType == PlatType::BOUNCE_PAD)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName,"models/level_1_bounce_platform.obj");
    }

  else if(m_platType == PlatType::BARRIER01)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName,"models/barrier_01.obj");
    }

  else if(m_platType == PlatType::BARRIER_LARGE)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName,"models/barrier_01.obj");
    }
  else if(m_platType == PlatType::ROTATING_L)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName, "models/level_4_rotating_platform.obj");

    }
  else if(m_platType == PlatType::BRIDGE_1)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName, "models/level_4_bridge_half.obj");

    }
  else if(m_platType == PlatType::BRIDGE_2)
    {
      CollisionShape *shapes=CollisionShape::instance();
      shapes->addMesh(m_platName, "models/level_4_bridge_half2.obj");

    }


}

void Platform::createPlatform()
{
  // add the collision shape to the physics world
  m_physics->addPlatform(m_platName,m_pos,1,ngl::Vec3(0,0,0));

}

void Platform::addRot(int _deg)
{
  // adds the specified angle to the total degrees left to turn
  m_deg += _deg;
}

void Platform::setRot(int _rot)
{
  m_rot = _rot;
}

void Platform::rotatePlatform(std::string _axis)
{
  //only change angle if m_deg is not 0
  ngl::Mat4 trans = m_physics->getTransformMatrix(m_platName);

  // if m_deg is negative, decrement m_rot and increment m_deg
  if(m_deg < 0)
    {
      m_deg++;
      m_rot--;
    }
  // otherwise increment m_rot and decrement m_deg
  else if (m_deg > 0)
    {
      m_deg--;
      m_rot++;
    }

  // depending on which axis is specified, set the rotation in the transformation matrix to m_rot
  if(_axis == "x")
    {
      trans.rotateX(m_rot);
    }
  else if(_axis == "y")
    {
      trans.rotateY(m_rot);
    }
  else if(_axis == "z")
    {
      trans.rotateZ(m_rot);
    }

  m_physics->setTransformMatrix(m_platName, trans);
}



void Platform::movePlatform(float _vel, float _range,Direction _d)
{

  // move depending on which Direction is chosen
  if(_d == MOVE_X )
    {
      ngl::Vec3 currentPos = m_physics->getPosition(m_platName);

      // if current position is outside the range, change Direction
      if(currentPos[0] - _range >= m_pos[0])
        {

          m_platformDirection = -1;
        }

      if(currentPos[0] + _range <= m_pos[0])
        {
          m_platformDirection = 1;
        }

      // set the platform's velocity
      ngl::Vec3 platformMove(m_platformDirection*_vel,0,0);
      m_physics->setLinVelocity(m_platName,platformMove);
    }

  else if(_d == MOVE_Y)
    {
      ngl::Vec3 currentPos = m_physics->getPosition(m_platName);

      // if current position is outside the range, change Direction
      if(currentPos[1] - _range >= m_pos[1])
        {
          m_platformDirection = -1;
        }

      if(currentPos[1] + _range <= m_pos[1])
        {
          m_platformDirection = 1;
        }

      // set the platform's velocity
      ngl::Vec3 platformMove(0,m_platformDirection*_vel,0);
      m_physics->setLinVelocity(m_platName,platformMove);
    }
  else if(_d ==MOVE_Z)
    {
      ngl::Vec3 currentPos = m_physics->getPosition(m_platName);

      // if current position is outside the range, change Direction
      if(currentPos[2] - _range >= m_pos[2])
        {
          m_platformDirection = -1;
        }

      if(currentPos[2] + _range <= m_pos[2])
        {
          m_platformDirection = 1;
        }

      // set the platform's velocity
      ngl::Vec3 platformMove(0,0,m_platformDirection*_vel);
      m_physics->setLinVelocity(m_platName,platformMove);
    }

}

ngl::Vec3 Platform::getPlatformPos()
{
  return m_physics->getPosition(m_platName);
}

std::string Platform::getPlatformName()
{
  return m_platName;
}

PlatType Platform::getPlatType()
{
  return m_platType;
}




