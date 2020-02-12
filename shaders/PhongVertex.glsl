#version 420 core
// @brief The vertex passed in
layout (location=0) in  vec3  inVert;
// @brief The UV coordinates passed in
layout (location=1) in vec2 inUV;
// @brief The UV coordinates passed in
layout (location=2) in  vec3  inNormal;

/// modified from the OpenGL Shading Language Example "Orange Book"
/// Roost 2002

out vec4 ShadowCoord;
// @brief The vector from camera position to vertex position in world space
out vec3 eyeDirection;
// @brief The vector from light position to vertex position
out vec3 lightDir;
// @brief The vector halfway between the eyeDirection and lightDir
out vec3 halfVector;
// @brief Visibility of vertex through the fog
out float visibility;
out vec3 surfaceNormal;
out vec2 vertexUV;

struct Lights
{
        vec4 position;
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        float constantAttenuation;
        float spotCosCutoff;
        float quadraticAttenuation;
        float linearAttenuation;
};

// @brief camera position
uniform vec3 viewerPos;
uniform Lights light;
uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;
uniform mat3 normalMatrix;
uniform mat4 textureMatrix;

// @brief Turn stripes on the object on or off
uniform int stripeBool;
// @brief Turn stripes on the object on or off
flat out int stripeBoolFrag;

// @brief Fog density
const float density = 0.005;
// @brief Fog gradient
const float gradient = 3.0;

void main()
{
    // @brief The vertex in camera coordinates
    vec4 eyeCord = MV * vec4(inVert, 1.0);
    // @brief The vertex in world coordinates
    vec4 worldPosition = M * vec4(inVert, 1.0);
    eyeDirection = normalize(viewerPos - worldPosition.xyz);
    surfaceNormal = normalize(normalMatrix * inNormal);
    lightDir = vec3(light.position.xyz - eyeCord.xyz);
    normalize(lightDir);

    halfVector = normalize(eyeDirection + lightDir);

    // create our shadow coordinates
    ShadowCoord = textureMatrix * vec4(inVert, 1.0);

    gl_Position = MVP * vec4(inVert, 1.0);

    vertexUV = inUV;

    // ---------------- FOG ------------------- //
    vec4 positionRelativeToCamera = gl_Position;
    float distance = length(positionRelativeToCamera.xyz);
    //calc exponential graph
    visibility = exp(-pow((distance * density),gradient));
    //clamp between 0 and 1
    visibility = clamp(visibility, 0.0, 1.0);

    stripeBoolFrag = stripeBool;

}
