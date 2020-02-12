#ifndef COIN
#define COIN

#include <NGLDraw.h>
#include "Trigger.h"
#include "Ltimer.h"

class PhysicsWorld;
//-------------------------------------------------------------------------------------------------------------------------
/// @file Coin.h
/// @author Morgan Henty
/// @version 1.0
/// @class Coin
/// @brief This class creates the coin
//--------------------------------------------------------------------------------------------------------------------------
class Coin
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor of the animation class
  /// @param _pos vector position of coin (x,y,z)
  /// @param _id identifier for coin
  /// @param _physics pointer to our PhysicsWorld
  //----------------------------------------------------------------------------------------------------------------------
  Coin(ngl::Vec3 _pos, std::string _id, PhysicsWorld * _physics);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor of the animation class
  //----------------------------------------------------------------------------------------------------------------------
  ~Coin();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns a vector position of the coin
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getCoinPos();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns a pointer to the coin's trigger
  //----------------------------------------------------------------------------------------------------------------------
  Trigger *getTrigger();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns bool value whether player is in contact with coin's trigger
  //----------------------------------------------------------------------------------------------------------------------
  bool isCoinHit(std::string _playerName);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief set whether coin state is active or deactivated
  //----------------------------------------------------------------------------------------------------------------------
  void setCoinState(bool _state);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief start the coin timer
  //----------------------------------------------------------------------------------------------------------------------
  void startTime();

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the current state of coin
  //----------------------------------------------------------------------------------------------------------------------
  bool getCoinState() { return m_coinState;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the current coin time
  //----------------------------------------------------------------------------------------------------------------------

  inline float getTime() { return (m_coinTimer.getTicks() / 1000.0f );}


private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to the trigger class, used for the coin's trigger box
  //----------------------------------------------------------------------------------------------------------------------
  Trigger *m_coinTrigger;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to our physicsWorld
  //----------------------------------------------------------------------------------------------------------------------
  PhysicsWorld * m_physics;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector position of the coin (x,y,z)
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_coinPos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief identifier for coin
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_coinID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief min bounding box co-ordinates for coin's trigger (xmin,ymin,zmin)
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_bboxMin;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief max bounding box co-ordinates for coin's trigger (xmax,ymax,zmax)
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_bboxMax;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief holds the current state of the coin
  //----------------------------------------------------------------------------------------------------------------------
  bool m_coinState=1;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief store the current age of the coin
  //----------------------------------------------------------------------------------------------------------------------
  LTimer m_coinTimer;

};

#endif // COIN

