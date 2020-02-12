#ifndef TRIGGER
#define TRIGGER

#include "ngl/Vec3.h"
#include "PhysicsWorld.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/Transformation.h"

/// @file Trigger.h
/// @author Tom Rule
/// @class Trigger
/// @brief A box that detects if any specified member of the PhysicsWorld is inside

class PhysicsWorld;

class Trigger
{
  public:
    /// @brief Constructor
    /// @param[in] _name - a name as an ID for the trigger box
    /// @param[in] _pos1 - one corner of the box
    /// @param[in] _pos2 - the opposite corner to _pos1
    /// @param[in] _physics - pointer to the PhysicsWorld to use
    Trigger(std::string _name, ngl::Vec3 _pos1, ngl::Vec3 _pos2, PhysicsWorld * _physics);

    /// @brief Destructor
    ~Trigger();

    /// @brief Returns true if the specified physics body is inside the bounds
    /// @param[in] _name - the name of the physics body to test for
    bool isHit(std::string _name);

    /// @brief Returns the transformation matrix for the box, ready for drawing
    ngl::Transformation getTransform();

    /// @brief Returns the trigger's name
    std::string getTriggerID() {return m_triggerID; }

  private:

    /// @brief pointer to the PhysicsWorld containing the bodies to test
    PhysicsWorld * m_physics;

    /// @brief The least corner
    ngl::Vec3 m_pos1;

    /// @brief The greatest corner
    ngl::Vec3 m_pos2;

    /// @brief The trigger's name
    std::string m_triggerID;

};


#endif // TRIGGER
