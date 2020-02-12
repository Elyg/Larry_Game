#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include <ngl/Obj.h>


#define BIT(x) (1<<(x))

enum collisiontypes
{
  COL_NOTHING = 0,   //<Collide with nothing
  COL_BALL = BIT(0), //<Collide with ball
  COL_MESH = BIT(1), //<Collide with mesh
  COL_ENVI = BIT(2)  //<Collide with environment

};


PhysicsWorld::PhysicsWorld(bool _debug)
{
  ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
  m_collisionConfiguration.reset(  new btDefaultCollisionConfiguration());

  ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
  m_dispatcher.reset( new	btCollisionDispatcher(m_collisionConfiguration.get()));

  ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
  m_overlappingPairCache.reset( new btDbvtBroadphase());

  ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
  m_solver.reset(new btSequentialImpulseConstraintSolver);

  m_dynamicsWorld.reset(new btDiscreteDynamicsWorld(m_dispatcher.get(),
                                                    m_overlappingPairCache.get(),
                                                    m_solver.get(),
                                                    m_collisionConfiguration.get()));
  //enable debug drawing
  if (_debug ==true)
  {
      debugDrawer.reset( new BulletDebugDraw);
      debugDrawer->setDebugMode(BulletDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
      m_dynamicsWorld->setDebugDrawer(debugDrawer.get());
    }





}
void PhysicsWorld::addSphere(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia, bool _draw)
{
  //create a dynamic rigidbody

  btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);

  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();

  btScalar	mass(_mass);


  btVector3 localInertia(_inertia.m_x,_inertia.m_y,_inertia.m_z);
  colShape->calculateLocalInertia(mass,localInertia);
  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
  rbInfo.m_additionalAngularDampingFactor=0.0;
  rbInfo.m_additionalDamping=false;
   btRigidBody* body = new btRigidBody(rbInfo);
   // on/off debug drawing
   if(_draw != true)
     {
       body->setCollisionFlags(body->getCollisionFlags() | 32);
     }
  body->setFriction(1.0);
  body->setRestitution(0.4f);
  body->setRollingFriction(0.4f);
  body->forceActivationState(4);
  //body->setIgnoreCollisionCheck();

  m_dynamicsWorld->addRigidBody(body,COL_BALL,COL_ENVI);
  Body b;
  b.name=_shapeName;
  b.body=body;
  m_bodies.push_back(b);

}

void PhysicsWorld::addLegSphere(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia)
{
  //create a dynamic rigidbody

  btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);

  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();

  btScalar	mass(_mass);


  btVector3 localInertia(_inertia.m_x,_inertia.m_y,_inertia.m_z);
  colShape->calculateLocalInertia(mass,localInertia);
  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
  rbInfo.m_additionalAngularDampingFactor=0.0;
  rbInfo.m_additionalDamping=true;
  btRigidBody* body = new btRigidBody(rbInfo);
  //body->setLinearFactor(btVector3(0,1,0));
  body->setAngularFactor(btVector3(0,0,0));
  body->setFriction(0.2);
  body->setRestitution(0.0f);
  body->setRollingFriction(1.0f);
  body->forceActivationState(4);
  //body->setIgnoreCollisionCheck();

  m_dynamicsWorld->addRigidBody(body,COL_BALL,COL_ENVI);
  Body b;
  b.name=_shapeName;
  b.body=body;
  m_bodies.push_back(b);

}


void PhysicsWorld::addLevelMesh(std::string _shapeName,const ngl::Vec3 &_pos)
{
  //create a dynamic rigidbody
  btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);
  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();
  btScalar	mass(0.0);
  btVector3 localInertia(0,0,0);
  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);
  body->setFriction(1.0);
  body->setRestitution(1.0f);
  body->setRollingFriction(1.0f);
  m_dynamicsWorld->addRigidBody(body,COL_ENVI, COL_MESH | COL_BALL | COL_ENVI);
  Body b;
  b.name=_shapeName;
  b.body=body;
  m_bodies.push_back(b);
}
void PhysicsWorld::addMesh(std::string _shapeName, const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia)
{
  //create a dynamic rigidbody
  btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);
  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();
  btScalar	mass(_mass);

  btVector3 localInertia(_inertia.m_x,_inertia.m_y,_inertia.m_z);
  colShape->calculateLocalInertia(mass,localInertia);
  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);
  rbInfo.m_additionalAngularDampingFactor=4.0;
  rbInfo.m_additionalDamping=true;
  body->setFriction(1.0);
  body->setRestitution(0.1f);
  body->setRollingFriction(0.06f);
  body->forceActivationState(4);
  m_dynamicsWorld->addRigidBody(body,COL_MESH,COL_ENVI);
  Body b;
  b.name=_shapeName;
  b.body=body;
  m_bodies.push_back(b);

}

void PhysicsWorld::addBox(std::string _shapeName,const ngl::Vec3 &_pos)
{

  btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);



  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();

  btScalar	mass(2.f);


  btVector3 localInertia(20,120,120);
  colShape->calculateLocalInertia(mass,localInertia);
  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
  rbInfo.m_restitution = 0.1f;
  rbInfo.m_friction = 100.5f;
  rbInfo.m_additionalAngularDampingFactor=4.0;
  rbInfo.m_additionalDamping=true;
  btRigidBody* body = new btRigidBody(rbInfo);
  m_dynamicsWorld->addRigidBody(body,COL_ENVI, COL_MESH | COL_BALL | COL_ENVI);
  Body b;
  b.name=_shapeName;
  b.body=body;
  m_bodies.push_back(b);

}

PhysicsWorld::~PhysicsWorld()
{
}


void PhysicsWorld::addPlatform(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia)
{
  //create a dynamic rigidbody
  btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);
  /// Create Dynamic Objects
  btTransform startTransform;
  startTransform.setIdentity();
  btScalar	mass(_mass);

  btVector3 localInertia(_inertia.m_x,_inertia.m_y,_inertia.m_z);
  colShape->calculateLocalInertia(mass,localInertia);
  startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);

  // now set linear and angular factor of body to only allow movement on th xy plane, so that gravity doesn't effect the body's motion.
  // setting linearFactor to 1,0,0 only allows movement along x-axis
  body->setLinearFactor(btVector3(1,0,0));
  body->setAngularFactor(btVector3(0,0,0));

  body->setFriction(1.0);
  body->setRollingFriction(0.06f);
  body->setRestitution(0.5f);
  body->forceActivationState(4);

  // set the collision of the rigid body to react with the ball and the environment
  m_dynamicsWorld->addRigidBody(body,COL_ENVI, COL_MESH | COL_BALL | COL_ENVI);
  Body b;
  b.name=_shapeName;
  b.body=body;
  m_bodies.push_back(b);

}

void PhysicsWorld::step(float _timeStep, float _maxSubSteps, float _fixedTimeStep)
{
  //timeStep < maxSubSteps * fixedTimeStep
  m_dynamicsWorld->stepSimulation(_timeStep, _maxSubSteps, _fixedTimeStep);
}

ngl::Mat4 PhysicsWorld::getTransformMatrix(unsigned int _index)
{
  //get transformatiion matrix of object at index
  float matrix[16];
  btTransform trans;
  trans = m_bodies[_index].body->getCenterOfMassTransform();
  trans.getOpenGLMatrix(matrix);
  
  return ngl::Mat4( matrix[0],matrix[1],matrix[2],matrix[3],
      matrix[4],matrix[5],matrix[6],matrix[7],
      matrix[8],matrix[9],matrix[10],matrix[11],
      matrix[12],matrix[13],matrix[14],matrix[15]
      );

}



ngl::Mat4 PhysicsWorld::getTransformMatrix(std::string _shape)
{
  int k=0;
  bool found = false;
  float matrix[16];
  //search the collision object list and return the Mat4 based on the string name
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          btTransform trans;
          trans = m_bodies[k].body->getCenterOfMassTransform();
          trans.getOpenGLMatrix(matrix);
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"Did not find transformation matrix for shape: "+ _shape<<std::endl;
      std::cout<<"No such object"<<std::endl;
      return ngl::Mat4();
    }
  else
    {
      return ngl::Mat4( matrix[0],matrix[1],matrix[2],matrix[3],
          matrix[4],matrix[5],matrix[6],matrix[7],
          matrix[8],matrix[9],matrix[10],matrix[11],
          matrix[12],matrix[13],matrix[14],matrix[15]
          );
    }


}

void PhysicsWorld::setTransformMatrix(std::string _shape, ngl::Mat4 _mat)
{
  int k=0;
  bool found = false;
  btTransform trans;

  trans.setFromOpenGLMatrix(_mat.openGL());
 //search the collision object list and set the Mat4 on the string name
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"No such object"<<std::endl;
    }
  else
    {
      m_bodies[k].body->setCenterOfMassTransform(trans);
    }
}

ngl::Vec3 PhysicsWorld::getPosition(std::string _shape){
  int k=0;
  ngl::Vec3 newPos (0,0,0);
  bool found = false;
  //search the collision object list and return the position of the object
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          btVector3 tempPos = m_bodies[k].body->getCenterOfMassPosition();
          newPos[0] = tempPos.getX();
          newPos[1] = tempPos.getY();
          newPos[2] = tempPos.getZ();
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"Did not find position for shape: "+ _shape<<std::endl;
      std::cout<<"No such object"<<std::endl;
      return newPos;
    }
  else
    {
      return newPos;
    }

}
ngl::Vec3 PhysicsWorld::getLinVelocity(std::string _shape)
{
  int k=0;
  ngl::Vec3 newVelocity (0.0,0.0,0.0);
  bool found = false;
  //search the collision object list and return the linera velocity
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          btVector3 tempVel = m_bodies[k].body->getLinearVelocity();
          newVelocity[0] = tempVel.getX();
          newVelocity[1] = tempVel.getY();
          newVelocity[2] = tempVel.getZ();
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"Did not find velocity for shape: "+ _shape<<std::endl;
      std::cout<<"No such object"<<std::endl;
      return newVelocity;
    }
  else
    {
      return newVelocity;
    }

}


void PhysicsWorld::setAngVelocity(std::string _shape, ngl::Vec3 &_velocity)
{
  int k=0;
  btVector3 newVelocity;
  bool found = false;
  //search the collision object list and set the angular velocity
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          newVelocity.setX(_velocity.m_x);
          newVelocity.setY(_velocity.m_y);
          newVelocity.setZ(_velocity.m_z);
          m_bodies[k].body->setAngularVelocity(newVelocity);
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"Was not able to set angular velocity for shape: "+ _shape<<std::endl;
      std::cout<<"No such object"<<std::endl;
    }
}

ngl::Vec3 PhysicsWorld::getAngVelocity(std::string _shape)
{
  int k=0;
  ngl::Vec3 newVelocity;
  bool found = false;
  //search the collision object list and return the angular velocity
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          btVector3 tempVel = m_bodies[k].body->getAngularVelocity();
          newVelocity[0] = tempVel.getX();
          newVelocity[1] = tempVel.getY();
          newVelocity[2] = tempVel.getZ();
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"Did not find angular velocity for shape: "+ _shape<<std::endl;
      std::cout<<"No such object"<<std::endl;
      return newVelocity;
    }
  else
    {
      return newVelocity;
    }
}

void PhysicsWorld::setLinVelocity(std::string _shape, ngl::Vec3 &_velocity)
{
  int k=0;
  btVector3 newVelocity;
  bool found = false;
  //search the collision object list and set the linear velocity
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          newVelocity.setX(_velocity.m_x);
          newVelocity.setY(_velocity.m_y);
          newVelocity.setZ(_velocity.m_z);
          m_bodies[k].body->setLinearVelocity(newVelocity);
          found = true;
          break;
        }
      k++;
    }
  if(found == false)
    {
      std::cout<<"Was no able to set linear vel for shape: "+ _shape<<std::endl;
      std::cout<<"No such object"<<std::endl;
    }
}
void PhysicsWorld::reset()
{
  // start at 1 to leave the ground plane
  //not currently used
  for(unsigned int i=1; i<m_bodies.size(); ++i)
    {
      m_dynamicsWorld->removeRigidBody(m_bodies[i].body);
    }
  m_bodies.erase(m_bodies.begin()+1,m_bodies.end());

}
void PhysicsWorld::printBodies()
{
  int k =0;
  //prints current dynamic objects in the game
  for (auto i : m_bodies ){
      std::cout <<k<<". "<< i.name<<std::endl;
      k++;
    }
}
void PhysicsWorld::addImpulse(const ngl::Vec3 &_i, std::string _shape)
{
  int k=0;
  //search the collision object add apply an impulse
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          m_bodies[k].body->applyCentralImpulse(btVector3(_i.m_x,_i.m_y,_i.m_z));
          break;
        }
      k++;
    }

}



void PhysicsWorld::applyForceCentral(const ngl::Vec3 &_p, std::string _shape)
{
  int k=0;
  //search the collision object add apply a force
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          m_bodies[k].body->applyCentralForce(btVector3(_p.m_x,_p.m_y,_p.m_z));
          break;
        }
      k++;
    }

}
ngl::Vec3 PhysicsWorld::getCentreOfMass(std::string _shape)
{
  int k=0;
  btVector3 tempCenterPos;
  //search the collision object and get the centre of mass
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          tempCenterPos= m_bodies[k].body->getCenterOfMassPosition();
          break;
        }
      k++;
    }
  ngl::Vec3 centerPos;
  centerPos.set(tempCenterPos.getX(),tempCenterPos.getY(),tempCenterPos.getZ());
  return centerPos;
}

void PhysicsWorld::removeCollisionObj(std::string _shape)
{
  int k=0;
  //removes the collision object from the game and the game
  for (auto i : m_bodies ){
      if(i.name == _shape){

          m_dynamicsWorld->removeRigidBody(m_bodies[k].body);
          break;
        }
      k++;
    }
  m_bodies.erase(m_bodies.begin()+k);
}

void PhysicsWorld::debugWorldDraw()
{
  //updated the debug draw
  m_dynamicsWorld->debugDrawWorld();
  debugDrawer->drawUpdate();
}

bool PhysicsWorld::rayTest(const ngl::Vec3 &_a, const ngl::Vec3 &_b)
{
  // from raytest bullet wiki
  btVector3 a = btVector3(_a.m_x,_a.m_y,_a.m_z);
  btVector3 b = btVector3(_b.m_x,_b.m_y,_b.m_z);

  btCollisionWorld::ClosestRayResultCallback RayCallback(a, b);

  // Perform raycast
  m_dynamicsWorld->rayTest(a, b, RayCallback);


  if(RayCallback.hasHit())
    {
      return true;
    }
  else
    {
      return false;
    }
}

ngl::Vec3 PhysicsWorld::getVelocityOfPlatform(const ngl::Vec3 &_a, const ngl::Vec3 &_b)
{
  // from raytest bullet wiki
  btVector3 a = btVector3(_a.m_x,_a.m_y,_a.m_z);
  btVector3 b = btVector3(_b.m_x,_b.m_y,_b.m_z);

  btCollisionWorld::ClosestRayResultCallback RayCallback(a, b);

  // Perform raycast
  m_dynamicsWorld->rayTest(a, b, RayCallback);

  if(RayCallback.hasHit())
    {

        //find body matching RayCallback's collisionObject then get it's velocity
        int k = 0;
        for(auto i : m_bodies)
        {
            if(i.body == btRigidBody::upcast(RayCallback.m_collisionObject))
            {
                break;
            }
            k++;
        }
      return getLinVelocity(m_bodies[k].name);

    }
  else
    {
      return ngl::Vec3(0,0,0);
    }
}

bool PhysicsWorld::rayTest(const ngl::Vec3 &_a, const ngl::Vec3 &_b, ngl::Vec3 &_out)
{
  // from raytest bullet wiki
  btVector3 a = btVector3(_a.m_x,_a.m_y,_a.m_z);
  btVector3 b = btVector3(_b.m_x,_b.m_y,_b.m_z);

  btCollisionWorld::ClosestRayResultCallback RayCallback(a, b);

  // Perform raycast
  m_dynamicsWorld->rayTest(a, b, RayCallback);


  if(RayCallback.hasHit())
    {
      return true;
      _out = ngl::Vec3(RayCallback.m_hitPointWorld.getX(),RayCallback.m_hitPointWorld.getY(),RayCallback.m_hitPointWorld.getZ());
    }
  else
    {
      return false;
    }
}

void PhysicsWorld::setGravityForBody(std::string _shape, ngl::Vec3 _vec)
{
  int k=0;
  //search the dynamic body list add change it's gravity
  for (auto i : m_bodies )
    {
      if(i.name == _shape)
        {
          m_bodies[k].body->setGravity(btVector3(_vec.m_x,_vec.m_y,_vec.m_z));
          break;
        }
      k++;
    }
}


void PhysicsWorld::freezeBody(std::string _shape)
{
    int k=0;
    //freezes the collision object
    for (auto i : m_bodies )
    {
        if(i.name == _shape)
        {
            m_bodies[k].body-> setMassProps(0, btVector3(0, 0, 0));
            m_bodies[k].body-> setLinearVelocity(btVector3(0, 0, 0));
            m_bodies[k].body-> setAngularVelocity(btVector3(0, 0, 0));

            break;
        }
        k++;
    }

}

btRigidBody *PhysicsWorld::FindGameObject(btRigidBody* _shape)
{
    // function checks whether specific shape is in m_bodies. If true it is added to m_collisionBodies
    int k = 0;
    for(auto i : m_bodies)
    {
        if(i.body == _shape)
        {
            m_collisionBodies.push_back(i);
            return m_collisionBodies[k].body;
            break;
        }
        k++;
    }
    return _shape;

}

ngl::Vec3 PhysicsWorld::rayNormalTest(const ngl::Vec3 &_a, const ngl::Vec3 &_b)
{
  // from raytest bullet wiki
  btVector3 a = btVector3(_a.m_x,_a.m_y,_a.m_z);
  btVector3 b = btVector3(_b.m_x,_b.m_y,_b.m_z);

  btCollisionWorld::ClosestRayResultCallback RayCallback(a, b);
  // Perform raycast
  m_dynamicsWorld->rayTest(a, b, RayCallback);


 //returns the normal of the object that the ray has hit
  if(RayCallback.hasHit())
    {
      return ngl::Vec3(RayCallback.m_hitNormalWorld.getX(),RayCallback.m_hitNormalWorld.getY(),RayCallback.m_hitNormalWorld.getZ());
    }
  else
    {
      return false;
    }
}






