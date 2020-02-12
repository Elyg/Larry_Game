#include "ShaderFactory.h"

ShaderFactory::ShaderFactory()
{}
ShaderFactory::~ShaderFactory()
{

}
void ShaderFactory::createDebugShader()
{
  //DEBUG SHADER
  ngl::ShaderLib *shaderDebug=ngl::ShaderLib::instance();
  shaderDebug->createShaderProgram("DebugShader");
  // now we are going to create empty shaders for Frag and Vert
  shaderDebug->attachShader("DebugShaderVertex",ngl::ShaderType::VERTEX);
  shaderDebug->attachShader("DebugShaderFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shaderDebug->loadShaderSource("DebugShaderVertex","shaders/DebugShaderVertex.glsl");
  shaderDebug->loadShaderSource("DebugShaderFragment","shaders/DebugShaderFragment.glsl");
  // compile the shaders
  shaderDebug->compileShader("DebugShaderVertex");
  shaderDebug->compileShader("DebugShaderFragment");
  // add them to the program
  shaderDebug->attachShaderToProgram("DebugShader","DebugShaderVertex");
  shaderDebug->attachShaderToProgram("DebugShader","DebugShaderFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shaderDebug->bindAttribute("DebugShader",0,"inPos");
  // attribute 1 is the UV data u,v (if present)
  shaderDebug->bindAttribute("DebugShader",1,"inColour");
  // now we have associated this data we can link the shader
  shaderDebug->linkProgramObject("DebugShader");
}

void ShaderFactory::createTextureShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Texture
  shader->createShaderProgram("Texture");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("TextureVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("TextureFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("TextureVertex","shaders/TextureVert.glsl");
  shader->loadShaderSource("TextureFragment","shaders/TextureFrag.glsl");
  // compile the shaders
  shader->compileShader("TextureVertex");
  shader->compileShader("TextureFragment");
  // add them to the program
  shader->attachShaderToProgram("Texture","TextureVertex");
  shader->attachShaderToProgram("Texture","TextureFragment");
  // now we have associated this data we can link the shader
  shader->linkProgramObject("Texture");
}


void ShaderFactory::createGroundShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Ground for lava/water effects
  shader->createShaderProgram("Ground");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("GroundVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("GroundFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("GroundVertex","shaders/GroundVertex.glsl");
  shader->loadShaderSource("GroundFragment","shaders/GroundFragment.glsl");
  // compile the shaders
  shader->compileShader("GroundVertex");
  shader->compileShader("GroundFragment");
  // add them to the program
  shader->attachShaderToProgram("Ground","GroundVertex");
  shader->attachShaderToProgram("Ground","GroundFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Ground",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Ground",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Ground",2,"inNormal");
  // now we have associated this data we can link the shader
  shader->linkProgramObject("Ground");
}

void ShaderFactory::createPhongShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");
  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
}
