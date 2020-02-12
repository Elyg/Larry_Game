#ifndef COLLISIONSHAPE_H__
#define COLLISIONSHAPE_H__

#include <btBulletDynamicsCommon.h>
#include <unordered_map>
#include <memory>
#include <ngl/Types.h>
#include <string>

//-------------------------------------------------------------------------------------------------------------------------
/// @file CollisionShape.h
/// @author Jon Macey, modified by: Eligijus Titas
/// @class CollisionShape
/// @brief simple class to store instances of bullet collision
/// shapes this is a singleton so we only have one instance of
/// each shape.
/// Modification: added a methods for the convex geometry and shape list
//--------------------------------------------------------------------------------------------------------------------------
class CollisionShape
{
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    //----------------------------------------------------------------------------------------------------------------------
    static CollisionShape *instance();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _objFilePath the file path for the obj file
    //----------------------------------------------------------------------------------------------------------------------
    void addMesh(const std::string & _name, const std::string &_objFilePath);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _rad the radius of the sphere
    //----------------------------------------------------------------------------------------------------------------------
    void addSphere(const std::string & _name, ngl::Real _rad);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _w the width of the box
    /// @param [in] _h the height of the box
    /// @param [in] _d the depth of the box
    //----------------------------------------------------------------------------------------------------------------------
    void addBox(const std::string & _name, ngl::Real _w, ngl::Real _h, ngl::Real _d);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _rad the radius of the capsule
    /// @param [in] _height height of the capsule
    //----------------------------------------------------------------------------------------------------------------------
    void addCapsule(const std::string & _name, ngl::Real _rad, ngl::Real _height);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _rad the radius of the capsule
    /// @param [in] _height height of the capsule
    //----------------------------------------------------------------------------------------------------------------------
    void addCylinder(const std::string & _name, ngl::Real _rad, ngl::Real _height);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _rad the radius of the capsule
    /// @param [in] _height height of the capsule
    //----------------------------------------------------------------------------------------------------------------------
    void addCone(const std::string & _name, ngl::Real _rad, ngl::Real _height);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create instance of the collision shape
    /// @param [in] _name the shape name of the collision shape
    /// @param [in] _objFilePath the path of the obj file for the shape
    //----------------------------------------------------------------------------------------------------------------------
    void addLevelMesh(const std::string & _name, const std::string &_objFilePath);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief prints the shape map
    //----------------------------------------------------------------------------------------------------------------------
    void printShapeMap();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the shape of the given name
    /// @param [in] _name the shape name of the collision shape
    //----------------------------------------------------------------------------------------------------------------------
    btCollisionShape* getShape(const std::string &_name);
  private :
    CollisionShape(){}
    CollisionShape(const CollisionShape &_c)=delete;
    CollisionShape & operator=(const CollisionShape &_c)=delete;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a map of shapes with the name and a collision shape
    //----------------------------------------------------------------------------------------------------------------------
    std::unordered_map <std::string,std::unique_ptr<btCollisionShape>> m_shapes;
};



#endif
