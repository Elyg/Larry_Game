#ifndef CAMERA__
#define CAMERA__

#include "PhysicsWorld.h"
#include "ngl/Camera.h"

/// @file Camera.h
/// @author Tom Rule
/// @class Camera
/// @brief Contains the camera used for rendering and controls its movement.

class Camera
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for the camera class
    /// @param[in] _physics - pointer to the PhysicsWorld
    //----------------------------------------------------------------------------------------------------------------------
    Camera(PhysicsWorld * _physics);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief destructor
    //----------------------------------------------------------------------------------------------------------------------
    ~Camera();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Called each frame to update the camera's position
    /// @param[in] _name - the name of the PhysicsWorld body for the camera to focus on
    //----------------------------------------------------------------------------------------------------------------------
    void updatePos(std::string _name);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Can be called as an alternative to updatePos. Snaps the camera's position to the default position OR snaps the camera's position to the chosen angle
    /// @param[in] _name - the name of the PhysicsWorld body for the camera to focus on
    /// @param[in] _angle - the angle to rotate the camera about the focus
    //----------------------------------------------------------------------------------------------------------------------
    void snapPos(std::string _name);
    void snapPos(std::string _name, float _angle);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Can be called as an alternative to updatePos. Sets the camera's ideal position based on input values and lets the PhysicsWorld handle the motion. Less jarring than snapPos.
    /// @param[in] _name - the name of the PhysicsWorld body for the camera to focus on
    /// @param[in] _angle - the angle to rotate the camera about the focus
    /// @param[in] _focus - alternative focus for the camera if desired
    //----------------------------------------------------------------------------------------------------------------------
    void forcePos(std::string _name, float _angle);
    void forcePos(std::string _name, float _angle, ngl::Vec3 _focus);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the Eye vector
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 getEye();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the Eye vector
    /// @param[in] _eye - the vector to set
    //----------------------------------------------------------------------------------------------------------------------
    void setEye(ngl::Vec4 _eye);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the Look vector
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec4 getLook();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the Look vector
    /// @param[in] _look - the vector to set
    //----------------------------------------------------------------------------------------------------------------------
    void setLook(ngl::Vec4 _look);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the view matrix
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 getViewMatrix();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the ViewProjection matrix
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 getVPMatrix();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the Projection matrix
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 getProjectionMatrix();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the distance between the camera and the focus
    //----------------------------------------------------------------------------------------------------------------------
    float getDistance();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the distance between the camera and the focus
    /// @param[in] _distance - the distance to set
    //----------------------------------------------------------------------------------------------------------------------
    void setDistance(float _distance);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the height of the camera
    //----------------------------------------------------------------------------------------------------------------------
    float getHeight() {return m_camHeight;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief sets the height of the camera
    /// @param[in] _height - the new height to set
    //----------------------------------------------------------------------------------------------------------------------
    void setHeight(float _height) {m_camHeight = _height;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the angle of the camera about the focus
    //----------------------------------------------------------------------------------------------------------------------
    float getAngle();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the angle of the camera about the focus
    /// @param[in] _angle - the angle to set in radians
    //----------------------------------------------------------------------------------------------------------------------
    void setAngle(float _angle);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the ideal position of the camera
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 getIdealPos();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the ideal position for the camera
    /// @param[in] _pos - the vector to set
    //----------------------------------------------------------------------------------------------------------------------
    void setIdealPos(ngl::Vec3 _pos);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the camera's shape
    /// @param[in] _angle - the View angle
    /// @param[in] _aspect - the aspect ratio
    /// @param[in] _near - near clipping distance
    /// @param[in] _far - far clipping distance
    //----------------------------------------------------------------------------------------------------------------------
    void setShape(int _angle, float _aspect, float _near, float _far);
    //----------------------------------------------------------------------------------------------------------------------
  private:
    /// @brief A pointer to an NGL::Camera
    ngl::Camera *m_camera;

    /// @brief The position that the camera will always accelerate towards
    ngl::Vec3 m_camIdealPos;

    /// @brief The distance between the camera and the focus
    float m_camDistance;

    /// @brief Temporary distance for avoiding obstacles
    float m_tempDistance;

    /// @brief The angle of the camera about the focus
    float m_camAngle;

    /// @brief The height of the camera relative to the focus
    float m_camHeight;

    /// @brief Velocity value used to amend with the physics simulation
    ngl::Vec3 m_camVel;

    /// @brief a pointer to the PhysicsWorld to use
    PhysicsWorld * m_physics;
};

#endif // CAMERA

