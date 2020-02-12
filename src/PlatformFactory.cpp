#include "PlatformFactory.h"

PlatformFactory::PlatformFactory(PhysicsWorld *_physics)
{
  m_physics = _physics;

}

PlatformFactory::~PlatformFactory()
{
  // traverse through platforms
  for(auto i:m_platformList)
    {
      // delete current platform in list
      delete i;
    }
  // clear the list of all the platform pointers
  m_platformList.clear();
}

void PlatformFactory::createPlatform(ngl::Vec3 _pos, std::string _platName, PlatType _platType, PhysicsWorld* _physics)
{
  // create a new platform from our platform class and assign a pointer to it
  m_platform = new Platform(_pos,_platName,_platType,_physics);
  // call the creation of platform
  m_platform->createPlatform();
  // add the platform's pointer to a vector list
  m_platformList.push_back(m_platform);
}

void PlatformFactory::movePlatform(std::string _platformName, float _vel,float _range, Direction _d)
{
  int iter = 0;
  // traverse through platform list
  for(auto i:m_platformList)
    {
      iter++;
      // if pointer to platform equals the specified name
      if(i->getPlatformName() == _platformName)
        {
          //set that platform to move by our parameter values
          i->movePlatform(_vel,_range,_d);
        }
    }

}


void PlatformFactory::rotatePlatform(std::string _platformName, std::string _axis)
{
  int iter = 0;
  // traverse through platform list
  for(auto i:m_platformList)
    {
      iter++;
      // if pointer to platform equals specified name
      if(i->getPlatformName() == _platformName)
        {
          // rotate the platform by parameter value
          i->rotatePlatform(_axis);
        }
    }

}

void PlatformFactory::addRot(std::string _platformName, int _deg)
{
  int iter = 0;
  for(auto i:m_platformList)
    {
      iter++;
      if(i->getPlatformName() == _platformName)
        {
          // add rotation to specified platform
          i->addRot(_deg);
        }
    }

}

void PlatformFactory::setRot(std::string _platformName, int _rot)
{
    int iter = 0;
    for(auto& i:m_platformList)
    {
        iter++;
        if(i->getPlatformName() == _platformName)
        {
            i->setRot(_rot);
        }
    }

}


void PlatformFactory::deletePlatform(Platform *_platform)
{
  int iter = 0;
  for(auto i:m_platformList)
    {
      iter++;
      if(i == _platform)
        {
          //delete platform and remove pointer from list
          delete i;
          m_platformList.erase(m_platformList.begin()+(iter-1),m_platformList.begin()+(iter));
        }
    }
}
