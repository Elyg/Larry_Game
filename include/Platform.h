#ifndef PLATFORM
#define PLATFORM

#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include "Ltimer.h"

class PhysicsWorld;

enum Direction
{
  MOVE_Y = 0,
  MOVE_Z = 1,
  MOVE_X = 2,
  CIRCLEXY = 3,
  CIRCLEZY = 4,
  TEST =5
};

enum class PlatType
{
  MOVE_PLATE = 0,
  BOUNCE_PAD = 1,
  BARRIER01 = 2,
  BARRIER_LARGE = 3,
  ROTATING_L = 4,
  BRIDGE_1 = 5,
  BRIDGE_2 = 6

};
//----------------------------------------------------------------------------------------------------------------------
/// @file Platform.h
/// @author Morgan Henty, Tom Rule
/// @class Platform
/// @brief A selection of platforms that act as moving parts of the level
//----------------------------------------------------------------------------------------------------------------------
class Platform
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    /// @param[in] _pos - The central position for the platform
    /// @param[in] _platName - A name to use as ID for this platform
    /// @param[in] _platType - the type of platform to use
    /// @param[in] _physics - pointer to the PhysicsWorld
    //----------------------------------------------------------------------------------------------------------------------
    Platform(ngl::Vec3 _pos, std::string _platName, PlatType _platType, PhysicsWorld *_physics);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Creates a platform in the PhysicsWorld
    //----------------------------------------------------------------------------------------------------------------------
    void createPlatform();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Moves the platform in a back-and-forth motion
    /// @param[in] _vel - the speed of the motion
    /// @param[in] _range - the distance either side of the original position that the platform can go
    /// @param[in] _d - the axis on which the platform may move
    //----------------------------------------------------------------------------------------------------------------------
    void movePlatform(float _vel, float _range, Direction _d);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Add rotation to the platform
    /// @param[in] m_deg - the angle to rotate in degrees
    //----------------------------------------------------------------------------------------------------------------------
    void addRot(int _deg);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Set rotation of the platform
    /// @param[in] _rot - the angle to rotate in degrees
    //----------------------------------------------------------------------------------------------------------------------
    void setRot(int _rot);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Called each frame to update the platform's rotation
    /// @param _axis - the axis around which to rotate
    //----------------------------------------------------------------------------------------------------------------------
    void rotatePlatform(std::string _axis);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the position of the platform
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getPlatformPos();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the platform's name
    //----------------------------------------------------------------------------------------------------------------------
    std::string getPlatformName();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the platform's type
    //----------------------------------------------------------------------------------------------------------------------
    PlatType getPlatType();
    //----------------------------------------------------------------------------------------------------------------------

  private:

    /// @brief a pointer to the PhysicsWorld
    PhysicsWorld *m_physics;

    /// @brief the Direction of motion, either 1 or -1
    int m_platformDirection;

    /// @brief the central position of the platform
    ngl::Vec3 m_pos;

    /// @brief the name used as the platform's ID
    std::string m_platName;

    /// @brief the type of platform
    PlatType m_platType;

    /// @brief the angle left to rotate through
    int m_deg;

    /// @brief the cumulative angle of the platform
    int m_rot;

};

#endif // PLATFORM

