#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include "Ltimer.h"
#include "Controls.h"
#include "NGLDraw.h"
#include "Animation.h"
#include "GUIDraw.h"
#include "Text.h"
#include <boost/scoped_ptr.hpp>


/// @brief function to quit SDL with error message
/// @param[in] _msg the error message to send
void SDLErrorExit(const std::string &_msg);

/// @brief initialize SDL OpenGL context
SDL_GLContext createOpenGLContext( SDL_Window *window);

boost::scoped_ptr<NGLDraw> nglScene;
void openingCredits(GUIDraw *_gui);
void openingCutscene(GUIDraw *_gui);
void titleScreen(GUIDraw *_gui);
void mainMenu(GUIDraw *_gui);
void controlsScreen(GUIDraw *_gui);
void game(GUIDraw *_gui, Controls *_ctrl, Animation *m_animation, Character *_character, PhysicsWorld *_physics);
void pauseMenu(GUIDraw *_gui, Controls *_ctrl);
void exit(GUIDraw *_gui);

SDL_Window *window;
LTimer fpsTimer;
int countedFrames;
LTimer capTimer;
LTimer animTimer;
LTimer idleTimer;
bool spaceBarPressed;
ngl::Vec3 move;
bool debugValue;
float m_time;
int cutsceneTime;
LTimer titleScreenTimer;
LTimer fadeTimer;
LTimer cutsceneTimer;
bool showTitleScreenText;
bool gamePaused;
std::string gameState;
bool quit;
SDL_Event event;
std::string formOne;
std::string formTwo;
bool characterStopped;

int main(int argc, char** argv)
{

  // Initialize SDL's Video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
      // Or die on error
      SDLErrorExit("Unable to initialize SDL");
    }

  // now get the size of the display and create a window we need to init the video
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  // now create our window
  window=SDL_CreateWindow("LARRY'S GAME",
                          SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED,
                          rect.w/2,
                          rect.h/2,
                          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                          );
  // check to see if that worked or exit
  if (!window)
    {
      SDLErrorExit("Unable to create window");
    }

  // Create our opengl context and attach it to our window

  SDL_GLContext glContext=createOpenGLContext(window);
  if(!glContext)
    {
      SDLErrorExit("Problem creating OpenGL context");
    }
  // make this our current GL context (we can have more than one window but in this case not)
  SDL_GL_MakeCurrent(window, glContext);
  /* This makes our buffer swap syncronized with the monitor's vertical refresh */
  SDL_GL_SetSwapInterval(1);
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  // now clear the screen and swap whilst NGL inits (which may take time)
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  // flag to indicate if we need to exit
  quit=false;
  // now we create an instance of our ngl class, this will init NGL and setup basic
  // opengl stuff ext. When this falls out of scope the dtor will be called and cleanup
  // our gl stuff

  nglScene.reset(new NGLDraw);
  PhysicsWorld *physics = nglScene->getPhysicsWorld();
  Character *character = nglScene->getCharacter();
  Controls ctrl(physics, character);
  Animation *m_animation = nglScene->getAnimation();
  GUIDraw gui;

  // resize the ngl to set the screen size and camera stuff
  nglScene->resize(rect.w,rect.h);

  //Start counting frames per second
  countedFrames = 0;
  fpsTimer.start();
  animTimer.start();
  formOne = "sphere";
  formTwo = "larry";
  characterStopped = true;

  spaceBarPressed = false;

  ngl::Vec3 move(0,0,0);
  debugValue = false;
  m_time =0;

  SDL_SetWindowMinimumSize(window, 640,360);

  cutsceneTime = 0;
  showTitleScreenText = false;
  gamePaused = false;

  //initial game state
  gameState = "openingCredits";

  fadeTimer.start();

  //going through game states
  while(!quit)
    {
      if (gameState == "openingCredits")
        openingCredits(&gui);
      else if (gameState == "openingCutscene")
        openingCutscene(&gui);
      else if (gameState == "titleScreen")
        titleScreen(&gui);
      else if (gameState == "mainMenu")
        mainMenu(&gui);
      else if (gameState == "controlsScreen")
        controlsScreen(&gui);
      else if (gameState == "game")
        game(&gui, &ctrl, m_animation, character, physics);
      else if (gameState == "pauseMenu")
        pauseMenu(&gui, &ctrl);
      else if (gameState == "exit")
        exit(&gui);

    } //end while !quit loop

  // now tidy up and exit SDL
  SDL_Quit();
  return 0;

}

void openingCredits(GUIDraw *_gui)
{
  _gui->loadTexture("textures/openingCredits.png",0);
  int w,h;
  SDL_GetWindowSize(window,&w,&h);
  nglScene->resize(w,h);

  fadeTimer.start();
  while (fadeTimer.getTicks()/500.f < 1.0)
    {
      _gui->drawOpeningCredits();
      _gui->drawQuadColour(0,1,0,1,{0.0,0.0,0.0}, 1-fadeTimer.getTicks()/500.f);
      SDL_GL_SwapWindow(window);
    }

  _gui->loadAllTextures();
  gameState = "openingCutscene";

  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;
        default : break;
        } // end of event switch
    } // end of poll events

  fadeTimer.start();
  while (fadeTimer.getTicks()/500.f < 1.0)
    {
      _gui->drawOpeningCredits();
      _gui->drawQuadColour(0,1,0,1,{0.0,0.0,0.0}, fadeTimer.getTicks()/500.f);
      SDL_GL_SwapWindow(window);
    }

  fadeTimer.start();
  while (fadeTimer.getTicks()/1000.f < 0.6)
    {
      _gui->drawQuadColour(0,1,0,1,{0.0,0.0,0.0}, 1);
      SDL_GL_SwapWindow(window);
    }
  cutsceneTimer.start();

}

void openingCutscene(GUIDraw *_gui)
{
  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;
        case SDL_MOUSEBUTTONDOWN: gameState = "titleScreen"; break;
        case SDL_KEYDOWN:
          {
            switch( event.key.keysym.sym )
              {
              case SDLK_ESCAPE :  quit = true; break;
              case SDLK_RETURN : gameState = "titleScreen"; break;
              } // end of key process
          }  // end of keydown
        default : break;
        } // end of event switch
    } // end of poll events

  cutsceneTime = (cutsceneTimer.getTicks()/1000.f*24)+1;
  if (cutsceneTime < 349)
    _gui->drawOpeningCutscene(cutsceneTime);
  else _gui->drawOpeningCutscene(349);
  if (cutsceneTime > 369)
    gameState = "titleScreen";

  if (gameState == "titleScreen")
    fadeTimer.start();

  if (cutsceneTime > 349)
    _gui->drawQuadColour(0,1,0,1,{1.0,1.0,1.0}, ((20-(369-cutsceneTime))/20.0));

  SDL_GL_SwapWindow(window);
}

void titleScreen(GUIDraw *_gui)
{
  ngl::Vec2 pos;
  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;

        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;

        case SDL_KEYDOWN:
          {
            switch( event.key.keysym.sym )
              {
              case SDLK_ESCAPE :  quit = true; break;
              case SDLK_RETURN : gameState = "mainMenu"; break;
              } // end of key process
          }  // end of keydown
        default : break;
        } // end of event switch
    } // end of poll events

  cutsceneTime = (cutsceneTimer.getTicks()/1000.f*24)+1;
  if (cutsceneTime < 30)
    _gui->drawOpeningTitleSequence(cutsceneTime);
  else
    {

      if (titleScreenTimer.getTicks() == 0)
        titleScreenTimer.start();

      if (titleScreenTimer.getTicks() > 400)
        {
          titleScreenTimer.start();
          if (showTitleScreenText == false)
            showTitleScreenText = true;
          else
            showTitleScreenText = false;
        }

      if (showTitleScreenText == false)
        {
          int w,h;
          SDL_GetWindowSize(window,&w,&h);
          _gui->drawTitleScreen();
        }
      else
        _gui->drawTitleScreenNoText();

      if (fadeTimer.getTicks()/1000.f < 1.0)
        _gui->drawQuadColour(0,1,0,1,{1.0,1.0,1.0}, 1-(fadeTimer.getTicks()/1000.f));

    }
  SDL_GL_SwapWindow(window);

  if (gameState=="mainMenu")
    {
      fadeTimer.start();
      while (fadeTimer.getTicks()/500.f < 1.0)
        {
          _gui->drawTitleScreenNoText();
          _gui->drawQuadColour(0,1,0,1,{0.0,0.0,0.0}, fadeTimer.getTicks()/500.f);
          SDL_GL_SwapWindow(window);

        }
      fadeTimer.start();
    }

}

void mainMenu(GUIDraw *_gui)
{
  int button;
  ngl::Vec2 pos;
  int x,y;
  SDL_GetMouseState(&x,&y);
  int w,h;
  SDL_GetWindowSize(window,&w,&h);

  pos = ngl::Vec2(x,y);
  pos.m_x = pos.m_x/w;
  pos.m_y = pos.m_y/h;
  ngl::Vec2 pos1_1,pos1_2,pos2_1,pos2_2,pos3_1,pos3_2;
  pos1_1 = {0.3,0.1};
  pos1_2 = {0.7,0.3};
  pos2_1 = {0.3,0.4};
  pos2_2 = {0.7,0.6};
  pos3_1 = {0.3,0.7};
  pos3_2 = {0.7,0.9};
  if((pos.m_x > pos1_1.m_x && pos.m_x < pos1_2.m_x) && (pos.m_y > pos1_1.m_y && pos.m_y < pos1_2.m_y))
    button = 1;
  else if ((pos.m_x > pos2_1.m_x && pos.m_x < pos2_2.m_x) && (pos.m_y > pos2_1.m_y && pos.m_y < pos2_2.m_y))
    button = 2;
  else if ((pos.m_x > pos3_1.m_x && pos.m_x < pos3_2.m_x) && (pos.m_y > pos3_1.m_y && pos.m_y < pos3_2.m_y))
    button = 3;
  else
    button = 0;

  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;

        case SDL_WINDOWEVENT :
          int w,h;
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;

        case SDL_MOUSEBUTTONDOWN:
          {
            int w,h;
            SDL_GetWindowSize(window,&w,&h);

            pos = ngl::Vec2(event.motion.x, event.motion.y);
            pos.m_x = pos.m_x/w;
            pos.m_y = pos.m_y/h;

            if((pos.m_x > pos1_1.m_x && pos.m_x < pos1_2.m_x) && (pos.m_y > pos1_1.m_y && pos.m_y < pos1_2.m_y))
              {
                gameState = "game";
                nglScene->restartGame();
              }
            else if ((pos.m_x > pos2_1.m_x && pos.m_x < pos2_2.m_x) && (pos.m_y > pos2_1.m_y && pos.m_y < pos2_2.m_y))
              gameState = "controlsScreen";
            else if ((pos.m_x > pos3_1.m_x && pos.m_x < pos3_2.m_x) && (pos.m_y > pos3_1.m_y && pos.m_y < pos3_2.m_y))
              gameState = "exit";
          } break;

        case SDL_KEYDOWN:
          {
            switch( event.key.keysym.sym )
              {
              case SDLK_ESCAPE :  quit = true; break;
              } // end of key process
          }  // end of keydown
        default : break;
        } // end of event switch
    } // end of poll events

  SDL_GetWindowSize(window,&w,&h);
  _gui->drawMainMenu(button);
  if (fadeTimer.getTicks()/500.f < 1.0)
    _gui->drawQuadColour(0,1,0,1,{0.0,0.0,0.0}, 1-(fadeTimer.getTicks()/500.f));
  SDL_GL_SwapWindow(window);

  if (gameState=="game")
    {
      fadeTimer.start();
      while (fadeTimer.getTicks()/500.f < 1.0)
        {
          _gui->drawMenuOpenTop(0);
          _gui->drawMenuOpenBottom(0);
          SDL_GL_SwapWindow(window);
        }
      fadeTimer.start();
      nglScene->restartGameValues();
    }
}

void controlsScreen(GUIDraw *_gui)
{
  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;
        case SDL_MOUSEBUTTONDOWN : gameState = "mainMenu"; break;
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;
        case SDL_KEYDOWN:
          {
            switch( event.key.keysym.sym )
              {
              case SDLK_ESCAPE :  quit = true; break;
              } // end of key process
          }  // end of keydown
        default :
          // if a key is pressed stop idleTimer and reset it
          idleTimer.stop();
          idleTimer.reset();
          break;
        } // end of event switch
    } // end of poll events
  _gui->drawControlsScreen();

  // swap the buffers
  SDL_GL_SwapWindow(window);
}

void game(GUIDraw *_gui, Controls *_ctrl, Animation *m_animation, Character *_character, PhysicsWorld *_physics)
{
  gamePaused = false;
  capTimer.start();
  while( SDL_PollEvent( &event ) ){
      switch( event.type ){
        // this is the window x being clicked.
        case SDL_QUIT : quit = true; break;
          // process the mouse data by passing it to ngl class
        case SDL_MOUSEMOTION : nglScene->mouseMoveEvent(event.motion); break;
        case SDL_MOUSEBUTTONDOWN : nglScene->mousePressEvent(event.button); break;
        case SDL_MOUSEBUTTONUP : nglScene->mouseReleaseEvent(event.button); break;
        case SDL_MOUSEWHEEL : nglScene->wheelEvent(event.wheel); break;
          // if the window is re-sized pass it to the ngl class to change gl viewport
          // note this is slow as the context is re-create by SDL each time
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;

          // Look for a keypress
        case SDL_KEYDOWN:
          //debug drawing
          if((event.key.keysym.sym & SDLK_b) && (event.key.keysym.mod & KMOD_CTRL) && (event.key.keysym.mod & KMOD_SHIFT))
            {
              if(debugValue == false)
                {
                  debugValue = true;
                  if(debugValue == true)
                    {
                      nglScene->debugPhysicsWorld(true);
                      glEnable(GL_BLEND);
                      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                    }
                }
              else
                {
                  debugValue = false;
                  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                  nglScene->debugPhysicsWorld(false);
                  glDisable(GL_BLEND);

                }
            }
          // Check the SDLKey values and move change the coords
          switch( event.key.keysym.sym )
            {
            case SDLK_a:
              move[0] =-1;
              break;
            case SDLK_d:
              move[0] = 1;
              break;
            case SDLK_w:
              move[2] =-1;
              break;
            case SDLK_s:
              move[2] = 1;
              break;
            case SDLK_SPACE:
              {
                _ctrl->characterMovement(move);
                if (spaceBarPressed == false)
                  _ctrl->startJumpHopTimer();
                spaceBarPressed = true;
              }
              break;
            case SDLK_RETURN : gamePaused = true; break;
            case SDLK_ESCAPE :  quit = true; break;
            case SDLK_c : _ctrl->godMode(); break;// C - test stuff
            case SDLK_f : _ctrl->shapeshift(); break; // F - shapeshift
            default:
              break;
            }
          break;
        case SDL_KEYUP:
          switch( event.key.keysym.sym ){
            case SDLK_a:
              if( move[0] < 0 )
                move[0] = 0;
              break;
            case SDLK_d:
              if( move[0] > 0 )
                move[0] = 0;
              break;
            case SDLK_w:
              if( move[2] < 0 )
                move[2] = 0;
              break;
            case SDLK_s:
              if( move[2] > 0 )
                move[2] = 0;
              break;
            case SDLK_SPACE:
              _ctrl->stopJumpHopTimer();
              spaceBarPressed = false;

            default:
              break;
            }
          break;
        default:
          // if no key is pressed, start idleTimer
          idleTimer.start();
          break;
        }
    }

  if(_character->getPhase() == false)
    {
      //animation loops based on the velocity of the character
      ngl::Vec3 velocity = _physics->getLinVelocity(formTwo);
      velocity -= _character->getPlatformVelocity();
      float x = velocity[0];
      float z = velocity[2];

      ngl::Vec2 velocityXZ = ngl::Vec2(x, z);
      float speed = velocityXZ.length();

      if ((speed > 3) && (_character->detectCollisionForJump()))
        {
          m_animation->setAnimationTo(animCycle::WALK);
          m_animation->setAnimUpdate(1,animType::CHAR_TYPE);
          characterStopped = false;
        }
      if ((speed < 3) && (_character->detectCollisionForJump()))
        {
          if (characterStopped == false)
            {
              m_animation->setAnimationTo(animCycle::REST);
              m_animation->initStartFrame();
              m_animation->setAnimUpdate(1,animType::CHAR_TYPE);
              idleTimer.start();
              characterStopped = true;
            }
        }
      if ((_ctrl->getJumpHopTime() > 0) && (_ctrl->getJumpHopTime() < 10))
        {
          m_animation->setAnimationTo(animCycle::JUMP);
          m_animation->initStartFrame();
          m_animation->setAnimUpdate(1,animType::CHAR_TYPE);

        }

      if (_character->detectCollisionForJump() == false)
        characterStopped = false;
    }

  m_animation->setAnimationLoopOn((m_animation->getAnimUpdate(animType::CHAR_TYPE)), animType::CHAR_TYPE);
  m_animation->setAnimationLoopOn((m_animation->getAnimUpdate(animType::COIN_TYPE)), animType::COIN_TYPE);
  _ctrl->characterMovement(move);

  float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
  if( avgFPS > 2000000 )
    {
      avgFPS = 0;
    }

  m_time = fpsTimer.getTicks() / 1000.0f;

  // if Idle for greater than 5 seconds, set character animation to different rest animation

  if(idleTimer.getTicks()/1000.0f > 5.0)
    {
      m_animation->setAnimationTo(animCycle::REST2);
      m_animation->setAnimUpdate(1,animType::CHAR_TYPE);
    }

  if(idleTimer.getTicks()/1000.0f > 9.35)
    {
      m_animation->setAnimationTo(animCycle::REST);
      m_animation->initStartFrame();
      m_animation->setAnimUpdate(1,animType::CHAR_TYPE);
      idleTimer.start();
    }


  // controls the amount frames to play each second through a mod operation, to be used when vsync is off
  // if the result the games's fps mod animationFPS is equal to 0, the next frame is incremented, else
  // we delay the change in frame
  int animTimerSeconds = m_time * avgFPS;
  int animFPS = 60;
  int animAvgFPS = ((int)avgFPS/(animFPS))+1;

  if(animTimerSeconds%animAvgFPS == 0)
    {
      m_animation->incrCurrentFrame(animType::CHAR_TYPE);
      m_animation->incrCurrentFrame(animType::COIN_TYPE);
    }
  nglScene->updateDraw();
  nglScene->updateDebugDraw();
  nglScene->updatePhysics(m_time);
  ++countedFrames;



  if (gamePaused == true)
    gameState = "pauseMenu";


  if (fadeTimer.getTicks()/500.f < 1.0)
    {
      _gui->drawMenuOpenTop(fadeTimer.getTicks()/500.f);
      _gui->drawMenuOpenBottom(fadeTimer.getTicks()/500.f);
    }

  SDL_GL_SwapWindow(window);
}

void pauseMenu(GUIDraw *_gui, Controls *_ctrl)
{
  ngl::Vec2 pos;
  bool resumeSelect, mainMenuSelect;
  int x,y;
  SDL_GetMouseState(&x,&y);
  int w,h;
  SDL_GetWindowSize(window,&w,&h);

  pos = ngl::Vec2(x,y);
  pos.m_x = pos.m_x/w;
  pos.m_y = pos.m_y/h;
  ngl::Vec2 pos1, pos2;
  pos1 = {0.38,0.48};
  pos2 = {0.50,0.55};
  if((pos.m_x > pos1.m_x && pos.m_x < pos2.m_x) && (pos.m_y > pos1.m_y && pos.m_y < pos2.m_y))
    resumeSelect = true;
  else
    resumeSelect = false;
  pos1 = {0.38,0.58};
  pos2 = {0.53,0.65};
  if((pos.m_x > pos1.m_x && pos.m_x < pos2.m_x) && (pos.m_y > pos1.m_y && pos.m_y < pos2.m_y))
    mainMenuSelect = true;
  else
    mainMenuSelect = false;

  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;
        case SDL_WINDOWEVENT :
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;
        case SDL_MOUSEBUTTONDOWN:
          {
            pos1 = {0.38,0.48};
            pos2 = {0.50,0.55};
            if((pos.m_x > pos1.m_x && pos.m_x < pos2.m_x) && (pos.m_y > pos1.m_y && pos.m_y < pos2.m_y))
              gameState = "game";
            pos1 = {0.38,0.58};
            pos2 = {0.53,0.65};
            if((pos.m_x > pos1.m_x && pos.m_x < pos2.m_x) && (pos.m_y > pos1.m_y && pos.m_y < pos2.m_y))
              gameState = "mainMenu";
          } break;
        case SDL_KEYDOWN:
          {
            switch( event.key.keysym.sym )
              {
              case SDLK_RETURN : gamePaused = false; break;
              }
          }
        } //end of event switch
    } //end of poll events

  nglScene->updateDraw();
  _gui->drawQuadColour(0,1,0,1,{0.0,0.0,0.0}, 0.7);
  nglScene->updateDebugDraw();

  SDL_GetWindowSize(window,&w,&h);
  _gui->drawPauseScreen(w,h,resumeSelect,mainMenuSelect);

  SDL_GL_SwapWindow(window);

  if (gamePaused == false)
    {
      gameState = "game";

      //fixes problem where character moves without input after unpausing
      if( move[0] < 0 )
        move[0] = 0;
      if( move[0] > 0 )
        move[0] = 0;
      if( move[2] < 0 )
        move[2] = 0;
      if( move[2] > 0 )
        move[2] = 0;
      //fixes minor problem where character cant double jump after unpausing
      _ctrl->stopJumpHopTimer();
      spaceBarPressed = false;
    }
}

void exit(GUIDraw *_gui)
{
  int button;
  ngl::Vec2 pos;
  int x,y;
  SDL_GetMouseState(&x,&y);
  int w,h;
  SDL_GetWindowSize(window,&w,&h);

  pos = ngl::Vec2(x,y);
  pos.m_x = pos.m_x/w;
  pos.m_y = pos.m_y/h;
  ngl::Vec2 pos1_1,pos1_2,pos2_1,pos2_2;
  pos1_1 = {0.05,0.5};
  pos1_2 = {0.45,0.7};
  pos2_1 = {0.55,0.5};
  pos2_2 = {0.95,0.7};
  if((pos.m_x > pos1_1.m_x && pos.m_x < pos1_2.m_x) && (pos.m_y > pos1_1.m_y && pos.m_y < pos1_2.m_y))
    button = 1;
  else if ((pos.m_x > pos2_1.m_x && pos.m_x < pos2_2.m_x) && (pos.m_y > pos2_1.m_y && pos.m_y < pos2_2.m_y))
    button = 2;
  else
    button = 0;
  while ( SDL_PollEvent(&event) )
    {
      switch (event.type)
        {
        case SDL_QUIT : quit = true; break;
        case SDL_WINDOWEVENT :
          int w,h;
          // get the new window size
          SDL_GetWindowSize(window,&w,&h);
          nglScene->resize(w,h);
          break;
        case SDL_MOUSEBUTTONDOWN:
          {
            int w,h;
            SDL_GetWindowSize(window,&w,&h);

            pos = ngl::Vec2(event.motion.x, event.motion.y);
            pos.m_x = pos.m_x/w;
            pos.m_y = pos.m_y/h;

            if((pos.m_x > pos1_1.m_x && pos.m_x < pos1_2.m_x) && (pos.m_y > pos1_1.m_y && pos.m_y < pos1_2.m_y))
              quit = true;
            else if ((pos.m_x > pos2_1.m_x && pos.m_x < pos2_2.m_x) && (pos.m_y > pos2_1.m_y && pos.m_y < pos2_2.m_y))
              gameState = "mainMenu";
          } break;
        } //end of event switch
    } //end of poll events

  _gui->drawExitScreen(button);
  SDL_GL_SwapWindow(window);
}


SDL_GLContext createOpenGLContext(SDL_Window *window)
{
  // Request an opengl 3.2 context first we setup our attributes, if you need any
  // more just add them here before the call to create the context
  // SDL doesn't have the ability to choose which profile at this time of writing,
  // but it should default to the core profile
  // for some reason we need this for mac but linux crashes on the latest nvidia drivers
  // under centos
#ifdef __APPLE__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


  // set multi sampling else we get really bad graphics that alias
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);
  // Turn on double buffering with a 24bit Z buffer.
  // You may need to change this to 16 or 32 for your system
  // on mac up to 32 will work but under linux centos build only 16
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  // enable double buffering (should be on by default)
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  return SDL_GL_CreateContext(window);

}

void SDLErrorExit(const std::string &_msg)
{
  std::cerr<<_msg<<"\n";
  std::cerr<<SDL_GetError()<<"\n";
  SDL_Quit();
  exit(EXIT_FAILURE);
}
