#ifndef PHYSICSWORLD_H__
#define PHYSICSWORLD_H__

#include <btBulletDynamicsCommon.h>
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/Obj.h>
#include <memory>
#include <set>
#include <BulletDebugDraw.h>
//-------------------------------------------------------------------------------------------------------------------------
/// @file PhysicsWorld.h
/// @author Eligijus Titas, Morgan Henty, Tom Rule, Tarquin Pereira, Rebecca Maddison
/// @version 1.0
/// @class PhysicsWorld
/// @brief This manages coin creation and collisions of the coins
//--------------------------------------------------------------------------------------------------------------------------
class PhysicsWorld
{
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor
    /// @param[in]  _debug - enables or disables debug drawing
    //----------------------------------------------------------------------------------------------------------------------
    PhysicsWorld(bool _debug);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor
    //----------------------------------------------------------------------------------------------------------------------
    ~PhysicsWorld();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the gravity for the whole world
    /// @param[in] _x
    /// @param[in] _y
    /// @param[in] _z
    /// @param[in] _g - single vector for gravity
    //----------------------------------------------------------------------------------------------------------------------
    inline void setGravity(float _x, float _y, float _z)
    {
      m_dynamicsWorld->setGravity(btVector3(_x,_y,_z));
    }
    inline void setGravity(ngl::Vec3 _g)
    {
      m_dynamicsWorld->setGravity(btVector3(_g.m_x,_g.m_y,_g.m_z));
    }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the number of collision objects in the world
    //----------------------------------------------------------------------------------------------------------------------
    inline unsigned int getNumCollisionObjects()const
    {
      return m_dynamicsWorld->getNumCollisionObjects();
    }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the name of the body at the chosen index
    /// @param[in]  i - the index of the desired body
    //----------------------------------------------------------------------------------------------------------------------
    inline std::string getBodyNameAtIndex(unsigned int i) const{return m_bodies[i].name;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds a rigid body sphere to the physics world
    /// @param[in] _shapeName - the name of the shape
    /// @param[in] _pos - the shape's position
    /// @param[in] _mass - the mass of the shape
    /// @param[in] _inertia - the local inertia for the shape
    /// @param[in] _draw - enables drawing of the collision mesh in debug mode
    //----------------------------------------------------------------------------------------------------------------------
    void addSphere(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia, bool _draw);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds a rigid body capsule to the physics world, used for standing character
    /// @param[in] _shapeName - the name of the shape
    /// @param[in] _pos - the shape's position
    /// @param[in] _mass - the mass of the shape
    /// @param[in] _inertia - the local inertia for the shape
    //----------------------------------------------------------------------------------------------------------------------
    void addLegSphere(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds a rigid body mesh to the physics world
    /// @param[in] _shapeName - the name of the shape, must match the name of an existing collision shape
    /// @param[in] _pos - the shape's position
    /// @param[in] _mass - the mass of the shape
    /// @param[in] _inertia - the local inertia for the shape
    //----------------------------------------------------------------------------------------------------------------------
    void addMesh(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds an unmoving rigid body mesh to the physics world. Creates a convex collision shape.
    /// @param[in] _shapeName - the name of the shape, must match the name of an existing collision shape
    /// @param[in] _pos - the shape's position
    //----------------------------------------------------------------------------------------------------------------------
    void addLevelMesh(std::string _shapeName, const ngl::Vec3 &_pos);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds a rigid body cube to the physics world
    /// @param[in] _shapeName - the name of the shape
    /// @param[in] _pos - the shape's position
    //----------------------------------------------------------------------------------------------------------------------
    void addBox(std::string _shapeName,const ngl::Vec3 &_pos);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief steps through the physics simulation
    /// @param[in] _timeStep - time between two physics updates
    /// @param[in] _maxSubSteps - the maximum number of steps Bullet may use each time this function is called
    /// @param[in] _fixedTimeStep - the maximum size of a step
    //----------------------------------------------------------------------------------------------------------------------
    void step(float _timeStep, float _maxSubSteps, float _fixedTimeStep);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds a rigid body platform mesh to the physics world. These don't collide with level meshes.
    /// @param[in] _shapeName - the name of the shape, must match the name of an existing collision shape
    /// @param[in] _pos - the shape's position
    /// @param[in] _mass - the mass of the shape
    /// @param[in] _inertia - the local inertia for the shape
    //----------------------------------------------------------------------------------------------------------------------
    void addPlatform(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief adds an impulse force to the shape
    /// @param[in] _i - the vector for the force
    /// @param[in] _shape - the name of the shape to apply the force to
    //----------------------------------------------------------------------------------------------------------------------
    void addImpulse(const ngl::Vec3 &_i, std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief applies a force to the shape
    /// @param[in] _p - the vector for the force
    /// @param[in] _shape - the name of the shape to apply the force to
    //----------------------------------------------------------------------------------------------------------------------
    void applyForceCentral(const ngl::Vec3 &_p, std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the linear velocity of the specified shape
    /// @param[in] _shape - the name of the shape to use
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getLinVelocity(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the linear velocity of the specified shape
    /// @param[in] _shape - the name of the shape to use
    /// @param[in] _velocity - the new velocity vector
    //----------------------------------------------------------------------------------------------------------------------
    void setLinVelocity(std::string _shape, ngl::Vec3 &_velocity);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the angular velocity of the specified shape
    /// @param[in] _shape - the name of the shape to use
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getAngVelocity(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the angular velocity of the specified shape
    /// @param[in] _shape - the name of the shape to use
    /// @param[in] _velocity - the new velocity vector
    //----------------------------------------------------------------------------------------------------------------------
    void setAngVelocity(std::string _shape, ngl::Vec3 &_velocity);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief removes all physics bodies
    //----------------------------------------------------------------------------------------------------------------------
    void reset();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the position of the specified shape
    /// @param[in] _shape - the name of the shape to use
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getPosition(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the transformation matrix of the specified shape
    /// @param[in] _index - the index of the desired shape in the bodies array
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 getTransformMatrix(unsigned int _index);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the transformation matrix of the specified shape
    /// @param[in] _shape - the name of the shape to use
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 getTransformMatrix(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the transformation matrix of the specified shape
    /// @param[in] _shape - the name of the shape to use
    /// @param[in] _mat - the new matrix
    //----------------------------------------------------------------------------------------------------------------------
    void setTransformMatrix(std::string _shape, ngl::Mat4 _mat);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief prints the names of all rigid bodies in the world
    //----------------------------------------------------------------------------------------------------------------------
    void printBodies();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief removes the specified rigid body from the world
    /// @param[in] _shape - the name of the shape to remove
    //----------------------------------------------------------------------------------------------------------------------
    void removeCollisionObj(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the position of the specified shape's centre of mass
    /// @param[in] _shape - the name of the shape to use
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getCentreOfMass(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draws all the collision shapes as wireframes
    //----------------------------------------------------------------------------------------------------------------------
    void debugWorldDraw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief fires a ray from a to b and returns true if a collision object is obstructing it
    /// @param[in] _a - the origin point
    /// @param[in] _b - the target point
    /// @param[in] _out - the position of the impact point
    //----------------------------------------------------------------------------------------------------------------------
    bool rayTest(const ngl::Vec3 &_a, const ngl::Vec3 &_b);
    bool rayTest(const ngl::Vec3 &_a, const ngl::Vec3 &_b, ngl::Vec3 &_out);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief fires a ray from a to b and returns the velocity of the object it hits
    /// @param[in] _a - the origin point
    /// @param[in] _b - the target point
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getVelocityOfPlatform(const ngl::Vec3 &_a, const ngl::Vec3 &_b);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief fires a ray from a to b and returns the normal of the object at the hit point
    /// @param[in] _a - the origin point
    /// @param[in] _b - the target point
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 rayNormalTest(const ngl::Vec3 &_a, const ngl::Vec3 &_b);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the gravity multiplier for a specific body (useful for making things weightless)
    /// @param[in] _shape - the name of the shape to use
    /// @param[in] _vec - the new gravity vector to use
    //----------------------------------------------------------------------------------------------------------------------
    void setGravityForBody(std::string _shape, ngl::Vec3 _vec);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief stops all motion for the specified body
    /// @param[in] _shape - the name of the shape to use
    //----------------------------------------------------------------------------------------------------------------------
    void freezeBody(std::string _shape);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief function checks whether specific shape is in m_bodies. If true it is added to m_collisionBodies
    /// @param[in] _shape - the name of the shape to find
    //----------------------------------------------------------------------------------------------------------------------
    btRigidBody* FindGameObject(btRigidBody *_shape);
    //----------------------------------------------------------------------------------------------------------------------

  private :

    typedef struct
    {
      std::string name;
      btRigidBody* body;
    }Body;

    /// @brief the debug drawer
    std::unique_ptr<BulletDebugDraw> debugDrawer;

    // bullet stuff
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btBroadphaseInterface> m_overlappingPairCache ;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
    btCollisionShape* m_groundShape;
    // the dynamic body list 
    std::vector <Body> m_bodies;

    // collision callback
    typedef std::pair<const btRigidBody*, const btRigidBody*> collisionPair;
    typedef std::set<collisionPair> collisionPairs;
    Body collisionPairA;
    Body collisionPairB;
    std::vector <Body> m_collisionBodies;
    std::vector <std::vector <Body>> m_collisionBodiesPair;
    std::vector <Body> m_findGameBodies;
    collisionPairs m_pairsLastUpdate;
    //
};

#endif

