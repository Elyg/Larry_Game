#ifndef GUIDRAW_H__
#define GUIDRAW_H__

#include <ngl/Obj.h>
#include "Text.h"
//-------------------------------------------------------------------------------------------------------------------------
/// @file GUIDraw.h
/// @author Tarquin Pereira
/// @class GUIDraw
/// @brief This class manages the intro GUI
//--------------------------------------------------------------------------------------------------------------------------
class GUIDraw
{
  public :
    /// @brief ctor
    GUIDraw();

    /// @brief dtor used to remove textures
    ~GUIDraw();

    /// @brief loads the cutscene and menu images
    void loadAllTextures();

    /// @brief draws the opening credits screen
    void drawOpeningCredits();

    /// @brief draws the opening cutscene
    /// @param[in] cutsceneTime the frame number of the cutscene to draw
    void drawOpeningCutscene(int cutsceneTime);

    /// @brief draws the opening title sequence
    /// @param[in] cutsceneTime the frame number of the sequence to draw
    void drawOpeningTitleSequence(int cutsceneTime);

    /// @brief draws the title screen with 'PRESS ENTER' text
    void drawTitleScreen();

    /// @brief draws the title screen without text
    void drawTitleScreenNoText();

    /// @brief draws the main menu
    /// @param[in] button the id of the button the mouse is over
    void drawMainMenu(int button);

    /// @brief draws the controls screen
    void drawControlsScreen();

    /// @brief draws the exit screen
    /// @param[in] button the id of the button the mouse is over
    void drawExitScreen(int button);

    /// @brief draws the pause screen
    /// @param[in] w the width of the screen
    /// @param[in] h the height of the screen
    /// @param[in] resumeSelect flag to indicate if mouse is over 'resume' text
    /// @param[in] mainMenuSelect flag to indicate if mouse is over 'mainMenu' text
    void drawPauseScreen(int w, int h, bool resumeSelect, bool mainMenuSelect);

    /// @brief draws a texture on a two triangle quad
    /// @param[in] _b the bottom of the quad
    /// @param[in] _t the top of the quad
    /// @param[in] _b the left side of the quad
    /// @param[in] _b the right side of the quad
    /// @param[in] _d the distance of the quad to the camera used for layering textures
    void drawTexture(float _b, float _t, float _l, float _r, float _d);

    /// @brief loads the texture from a file
    /// @param[in] filePath path to the texture file
    /// @param[in] ID  the id of the texture to bind
    void loadTexture(std::string filePath, int ID);

    /// @brief draws a colour on a two triangle quad
    /// @param[in] _b the bottom of the quad
    /// @param[in] _t the top of the quad
    /// @param[in] _b the left side of the quad
    /// @param[in] _b the right side of the quad
    /// @param[in] colour the colour of the quad
    /// @param[in] alpha the transparency of the quad
    void drawQuadColour(float _b, float _t, float _l, float _r, ngl::Vec3 colour, float alpha);

    /// @brief draws the top part of the menu texture
    /// @param[in] move the distance the texture will move upwards
    void drawMenuOpenTop(float move);

    /// @brief draws the bottom part of the menu texture
    /// @param[in] move the distance the texture will move downwards
    void drawMenuOpenBottom(float move);

  private :
    /// @brief the text object for the 'PAUSE' title
    std::unique_ptr <Text> m_textPause;

    /// @brief the text object for the pause options - 'RESUME' and 'MAIN MENU'
    std::unique_ptr <Text> m_textPauseOptions;

    /// @brief an array of textures used for the menus and cutscene
    GLuint m_textureMenuID[401];
};

#endif
