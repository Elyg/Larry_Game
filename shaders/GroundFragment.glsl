#version 420 core
#define PI 3.14159265


// @brief[in] the vertex normal
in vec3 surfaceNormal;
// @brief[in] the shadow coord
in vec4 ShadowCoord;
// @brief[in] the vertex visibility
in float visibility;
// @brief[in] the vertex position in camera coords
in vec4 Position;
// @brief[in] the vertex normal in camera coords
in vec3 Normal;

/// @brief our output fragment colour
layout (location=0) out vec4 outColour;


/// @brief light structure
struct Lights
{
  vec4 diffuse;
};


uniform Lights light;
// @param the light position
uniform vec3 LightPosition;
// @param material diffuse colour passed from our program
uniform vec4 inColour;
// @param the background sky colour passed from our program
uniform vec3 skyColour;


vec4 diffAndSpec()
{
    // calculate a basic diffuse specular lighting model
    vec3 n = normalize(Normal);
    vec3 s = normalize(vec3(LightPosition) - vec3(Position));
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect(-s, n);
    float diffuse = max(dot(s,n), 0.0);
    float spec = pow(max(dot(r,v), 0.0), 2.0);
    return light.diffuse * (diffuse + spec);
}

void main ()
{
    // ambient component
    float ambientStr = 0.3;
    vec4 ambient = vec4(ambientStr * light.diffuse * inColour);

    vec4 diffSpec = diffAndSpec() + ambient;

    // mix the final lighting with the visibility through the sky
    outColour = mix(vec4(skyColour, 1.0), diffSpec, visibility);

}
