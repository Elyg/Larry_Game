#ifndef TRIGGERFACTORY_H
#define TRIGGERFACTORY_H

#include "Trigger.h"
#include <ngl/NGLStream.h>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
//-------------------------------------------------------------------------------------------------------------------------
/// @file TriggerFactory.h
/// @author Eligijus Titas
/// @version 1.0
/// @class TriggerFactory
/// @brief This class manages the trigger volumes,
/// which are volumes that check collisions with the physics objects (character, platform etc.)
//--------------------------------------------------------------------------------------------------------------------------
class TriggerFactory
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for TriggerFactory  class
    /// @param [in] _physics A pointer to the game physics world
    //----------------------------------------------------------------------------------------------------------------------
    TriggerFactory(PhysicsWorld *_physics);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called in order to create a trigger volume
    /// @param [in] _id a name of the trigger volume to create
    /// @param [in] _pointOne a position of either min/max point of the trigger volume
    /// @param [in] _pointTwo a position of either min/max point of the trigger volume
    //----------------------------------------------------------------------------------------------------------------------
    void createTrigger(std::string _id, ngl::Vec3 _pointOne, ngl::Vec3 _pointTwo);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called in order to remove the trigger volume from the list
    /// @param [in] _id a name of the trigger volume to delete
    //----------------------------------------------------------------------------------------------------------------------
    void deleteTrigger(std::string _id);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reads trigger volume position data from a txt file
    //----------------------------------------------------------------------------------------------------------------------
    void readTriggerPosData();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief creates trigger volumes using the position data from the read file
    //----------------------------------------------------------------------------------------------------------------------
    void createTriggerFromFile();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief prints a list of trigger volumes
    //----------------------------------------------------------------------------------------------------------------------
    void printTriggerList();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief checks collision between a trigger object and a trigger volume returning a true if collision occurs and false if not
    /// @param [in] _id a name of the trigger volume to check collision with
    /// @param [in] _physicsObjName a name of the physics object in the physics world to check collision with
    //----------------------------------------------------------------------------------------------------------------------
    bool checkPairHit(std::string _id, std::string _physicsObjName);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets a ngl::transformation of trigger with the given _id name
    /// @param [in] _id a name of the trigger volume to get the transformation of
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation getTransform(std::string _id);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets a ngl::transformation of trigger with the given index from the list
    /// @param [in] _index of the trigger volume in the list
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Transformation getTransform(int _index);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief gets the trigger volume list size
    //----------------------------------------------------------------------------------------------------------------------
    inline int getTriggerListSize(){return m_triggerList.size();}

  private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer used to hold the physics world pointer to acces the physics world of the game
    //----------------------------------------------------------------------------------------------------------------------
    PhysicsWorld *m_physics;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to hold the list of shared pointer of trigger volumes
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<std::shared_ptr<Trigger>> m_triggerList;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a string list that is populated when a txt file is read with trigger volume positions
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<std::string> m_triggerListUnordered;
};

#endif // TRIGGERFACTORY_H
