#ifndef SHADERFACTORY_H__
#define SHADERFACTORY_H__
#include <ngl/ShaderLib.h>
//-------------------------------------------------------------------------------------------------------------------------
/// @file shaderFactory.h
/// @author Elisabeth Wetchy
/// @version 1.0
/// @class ShaderFactory
/// @brief This class encapsulates the creation of all shader programs, loading and attaching of the sources, linking and compiling
//--------------------------------------------------------------------------------------------------------------------------
class ShaderFactory
{
public:
  ShaderFactory();
  ~ShaderFactory();

  void createDebugShader();
  void createTextureShader();
  void createGroundShader();
  void createPhongShader();
};

#endif
