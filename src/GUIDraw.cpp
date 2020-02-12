#include "GUIDraw.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <math.h>
#include <ngl/NGLStream.h>
#include <QGLWidget>
#include <sstream>

GUIDraw::GUIDraw()
{
  m_textPause.reset(new Text("font/arial.ttf",50));
  m_textPauseOptions.reset(new Text("font/arial.ttf",25));

  ngl::NGLInit::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  // creates shader for menu textures
  shader->createShaderProgram("TextureMenu");
  shader->attachShader("TextureMenuVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("TextureMenuFragment",ngl::ShaderType::FRAGMENT);
  shader->loadShaderSource("TextureMenuVertex","shaders/TextureVert.glsl");
  shader->loadShaderSource("TextureMenuFragment","shaders/TextureFrag.glsl");
  shader->compileShader("TextureMenuVertex");
  shader->compileShader("TextureMenuFragment");
  shader->attachShaderToProgram("TextureMenu","TextureVertex");
  shader->attachShaderToProgram("TextureMenu","TextureFragment");
  shader->linkProgramObject("TextureMenu");

  // creates shader for colour used in fade in / out effects
  shader->createShaderProgram("Colour");
  shader->attachShader("ColourVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("ColourFragment",ngl::ShaderType::FRAGMENT);
  shader->loadShaderSource("ColourVertex","shaders/ColourVertex.glsl");
  shader->loadShaderSource("ColourFragment","shaders/ColourFragment.glsl");
  shader->compileShader("ColourVertex");
  shader->compileShader("ColourFragment");
  shader->attachShaderToProgram("Colour","ColourVertex");
  shader->attachShaderToProgram("Colour","ColourFragment");
  shader->linkProgramObject("Colour");
}

GUIDraw::~GUIDraw()
{
  // delete all textures in the texture array
  for (int i=0; i <= 401; i++)
    glDeleteTextures(1,&m_textureMenuID[i]);
}

void GUIDraw::loadAllTextures()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("TextureMenu");

  //load all the frames in the cutscene
  for (int i=1; i<=349; i++)
    {
      std::stringstream ss;
      ss << i;
      std::string m_timeString = ss.str();
      std::string cutscenePath = "textures/introCutscene/cutscene"+ m_timeString + ".png";
      loadTexture(cutscenePath,i+20);
    }

  //load all the frames in the title opening sequence
  for (int i=1; i<=30; i++)
    {
      std::stringstream ss;
      ss << i;
      std::string m_timeString = ss.str();
      std::string cutscenePath = "textures/titleOpening/titleOpening"+ m_timeString + ".png";
      loadTexture(cutscenePath,i+370);
    }


  // load the menu textures and assign them an id
  loadTexture("textures/menu_title.png",1);
  loadTexture("textures/menu_title_text.png",2);
  loadTexture("textures/menu_blank.png",3);
  loadTexture("textures/controlsScreen.png",4);
  loadTexture("textures/quitMenu.png",5);
  loadTexture("textures/button_start.png",6);
  loadTexture("textures/button_start_down.png",7);
  loadTexture("textures/button_controls.png",8);
  loadTexture("textures/button_controls_down.png",9);
  loadTexture("textures/button_quit.png",10);
  loadTexture("textures/button_quit_down.png",11);
  loadTexture("textures/button_yes.png",12);
  loadTexture("textures/button_yes_down.png",13);
  loadTexture("textures/button_no.png",14);
  loadTexture("textures/button_no_down.png",15);
  loadTexture("textures/menuOpenBottom.png",16);
  loadTexture("textures/menuOpenTop.png",17);
}

void GUIDraw::drawOpeningCredits()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[0]);
  drawTexture(0,1,0,1,0);
}

void GUIDraw::drawOpeningCutscene(int cutsceneTime)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[cutsceneTime+20]);
  drawTexture(0,1,0,1,0);
}

void GUIDraw::drawOpeningTitleSequence(int cutsceneTime)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[cutsceneTime+370]);
  drawTexture(0,1,0,1,0);
}

void GUIDraw::drawTitleScreenNoText()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[1]);
  drawTexture(0,1,0,1,0);
}

void GUIDraw::drawTitleScreen()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[2]);
  drawTexture(0,1,0,1,0);
}

void GUIDraw::drawMainMenu(int button)
{
  //draw the menu background
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[3]);
  drawTexture(0,1,0,1,0);

  //draws the buttons on top, pressed button images are drawn instead when mouse is over them
  if (button == 1)
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[7]);
  else
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[6]);
  drawTexture(0.7,0.9,0.3,0.7,0.1);

  if (button == 2)
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[9]);
  else
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[8]);
  drawTexture(0.4,0.6,0.3,0.7,0.1);

  if (button == 3)
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[10]);
  else
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[11]);
  drawTexture(0.1,0.3,0.3,0.7,0.1);
}

void GUIDraw::drawControlsScreen()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[4]);
  drawTexture(0,1,0,1,0);
}

void GUIDraw::drawExitScreen(int button)
{
  //draw the menu background
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[5]);
  drawTexture(0,1,0,1,0);

  //draws the buttons on top, pressed button images are drawn instead when mouse is over them
  if (button == 1)
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[13]);
  else
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[12]);
  drawTexture(0.3,0.5,0.05,0.45,0.1);

  if (button == 2)
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[15]);
  else
    glBindTexture(GL_TEXTURE_2D,m_textureMenuID[14]);
  drawTexture(0.3,0.5,0.55,0.95,0.1);
}

void GUIDraw::drawPauseScreen(int w, int h, bool resumeSelect, bool mainMenuSelect)
{
  // draws 'PAUSE' text
  m_textPause->setColour(1,1,1);
  m_textPause->renderText(0.4*w,0.3*h,"PAUSE");

  //draws 'RESUME' and 'MAIN MENU' texts. They are coloured yellow if the mouse is over them
  if (resumeSelect)
    m_textPauseOptions->setColour(1,1,0);
  else
    m_textPauseOptions->setColour(1,1,1);
  m_textPauseOptions->renderText(0.4*w,0.5*h,"RESUME");
  if (mainMenuSelect)
    m_textPauseOptions->setColour(1,1,0);
  else
    m_textPauseOptions->setColour(1,1,1);
  m_textPauseOptions->renderText(0.4*w,0.6*h,"MAIN MENU");
}

void GUIDraw::drawMenuOpenTop(float move)
{
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[17]);
  //moves the texture upwards
  drawTexture(move,move+1,0,1,0.2);
}

void GUIDraw::drawMenuOpenBottom(float move)
{
  glBindTexture(GL_TEXTURE_2D,m_textureMenuID[16]);
  //moves the texture downwards
  drawTexture(-move,-move+1,0,1,0.3);
}

void GUIDraw::drawTexture(float _b, float _t, float _l, float _r, float _d)
{
  // invert _d and convert ranges of coordinates from (0..1) to (-1..1)
  _d *= -1;
  _b = (_b - 0.5) *2;
  _t = (_t - 0.5) *2;
  _l = (_l - 0.5) *2;
  _r = (_r - 0.5) *2;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("TextureMenu");
  ngl::Mat4 MVP=1;
  shader->setShaderParamFromMat4("MVP",MVP);

  ngl::VertexArrayObject *quad=ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  float* vert = new float[18];	// vertex array
  float* uv = new float[12];	// uv array

  vert[0] =_l; vert[1] =  _t; vert[2] =_d;
  vert[3] = _r; vert[4] =  _t; vert[5] =_d;
  vert[6] = _l; vert[7] = _b; vert[8]= _d;

  vert[9] =_l; vert[10]= _b; vert[11]=_d;
  vert[12] =_r; vert[13]=_t; vert[14]=_d;
  vert[15] =_r; vert[16]= _b; vert[17]=_d;

  uv[0] =0.0; uv[1] =  1.0;
  uv[2] = 1.0; uv[3] =  1.0;
  uv[4] = 0.0; uv[5] = 0.0;

  uv[6] =0.0; uv[7]= 0.0;
  uv[8] =1.0; uv[9]= 1.0;
  uv[10] =1.0; uv[11]= 0.0;

  quad->bind();
  quad->setData(18*sizeof(GLfloat),vert[0]);
  quad->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  quad->setData(12*sizeof(GLfloat),uv[0]);
  quad->setVertexAttributePointer(1,2,GL_FLOAT,0,0);
  quad->setNumIndices(6);
  quad->draw();
  quad->unbind();
  quad->removeVOA();
  delete quad;
  delete uv;

  glDisable(GL_BLEND);
}

void GUIDraw::loadTexture(std::string filePath,int ID)
{
  QImage image;
  //converts std::string to QString so load function will read the string
  QString qFilePath = QString::fromUtf8(filePath.c_str());
  bool loaded=image.load(qFilePath);
  if(loaded == true)
    {
      int width=image.width();
      int height=image.height();
      // note this method is depracted as it uses the Older GLWidget but does work
      image = QGLWidget::convertToGLFormat(image);

      glActiveTexture(GL_TEXTURE0);
      glGenTextures(1,&m_textureMenuID[ID]);
      glBindTexture(GL_TEXTURE_2D,m_textureMenuID[ID]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                   0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
      glGenerateMipmap(GL_TEXTURE_2D); //  Allocate the mipmaps
    }
}

void GUIDraw::drawQuadColour(float _b, float _t, float _l, float _r, ngl::Vec3 colour, float alpha)
{
  // convert ranges of coordinates from (0..1) to (-1..1)
  _b = (_b - 0.5) *2;
  _t = (_t - 0.5) *2;
  _l = (_l - 0.5) *2;
  _r = (_r - 0.5) *2;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Colour");
  ngl::Mat4 MVP=1;
  shader->setShaderParamFromMat4("MVP",MVP);

  ngl::VertexArrayObject *quad=ngl::VertexArrayObject::createVOA(GL_TRIANGLES);
  float* vert = new float[18];	// vertex array
  float* col = new float[24];	// color array

  vert[0] =_l; vert[1] =  _t; vert[2] =-1.0;
  vert[3] = _r; vert[4] =  _t; vert[5] =-1.0;
  vert[6] = _l; vert[7] = _b; vert[8]= -1.0;

  vert[9] =_l; vert[10]= _b; vert[11]=-1.0;
  vert[12] =_r; vert[13]=_t; vert[14]=-1.0;
  vert[15] =_r; vert[16]= _b; vert[17]=-1.0;

  col[0] =colour.m_x; col[1] =colour.m_y; col[2] =colour.m_z; col[3] =alpha;
  col[4] =colour.m_x; col[5] =colour.m_y; col[6] =colour.m_z; col[7] =alpha;
  col[8] =colour.m_x; col[9] =colour.m_y; col[10] =colour.m_z; col[11] =alpha;

  col[12] =colour.m_x; col[13] =colour.m_y; col[14] =colour.m_z; col[15] =alpha;
  col[16] =colour.m_x; col[17] =colour.m_y; col[18] =colour.m_z; col[19] =alpha;
  col[20] =colour.m_x; col[21] =colour.m_y; col[22] =colour.m_z; col[23] =alpha;

  quad->bind();
  quad->setData(18*sizeof(GLfloat),vert[0]);
  quad->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
  quad->setData(24*sizeof(GLfloat),col[0]);
  quad->setVertexAttributePointer(1,4,GL_FLOAT,0,0);
  quad->setNumIndices(6);
  quad->draw();
  quad->unbind();
  quad->removeVOA();
  delete quad;

  glDisable(GL_BLEND);
}
