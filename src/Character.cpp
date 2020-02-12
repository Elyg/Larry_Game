#include "Character.h"
#include "PhysicsWorld.h"
#include "Text.h"
#include "Ltimer.h"
#include "Level.h"

Character::Character(PhysicsWorld *_physics)
{
  //initialize the characters attributes
  m_shapeshift = true;
  m_shiftable = true;
  m_mulitplier = 80*2;
  m_mass=1.0f;
  m_maxVelocity =12;
  m_physics = _physics;
  m_initialBallPos = ngl::Vec3(45,3,0);
  m_attractEnable = false;
  m_formOne = "sphere";
  m_formTwo = "larry";
  m_clamp = true;

  //create timer object for jump hops
  LTimer jumpHopTimer;
  jumpHopTimer.start();
  m_jumpForce = 35.0f ;
  m_jumpHopMaxTime = 170;
  m_friction = 0.80;
  m_doubleJump = true;
  m_oldPlatformVelocity = ngl::Vec3(0,0,0);
  m_platformVelocity = ngl::Vec3(0,0,0);
  //create collision shapes
  CollisionShape *shapes=CollisionShape::instance();
  shapes->addSphere(m_formOne,4);
  shapes->addCapsule(m_formTwo, 1, 3.5);

}

Character::~Character(){}

void Character::createCharacter(ngl::Vec3 _pos)
{
  //store the position of the character
  m_initialBallPos = _pos;
  //add to the physics world
  m_physics->addSphere(m_formOne,m_initialBallPos,0.5+m_mass,ngl::Vec3(0,0,0), true);
}

void Character::shapeshift()
{
  if(m_shiftable)
    {
      //shapeshift to 1st char
      if (m_shapeshift == false)
        {
          //store all the physics properties of the second char
          ngl::Vec3 secondCharVelLin = m_physics->getLinVelocity(m_formTwo);
          ngl::Vec3 secondCharVelAng = m_physics->getAngVelocity(m_formTwo);
          ngl::Vec3 secondCharPos = m_physics->getPosition(m_formTwo);
          m_physics->removeCollisionObj(m_formTwo);
          secondCharPos[1] -=2;
          //apply the stored physics values to the first character
          m_physics->addSphere(m_formOne,secondCharPos,0.5f+m_mass,ngl::Vec3(0,0,0),true);
          m_physics->setAngVelocity(m_formOne,secondCharVelAng);
          m_physics->setLinVelocity(m_formOne,secondCharVelLin);
          m_shapeshift = true;
        }
      //shapeshift to 2nd char
      else
        {
          //store all physics of the first character
          ngl::Vec3 firstCharVelLin = m_physics->getLinVelocity(m_formOne);
          ngl::Vec3 firstCharPos = m_physics->getPosition(m_formOne);
          m_physics->removeCollisionObj(m_formOne);
          firstCharPos[1] +=2.2;
          //apply the stored attributes to the second character
          m_physics->addLegSphere(m_formTwo,firstCharPos,0.0+m_mass,ngl::Vec3(0,0,0));
          m_physics->setLinVelocity(m_formTwo,firstCharVelLin);
          m_shapeshift = false;
        }
    }
}

void Character::jumpImpulse()
{
  if (m_shapeshift == false)
    {
      // checks whether the character is on a mesh before jumping
      if ((detectCollisionForJump()) || (jumpHopTimer.getTicks() > 0))
        {
          //set y velocity to 0 before adding impulse
          ngl::Vec3 sphereVel = m_physics->getLinVelocity(m_formTwo);
          ngl::Vec3 sphereVelNoYVel = ngl::Vec3(sphereVel.m_x,0,sphereVel.m_z);
          m_physics->setLinVelocity(m_formTwo,sphereVelNoYVel);
          m_physics->addImpulse(ngl::Vec3(0.0f,m_jumpForce,0.0f),m_formTwo);
        }
    }
}

bool Character::detectCollisionForJump()
{
  // checks if character is standing on a mesh
  if (m_shapeshift ==true)
    {
      ngl::Vec3 spherePos = m_physics->getPosition(m_formOne);
      ngl::Vec3 start = ngl::Vec3(spherePos.m_x,spherePos.m_y,spherePos.m_z);
      ngl::Vec3 end = ngl::Vec3(spherePos.m_x,(spherePos.m_y)-1.5,spherePos.m_z);
      return m_physics->rayTest(start, end);
    }

  if (m_shapeshift ==false)
    {
      ngl::Vec3 applePos = m_physics->getPosition(m_formTwo);
      ngl::Vec3 start = ngl::Vec3(applePos.m_x,applePos.m_y,applePos.m_z);
      ngl::Vec3 end = ngl::Vec3(applePos.m_x,(applePos.m_y)-3.5,applePos.m_z);
      return m_physics->rayTest(start, end);
    }
}

void Character::characterMove(ngl::Vec3 _move)
{
  ngl::Vec4 dir = m_cameraDir;
  dir.m_y=0;
  dir.normalize();

  //adds velocity of the mesh the character is standing on
  if (m_shapeshift ==false)
    {
      ngl::Vec3 applePos = m_physics->getPosition(m_formTwo);
      ngl::Vec3 start = ngl::Vec3(applePos.m_x,applePos.m_y,applePos.m_z);
      ngl::Vec3 end = ngl::Vec3(applePos.m_x,(applePos.m_y)-3.5,applePos.m_z);
      m_platformVelocity = m_physics->getVelocityOfPlatform(start,end);

      ngl::Vec3 velocity = m_physics->getLinVelocity(m_formTwo);
      if (m_platformVelocity != m_oldPlatformVelocity)
      {
          velocity -= m_oldPlatformVelocity;
          velocity += m_platformVelocity;
          m_oldPlatformVelocity = m_platformVelocity;
      }
      m_physics->setLinVelocity(m_formTwo,velocity
                                );
    }

  ngl::Vec3 velocity=0;
  //move character based on the _move direction vector
  //taking the camera's direction into account
  if(_move.m_z < 0)
    {
      ngl::Vec4 tempVel = (dir*m_mulitplier);
      velocity+=ngl::Vec3(tempVel.m_x,0,tempVel.m_z);
    }

  if (_move.m_z > 0)
    {
      ngl::Vec4 tempVel = (dir*m_mulitplier*(-1));
      velocity+=ngl::Vec3(tempVel.m_x,0,tempVel.m_z);
    }

  if (_move.m_x > 0)
    {
      ngl::Vec4 temp = dir.cross(ngl::Vec4(1,1,1,1));
      ngl::Vec4 tempVel = (temp*m_mulitplier);
      velocity+=ngl::Vec3(tempVel.m_x,0,tempVel.m_z);
    }
  if (_move.m_x < 0)
    {
      ngl::Vec4 temp = dir.cross(ngl::Vec4(1,1,1,1));
      ngl::Vec4 tempVel = (temp*m_mulitplier*(-1));
      velocity+=ngl::Vec3(tempVel.m_x,0,tempVel.m_z);
    }

  // if character is standing still and on the platform apply friction to it
  if ((_move.m_x == 0) & (_move.m_z == 0) & (detectCollisionForJump()) & (m_shapeshift ==false))
    {
      ngl::Vec3 velocity = m_physics->getLinVelocity(m_formTwo);
      velocity -= m_platformVelocity;
      velocity *= m_friction;
      velocity += m_platformVelocity;
      m_physics->setLinVelocity(m_formTwo,velocity);
    }

  //ignore the y direction
  velocity.m_y = 0;

  //used for the stickiness direction
  if(m_collisionNormal[0] == 1)
    {
      velocity.m_y = -velocity.m_x;
      velocity.m_x = 0;
    }
  if(m_collisionNormal[2] == 1)
    {
      velocity.m_y = -velocity.m_z;
      velocity.m_z = 0;
    }
  if(m_collisionNormal[0] == -1)
    {
      velocity.m_y = velocity.m_x;
      velocity.m_x = 0;
    }
  if(m_collisionNormal[2] == -1)
    {
      velocity.m_y = velocity.m_z;
      velocity.m_z = 0;
    }

  //clamp the characters' velocity
  if (m_shapeshift == true)
    {
      m_maxVelocity =18;
      if (m_clamp==true)
        {
          clampVel(m_formOne);
        }
      m_physics->applyForceCentral(velocity, m_formOne);
    }
  if (m_shapeshift ==false)
    {
      m_maxVelocity =12+m_platformVelocity.length();
      clampVel(m_formTwo);
      m_physics->applyForceCentral(velocity, m_formTwo);
    }

  // adds an upwards impulse to the character based on jumpHopTimer
  m_jumpHopTime = jumpHopTimer.getTicks();
  if (m_jumpHopTime>m_jumpHopMaxTime)
    {
      jumpHopTimer.stop();
    }
  if (m_jumpHopTime>0)
    jumpImpulse();

  // if character is standing on a mesh cancel double jump
  if (detectCollisionForJump())
    m_doubleJump = true;
}

void Character::startJumpHopTimer()
{
  //jumpHopTimer starts when character jumps from a mesh or if it can double jump
  if (detectCollisionForJump())
    jumpHopTimer.start();
  if (detectCollisionForJump() == false)
    {
      if (m_doubleJump)
        jumpHopTimer.start();
      m_doubleJump = false;
    }
}

void Character::stopJumpHopTimer()
{
  //stops the jump hop timer
  jumpHopTimer.stop();
}

void Character::clampVel(std::string _shape)
{
  //clamp velocity based on string shape
  ngl::Vec3 velocity = m_physics->getLinVelocity(_shape);
  ngl::Vec3 velocityOLD = velocity;
  float x = velocity[0];
  float z = velocity[2];

  ngl::Vec2 velocityXZ = ngl::Vec2(x, z);
  float speed = velocityXZ.length();
  if(speed > m_maxVelocity)
    {
      //velocity slowly decreases for more gradual slow down
      velocity *= 0.95;
    }
  velocity[1]=velocityOLD[1];
  m_physics->setLinVelocity(_shape,velocity);

}

void Character::speed(std::string _shape)
{
  //increases characters velocity
  ngl::Vec3 velocity = m_physics->getLinVelocity(_shape);
  velocity *= 1.04;
  m_physics->setLinVelocity(_shape,velocity);
}


void Character::freezeCharacter()
{
  //starts the counter and freezes the character based on the duration of the counter
  m_freezeTimer.stop();
  m_freezeTimer.reset();
  m_physics->freezeBody(m_formOne);
  m_freezeTimer.start();
  float seconds = m_freezeTimer.getTicks();
  while (seconds<50)
    {
      seconds = m_freezeTimer.getTicks()/10.0f;
    }
  m_freezeTimer.stop();
  m_freezeTimer.reset();

}

void Character::attractForce(float _gravityFactor, float _attractForceStr, std::string _name)
{
  //checks if need to enable
  if(m_attractEnable == true)
    {
      //normal for the positive X direction
      //get the position of the character
      ngl::Vec3 charPos = m_physics->getPosition(_name);
      //create start and end points for the ray
      ngl::Vec3 start = ngl::Vec3(charPos.m_x,charPos.m_y,charPos.m_z);
      ngl::Vec3 end = ngl::Vec3(charPos.m_x-_attractForceStr,charPos.m_y,charPos.m_z);
      ngl::Vec3 normalPosX= getCollisionNormal(start, end);
      //normal for the negative X direction
      start = ngl::Vec3(charPos.m_x,charPos.m_y,charPos.m_z);
      end = ngl::Vec3(charPos.m_x,charPos.m_y,charPos.m_z-_attractForceStr);
      ngl::Vec3 normalNegX = getCollisionNormal(start, end);

      //normal for the positive Z direction
      start = ngl::Vec3(charPos.m_x,charPos.m_y,charPos.m_z);
      end = ngl::Vec3(charPos.m_x +_attractForceStr,charPos.m_y,charPos.m_z);
      ngl::Vec3 normalPosZ = getCollisionNormal(start, end);
      ;
      //normal for the negative Z direction
      start = ngl::Vec3(charPos.m_x,charPos.m_y,charPos.m_z);
      end = ngl::Vec3(charPos.m_x ,charPos.m_y,charPos.m_z + _attractForceStr);
      ngl::Vec3 normalNegZ = getCollisionNormal(start, end);

      //add all the collision normals
      m_collisionNormal = normalPosX+normalNegX+normalPosZ+normalNegZ;
      // check if it collided with anything
      if(m_collisionNormal.length() == 0.0)
        {
          //if not set the gravity to default
          m_physics->setGravityForBody(_name, ngl::Vec3(0, (-9.8*_gravityFactor), 0));
        }
      else
        {
          //if collided set normal to be the gravity for the object
          m_collisionNormal.normalize();
          m_physics->setGravityForBody(_name, m_collisionNormal*9.8*_gravityFactor*(-1));
        }
    }
}

ngl::Vec3 Character::getGravityDir()
{
  //default gravity direction
  if(m_collisionNormal.length() == 0.0)
    {
      return ngl::Vec3(0,-1,0);
    }

  //gravity direction for the stickiness
  else
    {
      return  m_collisionNormal;
    }
}

ngl::Vec3 Character::getCollisionNormal(ngl::Vec3 a, ngl::Vec3 b)
{
  //calls the ray testing to get the normals of the collision object
  return m_physics->rayNormalTest(a, b);
}

void Character::setCameraInfo(ngl::Vec4 &_camVector)
{
  //sets camera info
  m_cameraDir = _camVector;
}

void Character::godMode()
{
  //enables the cheat, by disabling the gravity
  m_physics->setGravityForBody(m_formOne, ngl::Vec3 (0,0,0));
  this->setAttractFlag(false);
}

void Character::setMassPositive()
{
  m_mass +=0.1f;
}

void Character::setMassNegative()
{
  m_mass -=0.1f;
}

void Character::setMaxVelPositive()
{
  m_maxVelocity +=0.5f;
}

void Character::setMaxVelNegative()
{
  m_maxVelocity -=0.5f;;
}

void Character::setMultiplierPositive()
{
  m_mulitplier +=0.5f;
}

void Character::setMultiplierNegative()
{
  m_mulitplier -=0.5f;
}

void Character::setJumpForcePositive()
{
  m_jumpForce +=0.5f;
}

void Character::setJumpForceNegative()
{
  m_jumpForce -=0.5f;
}
void Character::setJumpHopMaxTimePositive()
{
  m_jumpHopMaxTime +=10;
}

void Character::setJumpHopMaxTimeNegative()
{
  m_jumpHopMaxTime -=10;
}

void Character::setFrictionPositive()
{
  m_friction +=0.01;
}

void Character::setFrictionNegative()
{
  m_friction -=0.01;
}

void Character::setGravityForCharacter(std::string _shape, ngl::Vec3 _vec)
{
  m_physics->setGravityForBody(_shape,_vec);
}

void Character::resetCharacter(Camera *_cam, ngl::Vec3 _charPos)
{
  //resets the character values
  if(m_shapeshift==true)
    {
      m_physics->removeCollisionObj(m_formOne);

    }
  if(m_shapeshift==false)
    {
      m_physics->removeCollisionObj(m_formTwo);
      m_shapeshift = true;
    }
  m_mass=1.0;
  m_initialBallPos = _charPos;
  m_physics->addSphere(m_formOne,m_initialBallPos,0.5+m_mass,ngl::Vec3(0,0,0), true);
  _cam->snapPos(m_formOne);
  m_clamp = true;
}

void Character::setAttractFlag(bool _value)
{
  m_attractEnable =_value;
}

void Character::setClampFlag(bool _value)
{
  m_clamp =_value;
}
