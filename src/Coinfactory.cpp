#include "CoinFactory.h"
#include <ngl/NGLStream.h>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

CoinFactory::CoinFactory()
{
  // set the global coin counter to zero when the factory is created
  m_coinCounter =0;
}

CoinFactory::~CoinFactory()
{
  // traverse through coins
  for(auto i:m_coinList)
    {
      // delete current coin in list
      delete i;
    }
  // clear the list of all the coin pointers
  m_coinList.clear();
}

void CoinFactory::createCoin(ngl::Vec3 _pos, std::string _id, PhysicsWorld *_physics)
{
  m_physics = _physics;
  // create a new coin by calling the coin class and feed in the parameters
  m_coin = new Coin(_pos,_id,m_physics);
  // push the pointer of the new coin to our coinlist
  m_coinList.push_back(m_coin);
}

void CoinFactory::deleteCoin(Coin *_coin)
{
  int iter = 0;
  // traverse through the pointers of our coin list
  for (auto i: m_coinList)
    {
      iter++;
      // if the pointer equals the specified coin pointer
      if(i == _coin)
        {
          // delete the coin
          delete i;
          // remove from the coin list
          m_coinList.erase(m_coinList.begin()+(iter-1),m_coinList.begin()+(iter));
        }
    }
}

void CoinFactory::readCoinPosData()
{
  QFile inFile("Preferences/coinPos.txt"); //destination
  if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      std::cerr<<"Error Reading file"<<std::endl;
      std::exit(1);
    }
  //read all variables as string lines
  QTextStream in(&inFile);
  while(!in.atEnd())
    {
      QString coinPosString = in.readLine();
      m_coinPosListUnordered.push_back (coinPosString.toFloat() );
    }
  //close file
  inFile.close();
  for(int i =0; i<m_coinPosListUnordered.size(); i+=3)
    {
      ngl::Vec3 point(m_coinPosListUnordered[0+i], m_coinPosListUnordered[1+i], m_coinPosListUnordered[2+i]);
      m_coinPosList.push_back(point);
    }
}
