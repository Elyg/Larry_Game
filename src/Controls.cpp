#include "Controls.h"

Controls::Controls(PhysicsWorld *_physics,
                   Character *_character)
{
  //stores the pointers for character and the physics world
  m_physics = _physics;
  m_character = _character;
}
//-------------------------------------------------------------------------
void Controls::jumpImpulse()
{
  m_character->jumpImpulse();
}
//-------------------------------------------------------------------------
void Controls::stopJumpHopTimer()
{
  m_character->stopJumpHopTimer();
}
//-------------------------------------------------------------------------
void Controls::startJumpHopTimer()
{
  m_character->startJumpHopTimer();
}
//-------------------------------------------------------------------------
void Controls::godMode()
{
  m_character->godMode();
}
//-------------------------------------------------------------------------
void Controls::setMassPositive()
{
  m_character->setMassPositive();
}
//-------------------------------------------------------------------------
void Controls::setMassNegative()
{
  m_character->setMassNegative();
}
//-------------------------------------------------------------------------
void Controls::setMaxVelPositive()
{
  m_character->setMaxVelPositive();
}
//-------------------------------------------------------------------------
void Controls::setMaxVelNegative()
{
  m_character->setMaxVelNegative();
}
//-------------------------------------------------------------------------
void Controls::setMultiplierPositive()
{
  m_character->setMultiplierPositive();
}
//-------------------------------------------------------------------------
void Controls::setMultiplierNegative()
{
  m_character->setMultiplierNegative();
}
//-------------------------------------------------------------------------
void Controls::characterMovement(ngl::Vec3 &_move)
{
  m_character->characterMove(_move);
}
//-------------------------------------------------------------------------
float Controls::getMass()
{
  return m_character->getMass();
}
//-------------------------------------------------------------------------
float Controls::getMaxVelocity()
{
  return m_character->getMaxVelocity();
}
//-------------------------------------------------------------------------
float Controls::getMultiplier()
{
  return m_character->getMultiplier();
}
//-------------------------------------------------------------------------
void Controls::shapeshift()
{
  m_character->shapeshift();
}
//-------------------------------------------------------------------------
void Controls::setJumpForcePositive()
{
  m_character->setJumpForcePositive();
}
//-------------------------------------------------------------------------
void Controls::setJumpForceNegative()
{
  m_character->setJumpForceNegative();
}
//-------------------------------------------------------------------------
void Controls::setJumpHopMaxTimePositive()
{
  m_character->setJumpHopMaxTimePositive();
}
//-------------------------------------------------------------------------
void Controls::setJumpHopMaxTimeNegative()
{
  m_character->setJumpHopMaxTimeNegative();
}
//-------------------------------------------------------------------------
float Controls::getJumpForce()
{
  return m_character->getJumpForce();
}
//-------------------------------------------------------------------------
int Controls::getJumpHopMaxTime()
{
  return m_character->getJumpHopMaxTime();
}
//-------------------------------------------------------------------------
void Controls::setFrictionPositive()
{
  m_character->setFrictionPositive();
}
//-------------------------------------------------------------------------
void Controls::setFrictionNegative()
{
  m_character->setFrictionNegative();
}
//-------------------------------------------------------------------------
float Controls::getFriction()
{
  return m_character->getFriction();
}
//-------------------------------------------------------------------------
float Controls::getJumpHopTime()
{
  return m_character->getJumpHopTime();
}
