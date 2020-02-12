#ifndef NGLDRAW_H__
#define NGLDRAW_H__

#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <SDL.h>
#include <ngl/Transformation.h>
#include <memory>
#include <ngl/Obj.h>
#include "Text.h"
#include <Character.h>
#include <Level.h>
#include "Animation.h"
#include "Camera.h"
#include "CoinFactory.h"
#include "PlatformFactory.h"
#include "HighScore.h"
#include "Trigger.h"
#include <list>
#include "TriggerFactory.h"
#include "ShaderFactory.h"

class Level;

class Character;

class PhysicsWorld;

class Animation;

class CoinFactory;

class PlatformFactory;
//-------------------------------------------------------------------------------------------------------------------------
/// @file NGLDraw.h
/// @author Eligijus Titas, Morgan Henty, Tom Rule, Tarquin Pereira, Rebecca Maddison, Gareth FitzGerald, Elisabeth Wetchy
/// @version 1.0
/// @class NGLDraw
/// @brief This manages coin creation and collisions of the coins
//--------------------------------------------------------------------------------------------------------------------------
class NGLDraw
{
  public :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor this will have a valid OpenGL context so we can create gl stuff
    //----------------------------------------------------------------------------------------------------------------------
    NGLDraw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor used to remove any NGL stuff created
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLDraw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief resize our screen and set the camera aspect ratio
    /// @param[in] _w the new width
    /// @param[in] _h the new height
    //----------------------------------------------------------------------------------------------------------------------
    void resize(int _w, int _h);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void draw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw the scene from the lights POV for the depth pass
    //----------------------------------------------------------------------------------------------------------------------
    void drawDepth();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (const SDL_MouseMotionEvent &_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse button is presses
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent (const SDL_MouseButtonEvent &_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent (const SDL_MouseButtonEvent &_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// @param _event the SDL mouse event structure containing all mouse info
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(const SDL_MouseWheelEvent &_event);


    void updateRot();
    /// @brief adds a cube used to test bullet physics
    void addCube();
    void updateDraw();
    void updateDebugDraw();
    void updatePhysics(float time);
    void debugPhysicsWorld(bool _value);
    void checkpoints();

    /// @brief method to create a noise texture
    /// @param[in] m_freq the frequency of the noise pattern
    /// @param[in] m_scale the scale (amplitude) of the noise pattern
    void makeNoise(float m_freq, float m_scale);

    /// @brief method to pick a shader type and load object matrices to the shader
    /// @param[in] _shaderName the name of the shader
    /// @param[in] _shaderColour the r,g,b colour values of the object
    /// @param[in] stripe switch for turning on or off stripe patterns
    /// @param[in] shine the specular reflectivity of the object
    /// @param[in] _trans the object transform matrix
    void shaderType(std::string _shaderName, ngl::Vec3 _shaderColour, bool stripe, float shine, ngl::Transformation &_trans);
//----------------------------------------------------------------------------------------------------------------------
    /// @brief increments the gravity factor
    void setGravityFactorPositive();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief decrements the gravity factor
    void setGravityFactorNegative();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the value that multiplies with -9.8 to create the gravity
    float getGravityFactor();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief resets the character and game values
    void restartGame();
//----------------------------------------------------------------------------------------------------------------------
    /// @brief returns a pointer to the PhysicsWorld
    PhysicsWorld *getPhysicsWorld(){return m_physics.get(); }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns a pointer to the character
    Character *getCharacter(){return m_character.get(); }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns a pointer to the level manager
    Level *getLevel(){return m_level.get(); }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns a pointer to the animation manager
    Animation *getAnimation(){ return m_animation.get();}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns a pointer to the coin factory
    CoinFactory *getCoinFactory() {return m_coinFactory.get();}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns a pointer to the platform factory
    PlatformFactory *getPlatformFactory() {return m_platformFactory.get();}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief increments the variable m_time
    int increaseShaderTime();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief create our fbo for shadows
    void createFramebufferObject();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief increment the lowThresholdKey for the disintegration effect
    float increaseLowThreshold();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief reset the lowThresholdKey to zero
    void resetLowThreshold();
    /// @brief reset the game values
    void restartGameValues();
    /// @brief reset the game values
    float increaseCoinColour();
    /// @brief reset coin color to zero
    void resetCoinColour();

//--------------------------------------------------------------------------------------------------------------------------


  private :
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief method to load transform data to the shaders
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader(ngl::Transformation &_trans);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
    float m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
    float m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the global mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Mat4 m_mouseGlobalTX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the model position for mouse movement
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a simple light use to illuminate the screen
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Light *m_light;
//----------------------------------------------------------------------------------------------------------------------
    /// @brief the PhysicsWorld from bullet physics
    std::unique_ptr<PhysicsWorld> m_physics;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief class to manage the high score
    std::unique_ptr<HighScore> m_highScore;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief class to manage the trigger volumes
    std::unique_ptr<TriggerFactory>m_triggerFactory;
//----------------------------------------------------------------------------------------------------------------------
    /// @brief class to manage the character
    std::unique_ptr<Character> m_character;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief class to manage all the level meshes
    std::unique_ptr<Level> m_level;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief class to handle all the animation
    std::unique_ptr<Animation> m_animation;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief class to manage the coins
    std::unique_ptr<CoinFactory> m_coinFactory;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief class to manage all the platforms
    std::unique_ptr<PlatformFactory> m_platformFactory;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief text object
    std::unique_ptr <Text> m_text;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief text object
    std::unique_ptr <Text> m_textHighscore;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief mesh for the character
    std::unique_ptr<ngl::Obj> m_larryMesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level1Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level2_1Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level2_2Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level2_3aMesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level2_3bMesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level3Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level4_1Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level4_2Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief level mesh
    std::unique_ptr<ngl::Obj> m_level5Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief coin mesh
    std::unique_ptr<ngl::Obj> m_coinMesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_platformMesh_platform;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_platformMesh_bouncePad;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_platformMesh_barrier01;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level2Platform;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level2Sliding1;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level2Sliding2;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level4Bridge1Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level4Bridge2Mesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level4RotPlatformMesh;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level5MovingPlat;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief platform mesh
    std::unique_ptr<ngl::Obj> m_level1_bouncePlatformMesh;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief level 5 mesh
    std::unique_ptr<ngl::Obj> m_level5Stick;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief finish mesh
    std::unique_ptr<ngl::Obj> m_finish;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief transformation matrices
    ngl::Mat4 m_bodyTransform;                  //collision matrix
    ngl::Transformation m_trans_grid;           //grid transform matrix
    ngl::Transformation m_trans_sphere;         //sphere transform matrix
    ngl::Transformation m_trans_cube;           //cube transform matrix
    ngl::Transformation m_trans_level;          //apple transform matrix
    ngl::Transformation m_trans_platform;       //platform transform matrix
    ngl::Transformation m_position;
    ngl::Transformation m_transform;
    ngl::Transformation m_trans_coin;
    ngl::Transformation m_empty_trans;           //grid transform matrix
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief position for debug text drawing
    ngl::Vec3 m_textPos;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief velocity for debug text drawing
    ngl::Vec3 m_textVel;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief width of the screen
    int m_w;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief height of the screen
    int m_h;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief stores the last time the physics was updated
    float m_lastTime;
        //----------------------------------------------------------------------------------------------------------------------
    ///@brief the rotation of the character
    float m_rot;

    float m_prevRot;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief enable or disable debug
    bool m_debug;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief the number of ticks until the button is active again
    int m_buttonCooldown;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief true if the bridge in level 4 is down
    bool m_bridgeDown;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief the starting position of the ball
    ngl::Vec3 m_initialBallPos;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief the mass of the ball
    float m_mass;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief enable stickiness
    bool m_attractEnable;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief the value that is multiplied with -9.8 to produce the gravity value
    float m_gravityFactor;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief the name of the ball
    std::string m_formOne;
        //----------------------------------------------------------------------------------------------------------------------
    /// @brief the name of the legged character
    std::string m_formTwo;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the position of the light camera used to create the shadow map
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vec3 m_lightPosition;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief camera from the lights point of view (used for shadows)
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Camera m_lightCamera;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief array to store multiple textures
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_TextureID[2];
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief array to store links to multiple shader subroutines
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_subroutines[3];
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our FBO id used by the FBO
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_fboID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our incrementing time variable to send to the shader for animation
    //----------------------------------------------------------------------------------------------------------------------
    int m_time;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief coin colour
    //----------------------------------------------------------------------------------------------------------------------
    float m_coinColour;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief lower nois threshold
    //----------------------------------------------------------------------------------------------------------------------
    float m_lowThresholdKey;

};


#endif
