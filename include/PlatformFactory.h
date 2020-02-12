#ifndef PLATFORMFACTORY
#define PLATFORMFACTORY

#include "Platform.h"
#include "CollisionShape.h"

class Platform;
//-------------------------------------------------------------------------------------------------------------------------
/// @file PlatformFactory.h
/// @author Eligijus Titas, Morgan Henty, Tom Rule
/// @version 1.0
/// @class PlatformFactory
/// @brief This manages coin creation and collisions of the coins
//--------------------------------------------------------------------------------------------------------------------------
class PlatformFactory
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor of the platform factory
  //----------------------------------------------------------------------------------------------------------------------
  PlatformFactory(PhysicsWorld *_physics);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor of the platform factory
  //----------------------------------------------------------------------------------------------------------------------
  ~PlatformFactory();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief creates a new platform by calling our platform class
  /// @param _pos position of our platform (x,y,z)
  /// @param _platNaeme name of our platform
  /// @param _platType enum class that specifies the type of platform to create
  /// @param _physics pointer to our physicsWorld
  //----------------------------------------------------------------------------------------------------------------------
  void createPlatform(ngl::Vec3 _pos, std::string _platName, PlatType _platType,PhysicsWorld *_physics);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set a specified platform to move
  /// @param _platformName name of the platform
  /// @param _vel velocity of the platform
  /// @param _range distance that the platform will move between
  /// @param _d enum that specifies the direction of platform in either x,y or z
  //----------------------------------------------------------------------------------------------------------------------
  void movePlatform(std::string _platformName, float _vel,float _range, Direction _d);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set a the platform to rotate
  /// @param _platformName name of our platform
  /// @param _axis string that specifies the axis rotation
  //----------------------------------------------------------------------------------------------------------------------
  void rotatePlatform(std::string _platformName, std::string _axis);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  add rotation value to the platform
  /// @param _platformName name of the platform
  //----------------------------------------------------------------------------------------------------------------------
  void addRot(std::string _platformName, int _deg);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief  set rotation value of the platform
  /// @param _platformName name of the platform
  //----------------------------------------------------------------------------------------------------------------------
  void setRot(std::string _platformName, int _rot);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief delete a specific platform
  /// @param _platform pointer to a platform
  //----------------------------------------------------------------------------------------------------------------------
  void deletePlatform(Platform *_platform);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the vector of pointers that hold all the platforms
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Platform *> getPlatformList() {return m_platformList;}
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector that stores the pointers to our platforms
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Platform *> m_platformList;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to a platform
  //----------------------------------------------------------------------------------------------------------------------
  Platform *m_platform;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to our physicsWorld
  //----------------------------------------------------------------------------------------------------------------------
  PhysicsWorld *m_physics;
};

#endif // PLATFORMFACTORY
