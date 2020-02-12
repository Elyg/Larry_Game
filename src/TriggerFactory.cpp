#include "TriggerFactory.h"

TriggerFactory::TriggerFactory(PhysicsWorld *_physics)
{
  //store the physics world pointer
  m_physics = _physics;
}
void TriggerFactory::createTrigger(std::string _id, ngl::Vec3 _pointOne, ngl::Vec3 _pointTwo)
{
  //create a shared pointer for a trigger
  std::shared_ptr<Trigger> trigger(new Trigger(_id, _pointOne, _pointTwo, m_physics));
  //push the shared pointer to the trigger list
  m_triggerList.push_back(trigger);
}
void TriggerFactory::createTriggerFromFile()
{
  //loop through the unordered list of strings that were read from txt file
  for(int i =0; i<m_triggerListUnordered.size(); i+=7)
    {
      //store name
      std::string name= m_triggerListUnordered[i];
      //convert the string values to floats and then create a two points
      ngl::Vec3 pointOne(std::stof(m_triggerListUnordered[1+i]), std::stof(m_triggerListUnordered[2+i]), std::stof(m_triggerListUnordered[3+i]));
      ngl::Vec3 pointTwo(std::stof(m_triggerListUnordered[4+i]), std::stof(m_triggerListUnordered[5+i]), std::stof(m_triggerListUnordered[6+i]));
      //create trigger using the extracted values from the file
      createTrigger(name, pointOne, pointTwo);
    }
}
void TriggerFactory::deleteTrigger(std::string _id)
{
  //iterator
  int i=0;
  //initialize found
  bool found = false;
  //loops through the trigger list and removes the trigger from the list
  for (auto trigger : m_triggerList )
    {
      if(_id == trigger->getTriggerID())
        {
          m_triggerList.erase(m_triggerList.begin()+i);
          found = true;
          break;
        }
      i++;
    }
  //if no trigger found print the error
  if(found == false)
    {
      std::cout<<"Did not find trigger: "+ _id<<std::endl;
    }
}

void TriggerFactory::readTriggerPosData()
{
  //open the file at the destination
  QFile inFile("Preferences/triggerPos.txt");
  if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      std::cerr<<"Error Reading file"<<std::endl;
      std::exit(1);
    }
  //read all variables as string lines and store them to the string list
  QTextStream in(&inFile);
  while(!in.atEnd())
    {
      QString coinPosString = in.readLine();
      m_triggerListUnordered.push_back(coinPosString.toStdString());

    }
  //close file
  inFile.close();

}
ngl::Transformation TriggerFactory::getTransform(std::string _id)
{
  //iterator
  int i=0;
  //initialize found
  bool found = false;
  //loops through the trigger list and gets the trigger's transformation
  for (auto trigger : m_triggerList)
    {
      if(_id == trigger->getTriggerID())
        {
          found = true;
          return trigger->getTransform();
        }
      i++;
    }
  //if no trigger found in the list return empty transformation and print the wrong trigger name
  if(found == false)
    {
      std::cout<<"Did not find trigger with ID: "+ _id<<std::endl;
      return ngl::Transformation();
    }
}
ngl::Transformation TriggerFactory::getTransform(int _index)
{
  //if index is less than the size of the trigger list get the transformation using the _index
  if(_index <= getTriggerListSize() )
    {
      return m_triggerList[_index]->getTransform();
    }
  else
    {
      std::cout<<"Index: "+std::to_string(_index)+" is out of scope of the list "<<std::endl;
    }
}
void TriggerFactory::printTriggerList()
{
  //prints the trigger volums
  for(auto i : m_triggerList)
    {
      std::string trigger ="";
      trigger = i->getTriggerID();
      std::cout<<trigger<<std::endl;
    }
}
bool TriggerFactory::checkPairHit(std::string _id, std::string _physicsObjName)
{
  //iterator
  int i=0;
  //initialize found
  bool found = false;
  //loops through the trigger list and check the physics object collides with the trigger volume
  for (auto trigger : m_triggerList)
    {
      if(_id == trigger->getTriggerID())
        {
          found = true;
          return trigger->isHit(_physicsObjName);
        }
      i++;
    }
  //if no trigger found with the given name print the error
  if(found == false)
    {
      std::cout<<"Did not find trigger with ID: "+ _id<<std::endl;
      return false;
    }
}
