#include "Coin.h"

Coin::Coin(ngl::Vec3 _pos, std::string _id, PhysicsWorld * _physics)
{
  // initalise parameters
  m_physics = _physics;
  m_coinPos = _pos;
  m_coinID = _id;
  m_coinTimer.start();
  m_coinTimer.pause();
  //create the bbox for the trigger around the coin
  m_bboxMin.set(m_coinPos.m_x-4,m_coinPos.m_y-4,m_coinPos.m_z-4);
  m_bboxMax.set(m_coinPos.m_x+4,m_coinPos.m_y+4,m_coinPos.m_z+4);

  // create a the new trigger for the coin and link it to the trigger pointer.
  m_coinTrigger = new Trigger(_id, m_bboxMin,m_bboxMax, _physics);

}

Coin::~Coin()
{
  // delete coin trigger when dtor is called to avoid memory leaks
  delete m_coinTrigger;
}

ngl::Vec3 Coin::getCoinPos()
{
  return m_coinPos;
}

Trigger* Coin::getTrigger()
{
  return m_coinTrigger;
}

bool Coin::isCoinHit(std::string _playerName)
{
  // checks if the coin's trigger has been in contact with player.Returns result as bool.
  return m_coinTrigger->isHit(_playerName);
}

void Coin::setCoinState(bool _state)
{
  if(_state == 0)
    {
      m_coinState = 0;
    }
  else if(_state == 1)
    {
      m_coinState = 1;
    }
}

void Coin::startTime()
{
  m_coinTimer.unpause();
}

