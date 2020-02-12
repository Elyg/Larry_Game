#include "NGLDraw.h"
#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <math.h>
#include <ngl/Random.h>
#include <ngl/NGLStream.h>
#include "Animation.h"
#include <glm/gtc/noise.hpp>
#include <QGLWidget>


const static float INCREMENT=0.01;
const static float ZOOM=2;
float Sky_RED = 0.64f;
float Sky_GREEN = 0.85f;
float Sky_BLUE = 0.91f;

NGLDraw::NGLDraw()
{

  //initialise the values
  m_debug = false;
  m_rotate= false;
  m_spinXFace=0;
  m_spinYFace=0;

  m_prevRot = 0;
  m_formOne = "sphere";
  m_formTwo = "larry";
  m_lightPosition.set(10,40,0);
  m_lastTime =0;

  m_buttonCooldown = 0;
  m_bridgeDown=true;
  m_gravityFactor = 16;

  //PHYSICS with either debug enabled or disabled (true or false values)
  //create main game objects
  m_physics.reset(new PhysicsWorld(true));
  m_triggerFactory.reset(new TriggerFactory(m_physics.get()));
  m_character.reset(new Character(m_physics.get()));
  m_level.reset(new Level(m_physics.get()));
  m_animation.reset(new Animation());
  m_coinFactory.reset(new CoinFactory());
  m_platformFactory.reset(new PlatformFactory(m_physics.get()));
  m_text.reset(new Text("font/arial.ttf",12));
  m_textHighscore.reset(new Text("font/arial.ttf",20));

  m_highScore.reset(new HighScore());
  m_highScore->highScoreRead();

  glClearColor(Sky_RED, Sky_GREEN, Sky_BLUE, 1.0f);
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::NGLInit::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  // now initialize our new camera
  m_cam = new Camera(m_physics.get());
  //Fake camera to make lights work
  ngl::Vec3 from(0,15,3);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  // now load to our new camera
  ngl::Camera *camcam = new ngl::Camera(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  camcam->setShape(60,(float)720.0/576.0,0.05,350);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  camcam->setEye(ngl::Vec4(10.0,10.0,10.0,1.0));
  ngl::Mat4 iv=camcam->getViewMatrix();
  iv.transpose();
  m_light = new ngl::Light(ngl::Vec3(m_lightPosition.m_x, 60.0f, m_lightPosition.m_z),
                           ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT);
  m_light->setTransform(iv);

  // here we set the light POV camera shape, the aspect is 1 as our
  // texture is square.
  // use the same clip plane as before but set the FOV a bit larger
  // to get slightly better shadows and the clip planes will help
  // to get rid of some of the artefacts
  m_lightCamera.setShape(60,(float)720.0/576.0, 0.05, 350);

  // -------------------------------- Create Main Shaders ---------------------------------//

  //create the shader factory
  ShaderFactory makeShaders;
  makeShaders.createDebugShader();
  makeShaders.createTextureShader();
  makeShaders.createGroundShader();
  shader->use("Ground");
  shader->setShaderParam1f("K", 10.3f);
  shader->setShaderParam1f("Velocity", 0.0005f);
  shader->setShaderParam1f("Amp", 0.03f);
  shader->setShaderParam3f("skyColour", Sky_RED, Sky_GREEN, Sky_BLUE);
  shader->setShaderParamFromVec4("light.diffuse", ngl::Vec4(m_light->getColour().m_r,m_light->getColour().m_g,m_light->getColour().m_b,m_light->getColour().m_a));

  makeShaders.createPhongShader();
  shader->use("Phong");

  ngl::Material m(ngl::STDMAT::GOLD);
  // load our material values to the shader into the structure material (see Vertex shader)
  m.loadToShader("material");
  shader->setShaderParam3f("viewerPos",camcam->getEye().m_x,camcam->getEye().m_y,camcam->getEye().m_z);
  shader->setShaderParamFromVec4("light.position", camcam->getEye());
  shader->setShaderParamFromVec4("light.diffuse", ngl::Vec4(1.0,1.0,1.0,1.0));
  shader->setUniform("light.ambient", ngl::Vec4(1.0,0.9,0.5,1.0));
  shader->setUniform("light.specular", ngl::Vec4(0.5,0.5,0.5,1.0));

  delete camcam;

  shader->setShaderParam3f("skyColour", Sky_RED, Sky_GREEN, Sky_BLUE);
  // set the disintegration thresholds
  shader->setShaderParam1f("LowThreshold", increaseLowThreshold());
  shader->setShaderParam1f("HighThreshold", 0.7);

  GLuint id = shader->getProgramID("Phong");
  // create our links to our various subroutine functions in the fragment shader
  m_subroutines[0] = glGetSubroutineIndex(id, GL_FRAGMENT_SHADER, "recordDepth");
  m_subroutines[1] = glGetSubroutineIndex(id, GL_FRAGMENT_SHADER, "shadeWithShadow");
  m_subroutines[2] = glGetSubroutineIndex(id, GL_FRAGMENT_SHADER, "shadeWithDisintegrate");

  // ------------------------------------------------------------------------------------------//

  // Generate array of multiple textures
  glGenTextures(2, m_TextureID);

  // now create our FBO and texture
  createFramebufferObject();

  makeNoise(15.15f, 1.52f);
  m_coinMesh.reset(new ngl::Obj("models/coin.obj"));
  m_coinMesh->createVAO();

  // Pass textures units to Phong shader
  shader->setShaderParam1i("ShadowMap", 0);
  shader->setShaderParam1i("NoiseMap", 1);

  //set gravity
  m_physics->setGravity(ngl::Vec3(0,(-9.8*m_gravityFactor),0));
  CollisionShape *shapes=CollisionShape::instance();
  //creating level meshes
  m_level->createLevel("level_1", ngl::Vec3(0,0,0));
  m_level->createLevel("level_2Platform", ngl::Vec3(0,0,0));
  m_level->createLevel("level_2_part_1", ngl::Vec3(0,0,0));
  m_level->createLevel("level_2_part_2", ngl::Vec3(0,0,0));
  m_level->createLevel("level_2_part_3a", ngl::Vec3(0,-10,0));
  m_level->createLevel("level_2_part_3b", ngl::Vec3(0,-30,0));
  m_level->createLevel("level_3", ngl::Vec3(0,-60,0));
  m_level->createLevel("level_4_part_1", ngl::Vec3(0,-70,0));
  m_level->createLevel("level_4_part_2", ngl::Vec3(0,-70,0));
  m_level->createLevel("level_5Stick", ngl::Vec3(0,-70,0));
  m_level->createLevel("level_5", ngl::Vec3(0,-70,0));
  m_level->createLevel("finish", ngl::Vec3(0,-70,0));

  shapes->addSphere("camSphere", 2); //collission sphere for camera
  m_character->createCharacter(ngl::Vec3(45,3,0));

  ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
  m_larryMesh.reset(new ngl::Obj("models/sphereChar.obj"));
  m_larryMesh->createVAO();
  prim->createSphere(m_formOne,2,40);
  prim->createTrianglePlane("grid",20,20,200,200,ngl::Vec3(0,1,0));

  //create VAO for level meshes
  m_level1Mesh.reset(new ngl::Obj("models/level_1.obj"));
  m_level1Mesh->createVAO();

  m_level2_1Mesh.reset(new ngl::Obj("models/level_2_part_1.obj"));
  m_level2_1Mesh->createVAO();
  m_level2_2Mesh.reset(new ngl::Obj("models/level_2_part_2.obj"));
  m_level2_2Mesh->createVAO();
  m_level2_3aMesh.reset(new ngl::Obj("models/level_2_part_3a.obj"));
  m_level2_3aMesh->createVAO();
  m_level2_3bMesh.reset(new ngl::Obj("models/level_2_part_3b.obj"));
  m_level2_3bMesh->createVAO();

  m_level3Mesh.reset(new ngl::Obj("models/level_3.obj"));
  m_level3Mesh->createVAO();

  m_level4_1Mesh.reset(new ngl::Obj("models/level_4_part_1.obj"));
  m_level4_1Mesh->createVAO();
  m_level4_2Mesh.reset(new ngl::Obj("models/level_4_part_2.obj"));
  m_level4_2Mesh->createVAO();

  m_level5Stick.reset(new ngl::Obj("models/level_5_sticky.obj"));
  m_level5Stick->createVAO();
  m_level5Mesh.reset(new ngl::Obj("models/level_5.obj"));
  m_level5Mesh->createVAO();
  m_finish.reset(new ngl::Obj("models/finish_line.obj"));
  m_finish->createVAO();

  // load in the animation data for our character
  m_animation->loadAnimationData(animType::CHAR_TYPE);
  // set the inital animation cycle to rest
  m_animation->setAnimationTo(animCycle::REST);
  // attach the character animation data to the character mesh
  m_animation->attachAnimToMesh(m_larryMesh,animType::CHAR_TYPE);

  //load in the animation data for the coins and attach to mesh
  m_animation->loadAnimationData(animType::COIN_TYPE);
  m_animation->setAnimationTo(animCycle::COIN_REST);
  m_animation->attachAnimToMesh(m_coinMesh,animType::COIN_TYPE);


  //create VAOs for the platform meshes
  m_platformMesh_platform.reset(new ngl::Obj("models/platform_mesh.obj"));
  m_platformMesh_platform->createVAO();

  m_platformMesh_bouncePad.reset(new ngl::Obj("models/level_1_bounce_platform.obj"));
  m_platformMesh_bouncePad->createVAO();

  m_platformMesh_barrier01.reset(new ngl::Obj("models/barrier_01.obj"));
  m_platformMesh_barrier01->createVAO();

  m_level4RotPlatformMesh.reset(new ngl::Obj("models/level_4_rotating_platform.obj"));
  m_level4RotPlatformMesh->createVAO();

  m_level4Bridge1Mesh.reset(new ngl::Obj("models/level_4_bridge_half.obj"));
  m_level4Bridge1Mesh->createVAO();
  m_level4Bridge2Mesh.reset(new ngl::Obj("models/level_4_bridge_half2.obj"));
  m_level4Bridge2Mesh->createVAO();

  //if changing size of pshere need to change stuff in addSphere method, the Y property

  shapes->addBox("cube",1.0f,1.0f,1.0f);

  //setup camera collision
  ngl::Vec4 centre(m_physics->getPosition(m_formOne));

  ngl::Vec4 result(centre.m_x + m_cam->getDistance() * cos(m_cam->getAngle()),centre.m_y+10,(centre.m_z + m_cam->getDistance() * sin(m_cam->getAngle())),0);
  m_physics->addSphere("camSphere", result.toVec3(), 0.5, ngl::Vec3(0,0,0), false);
  m_physics->setGravityForBody("camSphere", ngl::Vec3(0,0,0));

  // create main ground plane
  prim->createTrianglePlane("plane",40,40,300,300,ngl::Vec3(0,1,0));

  //text
  (*shader)["nglTextShader"]->use();
  m_text->setColour(1.0,0.0,0.0);

  // create platform positions
  ngl::Vec3 platPos1(-243,-10,-80);
  ngl::Vec3 platPos2(-263,-10,-100);
  ngl::Vec3 platPos3(-223,-10,-110);
  ngl::Vec3 platPos4(-47.984,-20.136,0);
  ngl::Vec3 platPos5(-80.08,-20.136,0);
  ngl::Vec3 platPos6(-152.12,-20.04,-162.931);
  ngl::Vec3 platPos9(-130,-158,-454);
  ngl::Vec3 platPos10(-130,-154,-546);
  ngl::Vec3 platPos11(-130,-154,-610);
  ngl::Vec3 platPos12(-90,-140,-763.7);

  // create platforms by calling the platform factory
  m_platformFactory->createPlatform(platPos1,"movingPlatform01",PlatType::MOVE_PLATE,m_physics.get());

  m_platformFactory->createPlatform(platPos2,"movingPlatform02",PlatType::MOVE_PLATE,m_physics.get());

  m_platformFactory->createPlatform(platPos3,"movingPlatform03",PlatType::MOVE_PLATE,m_physics.get());

  m_platformFactory->createPlatform(platPos12,"movingPlatform04",PlatType::MOVE_PLATE,m_physics.get());

  m_platformFactory->createPlatform(platPos4,"bouncePlatform01",PlatType::BOUNCE_PAD,m_physics.get());

  m_platformFactory->createPlatform(platPos5,"bouncePlatform02",PlatType::BOUNCE_PAD,m_physics.get());

  m_platformFactory->createPlatform(platPos6,"barrierPlatform01",PlatType::BARRIER01,m_physics.get());

  m_platformFactory->createPlatform(platPos9,"rotatingPlatform01",PlatType::ROTATING_L,m_physics.get());

  m_platformFactory->createPlatform(platPos10,"bridge01",PlatType::BRIDGE_1,m_physics.get());

  m_platformFactory->createPlatform(platPos11,"bridge02",PlatType::BRIDGE_2,m_physics.get());

  // read in coin position data from txt file
  m_coinFactory->readCoinPosData();

  // init coin global score to 0
  m_coinFactory->setCoinScore();

  // loop through the number of coins
  for(int i = 0; i < m_coinFactory->getNumCoins(); i++)
    {
      // create coins through the coin factory, assigning the correct position from the coinPosList
      m_coinFactory->createCoin(m_coinFactory->getCoinPos(i),std::to_string(i),m_physics.get());
    }

  //setup triggers
  m_triggerFactory->readTriggerPosData();
  m_triggerFactory->createTriggerFromFile();
  m_coinFactory->readCoinPosData();
}

float NGLDraw::increaseLowThreshold()
{
  return m_lowThresholdKey += 0.05f;
}

float NGLDraw::increaseCoinColour()
{
  return m_coinColour += 0.075;
}
void NGLDraw::resetLowThreshold()
{
  m_lowThresholdKey *= 0.0f;
}

void NGLDraw::resetCoinColour()
{
  m_coinColour *= 0.0f;
}

void NGLDraw::makeNoise(float m_freq, float m_scale)
{
  // ----------------------------- Noise Texture ---------------------------------//
  // set the a and b for freq and scale, we pass these in to the function
  // store 4 noise values in a single 2D texture. store perlin noise with 1 octave
  // in the 1st component(red channel), two octaves in the green channel, 3 blue, 4 alpha
  const static int width_noise_tex = 512;
  const static int height_noise_tex = 512;

  GLubyte *data = new GLubyte[ width_noise_tex * height_noise_tex * 4 ];
  float xFactor = 1.0f / (width_noise_tex - 1);
  float yFactor = 1.0f / (height_noise_tex - 1);
  //animate texture along x axis
  //float incrementFreqPos = increaseLowThreshold();

  for( int row = 0; row < height_noise_tex; row++)
    {
      for(int col = 0; col < width_noise_tex; col++)
        {
          float x = xFactor * col;
          float y = yFactor * row;
          float sum = 0.0f;
          float freq = m_freq;
          float scale = m_scale;

          // Computer sum for each octave of noise
          for( int oct = 0; oct < 4; oct++)
            {
              glm::vec2 p(x * freq, y * freq);
              // use glm library perlin noise, returns float between -1, 1
              float val = glm::perlin(p) / scale;

              sum += val;
              float result = (sum + 1.0f) / 2.0f;
              // Store in texture buffer
              data[((row * width_noise_tex + col) * 4) + oct] = (GLubyte) (result * 255.0f);
              //std::cout << "data noise tex: " << result << std::endl;
              freq *= 2.0f; // Double the freq for each octave
              scale *= m_scale; // Next power of b
            }
        }
    }

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_TextureID[1]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_noise_tex, height_noise_tex, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  std::cout<<"noise tex done"<<std::endl;
  //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_noise_tex, height_noise_tex, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);

  delete [] data;

}
void NGLDraw::shaderType(std::string _shaderName, ngl::Vec3 _shaderColour, bool stripe, float shine, ngl::Transformation &_trans)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  if(_shaderName == "Debug")
    {
      //debug shader
      shader->use("DebugShader");
      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;
      ngl::Mat4 M;
      M = m_mouseGlobalTX;
      MV=  M*m_cam->getViewMatrix();
      MVP= M*m_cam->getVPMatrix();
      normalMatrix=MV;
      normalMatrix.inverse();
      shader->setShaderParamFromMat4("MVP",MVP);

    }
  if(_shaderName == "Shadow")
    {
      //shader for the shadow
      shader->use("Phong");
      GLsizei numActiveUniforms;

      glGetProgramStageiv(shader->getProgramID("Phong"), GL_FRAGMENT_SHADER,
                          GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &numActiveUniforms);
      glUniformSubroutinesuiv(GL_FRAGMENT_SHADER,numActiveUniforms, &m_subroutines[0]);

      ngl::Mat4 MVP = _trans.getMatrix() * m_lightCamera.getVPMatrix();
      shader->setShaderParamFromMat4("MVP",MVP);
    }

  if(_shaderName == "Object")
    {
      //shader for the object
      shader->use("Phong");
      GLsizei numActiveUniforms;

      glGetProgramStageiv(shader->getProgramID("Phong"), GL_FRAGMENT_SHADER,
                          GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &numActiveUniforms);
      glUniformSubroutinesuiv(GL_FRAGMENT_SHADER,numActiveUniforms,&m_subroutines[1]);

      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;
      ngl::Mat4 M;
      M=_trans.getMatrix()*m_mouseGlobalTX;
      MV=  M*m_cam->getViewMatrix();
      MVP= M*m_cam->getVPMatrix();
      normalMatrix=MV;
      normalMatrix.inverse();
      shader->setShaderParamFromMat4("M",M);
      shader->setShaderParamFromMat4("MV",MV);
      shader->setShaderParamFromMat4("MVP",MVP);
      shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
      shader->setUniform("material.diffuse", ngl::Vec4(_shaderColour.m_x, _shaderColour.m_y, _shaderColour.m_z, 1.0));
      shader->setRegisteredUniform1f("material.shininess", shine);
      shader->setShaderParam1i("checker", 0);

      ngl::Mat4 bias;
      bias.scale(0.5,0.5,0.5);
      bias.translate(0.5,0.5,0.5);

      ngl::Mat4 view = m_lightCamera.getViewMatrix();
      ngl::Mat4 proj = m_lightCamera.getProjectionMatrix();
      ngl::Mat4 model = m_transform.getMatrix();

      ngl::Mat4 textureMatrix= model * view * proj * bias;
      shader->setShaderParamFromMat4("textureMatrix",textureMatrix);

      if (stripe == 1)
        {
          shader->setShaderParam1i("stripeBool",1);
        }
      else
        {
          shader->setShaderParam1i("stripeBool",0);
        }
    }
  if(_shaderName == "Disintegrate")
    {
      //disintegrate shader load values
      shader->use("Phong");
      GLsizei numActiveUniforms;
      glGetProgramStageiv(shader->getProgramID("Phong"), GL_FRAGMENT_SHADER,
                          GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &numActiveUniforms);
      glUniformSubroutinesuiv(GL_FRAGMENT_SHADER,numActiveUniforms,&m_subroutines[2]);
      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;
      ngl::Mat4 M;
      M=_trans.getMatrix()*m_mouseGlobalTX;
      MV=  M*m_cam->getViewMatrix();
      MVP= M*m_cam->getVPMatrix();
      normalMatrix=MV;
      normalMatrix.inverse();
      shader->setShaderParamFromMat4("M",M);
      shader->setShaderParamFromMat4("MV",MV);
      shader->setShaderParamFromMat4("MVP",MVP);
      shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
      shader->setUniform("material.diffuse", ngl::Vec4(_shaderColour.m_x, _shaderColour.m_y, _shaderColour.m_z, 1.0));

      if (stripe == 1)
        {
          shader->setShaderParam1i("stripeBool",1);
        }
      else
        {
          shader->setShaderParam1i("stripeBool",0);
        }
      ngl::Mat4 bias;
      bias.scale(0.5,0.5,0.5);
      bias.translate(0.5,0.5,0.5);
      ngl::Mat4 view = m_lightCamera.getViewMatrix();
      ngl::Mat4 proj = m_lightCamera.getProjectionMatrix();
      ngl::Mat4 model = m_transform.getMatrix();
      ngl::Mat4 textureMatrix= model * view * proj * bias;
      shader->setShaderParamFromMat4("textureMatrix",textureMatrix);
      shader->setShaderParam1f("LowThreshold", increaseLowThreshold());
      shader->setShaderParam1f("HighThreshold", 0.7);
    }

  if(_shaderName == "Ground")
    {
      // shader for the ground
      shader->use("Ground");
      ngl::Mat4 MV;
      ngl::Mat4 MVP;
      ngl::Mat3 normalMatrix;
      ngl::Mat4 M;
      M=_trans.getMatrix()*m_mouseGlobalTX;
      MV=  M*m_cam->getViewMatrix();
      MVP= M*m_cam->getVPMatrix();
      normalMatrix=MV;
      normalMatrix.inverse();
      shader->setShaderParamFromMat4("MVP",MVP);
      shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
      shader->setShaderParam4f("inColour", _shaderColour.m_x, _shaderColour.m_y, _shaderColour.m_z, 1.0);
      shader->setShaderParam1i("time", increaseShaderTime());
    }

  if(_shaderName == "Finish")
    {
      ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      shader->setShaderParam1i("checker", 1);
    }
}
constexpr int TEXTURE_WIDTH=1024;
constexpr int TEXTURE_HEIGHT=1024;

NGLDraw::~NGLDraw()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
  delete m_light;
  delete m_cam;

}

void NGLDraw::resize(int _w, int _h)
{
  glViewport(0,0,_w,_h);
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45,(float)_w/_h,0.05,350);
  m_text->setScreenSize(_w,_h);
  m_w = _w;
  m_h = _h;
  draw();
}

void NGLDraw::drawDepth()
{
  // enable culling
  glEnable(GL_CULL_FACE);
  // bind the FBO and render offscreen to the texture
  glBindFramebuffer(GL_FRAMEBUFFER,m_fboID);
  // bind the texture object to 0 (off )
  glBindTexture(GL_TEXTURE_2D,0);
  // we need to render to the same size as the texture to avoid
  // distortions
  glViewport(0,0,TEXTURE_WIDTH,TEXTURE_HEIGHT);

  // Clear previous frame values
  glClear( GL_DEPTH_BUFFER_BIT);
  // as we are only rendering depth turn off the colour / alpha
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  // render only the back faces so we don't get too much self shadowing
  glCullFace(GL_FRONT);
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();
  //shader->setUniform("material.diffuse", ngl::Vec4(1, 1, 0, 1));
  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  //draw character for SHADOW pass
  if(m_character->getPhase() == true)
    {
      m_bodyTransform=m_physics->getTransformMatrix(m_formOne);
      //shader->setUniform("material.diffuse", ngl::Vec4(0, 0, 0.7, 1));
      m_trans_sphere.setMatrix(m_bodyTransform);
      shaderType("Shadow",  ngl::Vec3(0.0, 0.0, 0.0),0,0, m_trans_sphere);
      prim->draw(m_formOne);
      m_bodyTransform.identity();
    }
  else if (m_character->getPhase() == false)
    {
      ngl::Vec3 vel(m_physics->getLinVelocity(m_formTwo));
      ngl::Vec3 pos(m_physics->getPosition(m_formTwo));
      pos[1] -=2.5;

      m_position.setPosition(pos);

      vel[1] =0;
      if(vel.length() != 0 && vel.length() > 4)
        {
          //calculate rotation for character
          vel.normalize();
          m_rot = atan2(vel.m_x, vel.m_z);
          m_rot *=180/ngl::PI;

          if(m_rot < 0.0)
            {
              m_rot+= 360.0f;
            }
          ngl::Vec3 rotator(0,m_rot,0);
          //m_prevRot = comb;
          m_position.setRotation(rotator);
        }
      shaderType("Shadow",  ngl::Vec3(0.0, 0.0, 0.0),0,0, m_position);
      m_larryMesh->draw();
      m_bodyTransform.identity();
    }
  //draw level
  m_trans_level.setMatrix(m_bodyTransform);
  shaderType("Shadow",  ngl::Vec3(0.0, 0.0, 0.0),0,0, m_trans_level);

  //get name of object
  std::string name;
  if(m_character->getPhase() == false)
    {
      name = m_formTwo;
    }
  if (m_character->getPhase() == true)
    {
      name = m_formOne;
    }

  ngl::Vec4 dir(m_cam->getLook() - m_cam->getEye());
  m_character->setCameraInfo(dir);

  m_cam->updatePos(name);
}

void NGLDraw::draw()
{
  //store name of the character
  std::string name;
  if(m_character->getPhase() == false)
    {
      name = m_formTwo;
    }
  if (m_character->getPhase() == true)
    {
      name = m_formOne;
    }

  // set the lightCamera (for depth mapping) position above the character
  ngl::Vec3 to = m_physics->getPosition(name);
  ngl::Vec3 up(1,0,0);
  m_lightPosition = m_physics->getPosition(name);
  m_lightPosition.m_y+=20;
  m_lightPosition.m_x+=5;
  m_lightPosition.m_z+=8;
  m_lightCamera.set(m_lightPosition,to,up);

  // -------- Pass 1 --------------///
  // Pass call the depth pass function
  drawDepth();
  //----------------------------------------------------------------------------------------------------------------------
  // Pass 2 use the texture
  // now we have created the texture for shadows render the scene properly
  // Here we also  bind to the default framebuffer, disable culling, and select the subroutine
  // function shadeWithShadow, and draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  // go back to our normal framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  // set the viewport to the screen dimensions
  // Gareth change this!
  glViewport(0,0,m_w,m_h);
  // glViewport(0, 0, m_w() * devicePixelRatio(), m_h() * devicePixelRatio());
  // enable colour rendering again (as we turned it off earlier)
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // bind the noise texture
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_TextureID[1]);
  // we need to generate the mip maps each time we bind
  glGenerateMipmap(GL_TEXTURE_2D);
  // now only cull back faces
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

  // bind the noise texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_TextureID[0]);

  if(m_character->getPhase() == true)
    {
      m_bodyTransform=m_physics->getTransformMatrix(m_formOne);

      m_trans_sphere.setMatrix(m_bodyTransform);

      // If the character hits the box then it will apply Disintegrate shader
      if(m_triggerFactory->checkPairHit("dis_1", m_formOne)  || m_triggerFactory->checkPairHit("dis_2", m_formOne) ||
         m_triggerFactory->checkPairHit("dis_3", m_formOne))

        {
          //print DEAD
          m_text->setColour(1.0,0.0,0.0);
          m_text->renderText(620,250,"DEAD");
          shaderType("Disintegrate", ngl::Vec3(1.0, 0.5, 0.6),1,200, m_trans_sphere);
        }
      else
        {
          resetLowThreshold();
          shaderType("Object", ngl::Vec3(1.0, 0.5, 0.6),1,200, m_trans_sphere);
        }
      prim->draw(m_formOne);
      m_bodyTransform.identity();
    }
  else if (m_character->getPhase() == false)
    {
      //get character attributes
      ngl::Vec3 vel(m_physics->getLinVelocity(m_formTwo));
      ngl::Vec3 pos(m_physics->getPosition(m_formTwo));
      pos[1] -=2.5;
      m_position.setPosition(pos);

      vel[1] =0;
      if(vel.length() != 0 && vel.length() > 4)
        {
          //calculate the rotation of the second character
          vel.normalize();
          m_rot = atan2(vel.m_x, vel.m_z);
          m_rot *=180/ngl::PI;

          if(m_rot < 0.0)
            {
              m_rot+= 360.0f;
            }
          ngl::Vec3 rotator(0,m_rot,0);
          //m_prevRot = comb;
          m_position.setRotation(rotator);
        }

      // If the character hits the box then it will apply Disintegrate shader
      if( m_triggerFactory->checkPairHit("dis_1", m_formTwo)  || m_triggerFactory->checkPairHit("dis_2", m_formTwo) ||
          m_triggerFactory->checkPairHit("dis_3", m_formTwo))
        {
          //print DEAD
          m_text->setColour(1.0,0.0,0.0);
          m_text->renderText(620,250,"DEAD");

          shaderType("Disintegrate", ngl::Vec3(1.0, 0.5, 0.6),1,200,  m_position);
        }
      else
        {
          resetLowThreshold();
          shaderType("Object", ngl::Vec3(1.0, 0.5, 0.6),1,200, m_position);

        }

      m_larryMesh->draw();
      m_bodyTransform.identity();

    }
  //fix slopes - set gravity to 0 if on a slope to prevent sliding down
  if (m_character->detectCollisionForJump())
    {
      if (m_character->getPhase() == false)
        {
          m_character->setGravityForCharacter(m_formTwo,ngl::Vec3(0,(0),0));
        }
    }
  else
    {
      m_character->setGravityForCharacter(m_formTwo,ngl::Vec3(0,(-9.8*m_gravityFactor),0));
    }

  m_trans_level.setMatrix(m_bodyTransform);
  shaderType("Object", ngl::Vec3(0.3, 0.15, 0.03), 0,600, m_trans_level);

  //draw level meshes
  m_level1Mesh->draw();

  m_level2_1Mesh->draw();
  m_level2_2Mesh->draw();

  //change in position
  m_bodyTransform=m_physics->getTransformMatrix("level_2_part_3a");
  m_trans_level.setMatrix(m_bodyTransform);
  shaderType("Object", ngl::Vec3(0.3, 0.15, 0.03), 0,600, m_trans_level);
  m_level2_3aMesh->draw();

  //change in position
  m_bodyTransform=m_physics->getTransformMatrix("level_2_part_3b");
  m_trans_level.setMatrix(m_bodyTransform);
  shaderType("Object", ngl::Vec3(0.3, 0.15, 0.03), 0,600, m_trans_level);
  m_level2_3bMesh->draw();

  //change in position
  m_bodyTransform=m_physics->getTransformMatrix("level_3");
  m_trans_level.setMatrix(m_bodyTransform);
  shaderType("Object", ngl::Vec3(0.3, 0.15, 0.03), 0,600, m_trans_level);
  m_level3Mesh->draw();

  //change in position
  m_bodyTransform=m_physics->getTransformMatrix("level_4_part_1");
  m_trans_level.setMatrix(m_bodyTransform);
  shaderType("Object", ngl::Vec3(0.3, 0.15, 0.03), 0,600, m_trans_level);
  m_level4_1Mesh->draw();
  m_level4_2Mesh->draw();


  shaderType("Object", ngl::Vec3(1.0, 1.0, 0.5), 0,600, m_trans_level);
  m_level5Stick->draw();
  shaderType("Object", ngl::Vec3(0.3, 0.15, 0.03), 0,600, m_trans_level);
  m_level5Mesh->draw();

  shaderType("Finish", ngl::Vec3(1.0,1.0,1.0), 0,600, m_trans_level); //checkerboard
  m_finish->draw();


  //draw buttons
  ngl::Transformation button = m_triggerFactory->getTransform("level_4_button_0");

  if(m_triggerFactory->checkPairHit("level_4_button_0", name))
    {
      button.addPosition(0,-7,0);
      shaderType("Object", ngl::Vec3(1.0, 0.0, 0.0), 0,600, button);
    }
  else
    {
      button.addPosition(0,-6,0);
      shaderType("Object", ngl::Vec3(0.7, 0.0, 0.0), 0,600, button);
    }
  prim->draw("cube");
  ngl::Transformation button2 = m_triggerFactory->getTransform("level_4_button_1");

  if(m_bridgeDown == false)
    {
      button2.addPosition(0,-7,0);
      shaderType("Object", ngl::Vec3(0.0, 8.0, 0.0), 0,600, button2);
    }
  else
    {
      button2.addPosition(0,-6,0);
      shaderType("Object", ngl::Vec3(0.7, 0.0, 0.0), 0,600, button2);
    }
  prim->draw("cube");

  //m_level1_bouncePlatformMesh->draw();

  //if character touches the box
  if(m_triggerFactory->checkPairHit("death_1", name) ||
     m_triggerFactory->checkPairHit("death_2", name) ||
     m_triggerFactory->checkPairHit("death_3", name) ||
     m_triggerFactory->checkPairHit("death_4", name) ||
     m_triggerFactory->checkPairHit("bounds", name) == false )
    {
      //freezes then resets
      m_character->freezeCharacter();
      m_character->resetCharacter(m_cam, m_level->getInitialPos());
      if(m_bridgeDown == false)
        {
          m_platformFactory->addRot("bridge01", -90);
          m_platformFactory->addRot("bridge02", 90);
          m_bridgeDown = true;
        }
      m_platformFactory->setRot("rotatingPlatform01", 0);
      m_platformFactory->rotatePlatform("rotatingPlatform01", "y");

    }

  //trigger to un-clamp velocity
  if(m_triggerFactory->checkPairHit("ramp_1", m_formOne) || m_triggerFactory->checkPairHit("ramp_2", m_formOne) ||
     m_triggerFactory->checkPairHit("ramp_3", m_formOne) || m_triggerFactory->checkPairHit("uBendVel", m_formOne))
    {
      m_character->setClampFlag(false);
    }
  //trigger to clamp velocity
  if(m_triggerFactory->checkPairHit("ramp_1_end", name) ||
     m_triggerFactory->checkPairHit("ramp_2_end", name) ||
     m_triggerFactory->checkPairHit("stick_4", name) ||
     m_triggerFactory->checkPairHit("stick_5", name))
    {
      m_character-> setClampFlag(true);
    }
  //trigger to speed up ball
  if(m_triggerFactory->checkPairHit("ramp_3", m_formOne))
    {
      m_character-> setClampFlag(false);
      m_character-> speed(m_formOne);
    }
  // bounce pad triggers
  if(m_triggerFactory->checkPairHit("bouncePad_01", m_formOne) || m_triggerFactory->checkPairHit("bouncePad_02", m_formOne)  )
    {
      ngl::Vec3 force;
      force.set(0,100,0);
      m_physics->setLinVelocity(m_formOne,force);
    }

  m_bodyTransform.identity();
  //draw ground
  m_trans_grid.setPosition(0,-200,0);
  shaderType("Ground", ngl::Vec3(0.93, 0.556, 0.14), 0,0, m_trans_grid);
  m_trans_grid.setScale(120.1,120.1,120.1);

  prim->draw("grid");

  //get name of object
  if(m_character->getPhase() == false)
    {
      name = m_formTwo;
    }
  if (m_character->getPhase() == true)
    {
      name = m_formOne;
    }
  //get camera direction vector
  ngl::Vec4 dir(m_cam->getLook() - m_cam->getEye());
  m_character->setCameraInfo(dir);

  if(m_triggerFactory->checkPairHit("camTrigger", name))
    {
      m_cam->forcePos(name, 0);
    }
  else if(m_triggerFactory->checkPairHit("UBend", name))
    {
      m_cam->snapPos(name, ngl::PI/2);
    }
  else
    {
      m_cam->updatePos(name);
    }

  //button triggers
  m_buttonCooldown--;
  if(m_triggerFactory->checkPairHit("level_4_button_0", name) && m_buttonCooldown <= 0)
    {
      m_platformFactory->addRot("rotatingPlatform01", 90);
      m_buttonCooldown = 100;
    }
  //bridge button check collision
  if(m_triggerFactory->checkPairHit("level_4_button_1", name) && m_bridgeDown)
    {
      m_platformFactory->addRot("bridge01", 90);
      m_platformFactory->addRot("bridge02", -90);
      m_bridgeDown=false;
      m_cam->forcePos(name, ngl::PI/2);
      m_cam->setDistance(40);
    }

  //character stuff//

  if(m_triggerFactory->checkPairHit("shiftTrigger_1", name)  || m_triggerFactory->checkPairHit("shiftTrigger_2", name))
    {
      m_character->setShiftable(false);
    }
  else
    {
      m_character->setShiftable(true);
    }

  // set moving platforms to translate
  m_platformFactory->movePlatform("movingPlatform01",10,10,MOVE_X);
  m_platformFactory->movePlatform("movingPlatform02",10,15,MOVE_Z);
  m_platformFactory->movePlatform("movingPlatform03",10,25,MOVE_X);
  m_platformFactory->movePlatform("movingPlatform04",10,40,MOVE_Z);
  m_platformFactory->movePlatform("barrierPlatform01",5,9,MOVE_X);

  // set the bounce platforms to zero velocity
  m_platformFactory->movePlatform("bouncePlatform01",0,30,MOVE_X);
  m_platformFactory->movePlatform("bouncePlatform02",0,30,MOVE_X);
  m_platformFactory->rotatePlatform("rotatingPlatform01", "y");
  m_platformFactory->rotatePlatform("bridge01", "x");
  m_platformFactory->rotatePlatform("bridge02", "x");
  m_physics->freezeBody("rotatingPlatform01");
  m_physics->freezeBody("bridge01");
  m_physics->freezeBody("bridge02");


  // draw platforms
  for(auto i:m_platformFactory->getPlatformList())
    {
      m_bodyTransform=m_physics->getTransformMatrix(i->getPlatformName());
      m_trans_platform.setMatrix(m_bodyTransform);
      if(i->getPlatType() == PlatType::MOVE_PLATE)
        {
          shaderType("Object", ngl::Vec3(1.55, 1.4, 1.0),0,600, m_trans_platform);

          m_platformMesh_platform->draw();
          m_bodyTransform.identity();
        }
      else if(i->getPlatType() == PlatType::BOUNCE_PAD)
        {
          shaderType("Object", ngl::Vec3(0.099, 0.2, 0.1),0,600, m_trans_platform);

          m_platformMesh_bouncePad->draw();
          m_bodyTransform.identity();
        }
      else if(i->getPlatType() == PlatType::BARRIER01)
        {
          shaderType("Object", ngl::Vec3(1.55, 1.4, 1.0),0,600, m_trans_platform);

          m_platformMesh_barrier01->draw();
          m_bodyTransform.identity();
        }
      else if(i->getPlatType() == PlatType::ROTATING_L)
        {
          shaderType("Object", ngl::Vec3(1.55, 1.4, 1.0),0,600, m_trans_platform);

          m_level4RotPlatformMesh->draw();
          m_bodyTransform.identity();
        }
      else if(i->getPlatType() == PlatType::BRIDGE_1)
        {
          shaderType("Object", ngl::Vec3(1.55, 1.4, 1.0),0,600, m_trans_platform);

          m_level4Bridge1Mesh->draw();
          m_bodyTransform.identity();
        }
      else if(i->getPlatType() == PlatType::BRIDGE_2)
        {
          shaderType("Object", ngl::Vec3(1.55, 1.4, 1.0),0,600, m_trans_platform);

          m_level4Bridge2Mesh->draw();
          m_bodyTransform.identity();
        }

    }

  // iterate the coinlist and draw all the coins
  for(auto i:m_coinFactory->getCoins())
    {
      m_trans_coin.setMatrix(m_bodyTransform);
      m_trans_coin.setPosition(i->getCoinPos());
      shaderType("Object", ngl::Vec3(1.4, 0.8, 0.05),0,5000, m_trans_coin);
      // check if any  coins have hit the player

      if((i->isCoinHit(m_formOne) || i->isCoinHit(m_formTwo)))
        {
          // if contact and coinstate is still active
          if(i->getCoinState()  == 1)
            {
              // set coin state to 0/dead
              i->setCoinState(0);
              m_coinFactory->addCoinScore(1);
            }
          // if coin is 0/dead it will do nothing
          else if(i->getCoinState() == 0)
            {
              i->startTime();
              float timer = i->getTime();
              //std::cout << "time " << timer << std::endl;
              shaderType("Object", ngl::Vec3(1.4 + increaseCoinColour(),
                                             0.8 + increaseCoinColour() *0.7,
                                             0.05),
                         0,5000, m_trans_coin);

              //shaderType("Disintegrate", ngl::Vec3(1.0, 0.7, 0.1 + increaseCoinColour()),0,5000, m_trans_coin);
              if((timer) >= 0.1)
                {

                  m_coinFactory->deleteCoin(i);
                  resetCoinColour();
                }
            }
        }
      //draw coins
      m_coinMesh->draw();
      m_bodyTransform.identity();
    }
  //  DRAW TRIGGER VOLUMES
  if(m_debug==true)
    {
      for(int i =0; i<m_triggerFactory->getTriggerListSize(); i++)
        {
          ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
          ngl::Transformation triggerBox = m_triggerFactory->getTransform(i);
          shaderType("Object", ngl::Vec3(1,0,0), true , true, triggerBox);
          prim->draw("cube");
        }
      //render debug text data
      m_text->setColour(1,1,1);
      float width = 0.005;
      m_text->setColour(1,1,0);
      std::string maxVel = std::to_string(m_character->getMaxVelocity());
      std::string multi = std::to_string(m_character->getMultiplier());
      std::string mass = std::to_string(m_character->getMass()+0.5);
      m_text->renderText(width*2*m_w,0.04*m_h,"Current character: ");
      //character one
      if(m_character->getPhase()==true)
        {
          m_textPos = m_physics->getPosition(m_formOne);
          m_textVel = m_physics->getLinVelocity(m_formOne);
          ngl::Vec3 gravityDir = m_character->getGravityDir();
          std::string posX = std::to_string(m_textPos.m_x);
          std::string posY = std::to_string(m_textPos.m_y);
          std::string posZ = std::to_string(m_textPos.m_z);
          std::string velX = std::to_string(m_textVel.m_x);
          std::string velY = std::to_string(m_textVel.m_y);
          std::string velZ = std::to_string(m_textVel.m_z);

          std::string gravityDirX = std::to_string(gravityDir.m_x);
          std::string gravityDirY = std::to_string(gravityDir.m_y);
          std::string gravityDirZ = std::to_string(gravityDir.m_z);

          m_text->setColour(1,0,0);
          m_text->renderText(width*m_w,0.06*m_h,m_formOne);
          m_text->setColour(1,1,0);
          m_text->renderText(width*m_w,0.08*m_h,"POS: ["+posX+", "+posY+", "+posZ+"]");
          m_text->renderText(width*m_w,0.1*m_h,"VEL: ["+velX+", "+velY+", "+velZ+"]");
          m_text->renderText(width*m_w,0.12*m_h,"GRAVITY DIR: ["+gravityDirX+", "+gravityDirY+", "+gravityDirZ+"]");
          m_text->renderText(width*m_w,0.14*m_h,"GROUND RAYTEST: "+std::to_string(m_character->detectCollisionForJump()));
        }
      //character two
      if(m_character->getPhase()==false)
        {
          m_textPos = m_physics->getPosition(m_formTwo);
          m_textVel = m_physics->getLinVelocity(m_formTwo);
          std::string posX = std::to_string(m_textPos.m_x);
          std::string posY = std::to_string(m_textPos.m_y);
          std::string posZ = std::to_string(m_textPos.m_z);
          std::string velX = std::to_string(m_textVel.m_x);
          std::string velY = std::to_string(m_textVel.m_y);
          std::string velZ = std::to_string(m_textVel.m_z);
          m_text->setColour(1,0,0);
          m_text->renderText(width*m_w,0.06*m_h,m_formTwo);
          m_text->setColour(1,1,0);
          m_text->renderText(width*m_w,0.08*m_h,"POS: ["+posX+", "+posY+", "+posZ+"]");
          m_text->renderText(width*m_w,0.1*m_h,"VEL: ["+velX+", "+velY+", "+velZ+"]");
          m_text->renderText(width*m_w,0.12*m_h,"GROUND RAYTEST: "+std::to_string(m_character->detectCollisionForJump()));
          m_text->renderText(width*m_w,0.16*m_h,"DOUBLE JUMP: "+std::to_string(m_character->getDoubleJump()));
        }
      m_text->setColour(0.35,0.72,0.99);
      m_text->renderText(width*m_w,0.18*m_h,"VEL MAX (Y not affected): "+maxVel);
      m_text->renderText(width*m_w,0.2*m_h,"VEL MULT: "+multi);
      m_text->renderText(width*m_w,0.22*m_h,"MASS: "+mass);
      float objCount = m_physics->getNumCollisionObjects();
      std::string objCounter = std::to_string(int(objCount));
      m_text->renderText(width*m_w,0.24*m_h,"Number of collision objects: "+objCounter);
      m_text->setColour(1.00,0.52,0.96);
      m_text->renderText(width*m_w,0.26*m_h,"B,N,M - to debug (draw collision shapes, point, poly)");
      m_text->setColour(0,0.7,0);
      m_text->renderText(width*m_w,0.28*m_h,"W,A,S,D,SPACE - to move, F - to shapeshift");
      m_text->renderText(width*m_w,0.30*m_h,"Keys: 1,2,3,4,5,6 - (Add/Minus mass, maxVel, multiplyVel)");
      m_text->renderText(width*m_w,0.32*m_h,"Keys: 7 - Add a cube");


      //m_triggerFactory->printTriggerList();
    }
  //finish line trigger
  if(m_triggerFactory->checkPairHit("finish", m_formOne) || m_triggerFactory->checkPairHit("finish", m_formOne) ||
     m_triggerFactory->checkPairHit("finish", m_formTwo) || m_triggerFactory->checkPairHit("finish", m_formTwo) )
    {
      //print THE END
      m_highScore->setHighScore();
      m_highScore->highScorePause();
      m_textHighscore->setColour(0.0,1.0,0.0);
      m_textHighscore->renderText(0.5*m_w,0.5*m_h,"THE END");
    }
  //draw highscore and coin score
  int score = m_highScore->getScoreTime();
  int coins = m_coinFactory->getCoinScore();
  m_textHighscore->setColour(1.0,1.0,0.0);
  m_textHighscore->renderText(0.7*m_w,0.06*m_h,"High Score: "+std::to_string( (m_highScore->getHighScore() /1000/60) %60)+" min : "+std::to_string( (m_highScore->getHighScore() /1000)%60)+" sec" );
  m_textHighscore->renderText(0.7*m_w,0.1*m_h,"Current Time: "+std::to_string( ((score /1000)/60) %60)+" min : "+std::to_string( (score /1000)%60)+" sec"  );
  m_textHighscore->setColour(1.0,0.5,0.5);
  m_textHighscore->renderText(0.7*m_w,0.15*m_h,"COINS: "+std::to_string(coins/3));
  //checking if character is on a checkpoint
  m_level->checkpoints(m_character->getPhase());

}

void NGLDraw::createFramebufferObject()
{
  // Try to use a texture depth component
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_TextureID[0]);

  //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  //glBindTexture(GL_TEXTURE_2D, 0);

  // create our FBO
  glGenFramebuffers(1, &m_fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
  // disable the Phong and read buffers as we only want depth
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  // attach our texture to the FBO
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_TextureID[0], 0);

  // switch back to window-system-provided framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void NGLDraw::mouseMoveEvent (const SDL_MouseMotionEvent &_event)
{
  if(m_rotate)
    {
      float diffx=_event.x-m_origX;
      float diffy=_event.y-m_origY;
      //set the camera's angle and height based on the difference in mouse position
      m_cam->setAngle(m_cam->getAngle() + diffx/(ngl::PI*30.0f));
      m_cam->setHeight(m_cam->getHeight() + diffy/8);
      m_origY = _event.y;
      m_origX = _event.x;
    }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mousePressEvent (const SDL_MouseButtonEvent &_event)
{
  // this method is called when the mouse button is pressed in this case we
  // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
  if(_event.button == SDL_BUTTON_LEFT)
    {
      m_origX = _event.x;
      m_origY = _event.y;
      m_rotate =true;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::mouseReleaseEvent (const SDL_MouseButtonEvent &_event)
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event.button == SDL_BUTTON_LEFT)
    {
      m_rotate=false;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLDraw::wheelEvent(const SDL_MouseWheelEvent &_event)
{

  // check the diff of the wheel position (0 means no change)
  if(_event.y > 0)
    {
      m_cam->setDistance( m_cam->getDistance()+ZOOM);
    }
  else if(_event.y <0 )
    {
      m_cam->setDistance( m_cam->getDistance()-ZOOM);
    }

  // check the diff of the wheel position (0 means no change)
  if(_event.x > 0)
    {
      m_cam->setDistance( m_cam->getDistance()-ZOOM);
    }
  else if(_event.x <0 )
    {
      m_cam->setDistance( m_cam->getDistance()+ZOOM);
    }
}

void NGLDraw::updateDraw()
{
  //draw
  this->draw();
}
void NGLDraw::updateDebugDraw()
{
  if(m_debug==true)
    {
      shaderType("Debug",  ngl::Vec3(0.0, 0.0, 0.0),0,0, m_empty_trans);
      m_physics->debugWorldDraw();
    }
}
void NGLDraw::updatePhysics(float time)
{
  //used for stickiness
  // if true enable stickiness
  if(m_triggerFactory->checkPairHit("stick_1", m_formOne) || m_triggerFactory->checkPairHit("stick_2", m_formOne) ||
     m_triggerFactory->checkPairHit("stick_3", m_formOne) || m_triggerFactory->checkPairHit("stick_4", m_formOne)||
     m_triggerFactory->checkPairHit("stick_5", m_formOne) || m_triggerFactory->checkPairHit("stick_6", m_formOne))
    {
      m_character->attractForce(m_gravityFactor,5,m_formOne);
      m_character->setAttractFlag(true);
    }
  else
    {
      //disable stickiness
      m_character->setAttractFlag(false);
    }
  if(m_triggerFactory->checkPairHit("stick_1", m_formTwo) || m_triggerFactory->checkPairHit("stick_2", m_formTwo) ||
     m_triggerFactory->checkPairHit("stick_3", m_formTwo) || m_triggerFactory->checkPairHit("stick_4", m_formTwo) ||
     m_triggerFactory->checkPairHit("stick_5", m_formTwo) || m_triggerFactory->checkPairHit("stick_6", m_formTwo))
    {
      //disable stickiness
      m_character->setAttractFlag(false);
    }
  //reset the gravity
  m_character->attractForce(m_gravityFactor,5,m_formOne);
  //m_level->movePlatform(4,120, 15, LEFTRIGHT, "movingPlatform01");

  m_physics->step(time - m_lastTime, 12, 1.0f/360.0f);
  m_lastTime = time;
}

void NGLDraw::debugPhysicsWorld(bool _value)
{
  // debug drawing
  m_debug = _value;
  if(_value==true)
    {
      CollisionShape *shapes=CollisionShape::instance();
      std::cout<<"Shapes map"<<std::endl;
      shapes->printShapeMap();
      std::cout<<"m_physics bodies"<<std::endl;
      m_physics->printBodies();
    }
}

int NGLDraw::increaseShaderTime()
{
  return m_time += 1;
}

void NGLDraw::addCube()
{
  ngl::Vec3 pos;
  ngl::Random *rng=ngl::Random::instance();
  ngl::Vec3 posRNG=rng->getRandomVec3();
  if(m_character->getPhase()==true)
    {
      pos=m_physics->getPosition(m_formOne);
      pos+=posRNG*6;
    }
  if(m_character->getPhase()==false)
    {
      pos=m_physics->getPosition(m_formTwo);
      pos+=posRNG*6;
    }
  pos.m_y=20;
  m_physics->addBox("cube",pos);
}


void NGLDraw::setGravityFactorPositive()
{
  m_gravityFactor +=1.0f;
  m_physics->setGravity(ngl::Vec3(0,(-9.8*m_gravityFactor),0));
}

void NGLDraw::setGravityFactorNegative()
{
  m_gravityFactor -=1.0f;
  m_physics->setGravity(ngl::Vec3(0,(-9.8*m_gravityFactor),0));
}

float NGLDraw::getGravityFactor(){

  return m_gravityFactor;
}

void NGLDraw::restartGame()
{
  m_character->resetCharacter(m_cam, ngl::Vec3 (45,3,0));
  // need to reset score, coins, checkpoints, etc.
}
void NGLDraw::restartGameValues()
{
  m_highScore->highScoreStart();
  // loop through the number of coins
  for(auto coin: m_coinFactory->getCoins())
    {
      // create coins through the coin factory, assigning the correct position from the coinPosList
      m_coinFactory->deleteCoin(coin);
    }
  // loop through the number of coins
  for(int i = 0; i < m_coinFactory->getNumCoins(); i++)
    {
      // create coins through the coin factory, assigning the correct position from the coinPosList
      m_coinFactory->createCoin(m_coinFactory->getCoinPos(i),std::to_string(i),m_physics.get());
    }
  //reset the bridge and rot platform
  if(m_bridgeDown == false)
    {
      m_platformFactory->addRot("bridge01", -90);
      m_platformFactory->addRot("bridge02", 90);
      m_bridgeDown = true;
    }
  m_platformFactory->setRot("rotatingPlatform01", 0);
  m_platformFactory->rotatePlatform("rotatingPlatform01", "y");
  m_coinFactory->setCoinScore();
}
