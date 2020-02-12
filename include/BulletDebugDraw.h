#ifndef BULLETDEBUGDRAW_H
#define BULLETDEBUGDRAW_H

#include <LinearMath/btIDebugDraw.h>
#include <vector>
#include <ngl/Vec3.h>

//-------------------------------------------------------------------------------------------------------------------------
/// @file bulletDebugDraw.h
/// @author Eligijus Titas
/// @version 1.0
/// @class BulletDebugDraw
/// @brief This class gets the data from the bullet engine to draw debug meshes
//--------------------------------------------------------------------------------------------------------------------------
class BulletDebugDraw: public btIDebugDraw
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for BulletDebugDraw  class
  //----------------------------------------------------------------------------------------------------------------------
  BulletDebugDraw();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief destructor for the BulletDebugDraw Class
  //----------------------------------------------------------------------------------------------------------------------
  ~BulletDebugDraw();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a virtual method from the bullet engine to draw the lines
  /// @param [in] &from first point of the vertex used to draw the line
  /// @param [in] &to second  point of the vertex used to draw the line
  /// @param [in] &color a colour for the lines
  //----------------------------------------------------------------------------------------------------------------------
  virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets a debug mode (check bullet for different modes)
  /// @param [in] _debugMode sets the debug mode
  //----------------------------------------------------------------------------------------------------------------------
  virtual void setDebugMode (int _debugMode);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief draw contact point (not used in this case, but it needs to be defined in order to get the basic debug drawing)
  /// @param [in] &PointOnB the normal of the contact
  /// @param [in] &normalOnB sets the normal for drawing
  /// @param [in] distance the distance for the contact
  /// @param [in] lifeTime the lifetime for the contact point
  /// @param [in] color sets the color for contact point
  //----------------------------------------------------------------------------------------------------------------------
  virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief bullets passes the warnings using this method
  /// @param [in] warningString the warnings from bullet
  //----------------------------------------------------------------------------------------------------------------------
  virtual void reportErrorWarning(const char* warningString);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief bullets passes the warnings using this method
  /// @param [in] warningString the warnings from bullet
  //----------------------------------------------------------------------------------------------------------------------
  virtual void draw3dText(const btVector3& location, const char* textString);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the debug mode
  /// @param [in] warningString the warnings from bullet
  //----------------------------------------------------------------------------------------------------------------------
  virtual int  getDebugMode() const{return m_debugModes;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief update method for the drawing of the lines (not a very efficient way)
  /// @param [in] warningString the warnings from bullet
  //----------------------------------------------------------------------------------------------------------------------
  void drawUpdate();
private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the debug mode variable
  //----------------------------------------------------------------------------------------------------------------------
  int m_debugModes;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a list of points from bullet
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> m_vertices;
};

#endif // BULLETDEBUGDRAW_H
