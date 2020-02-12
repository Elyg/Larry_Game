#version 420 core

// Ground Vertex Shader by Gareth FitzGerald

/// @brief the vertex passed in
layout (location=0) in  vec3  inVert;
/// @brief the in uv
layout (location=1) in vec2 inUV;
/// @brief the normal passed in
layout (location=2) in  vec3  inNormal;

// model view projection calculated in NGLDraw
uniform mat4 MVP;
// normal matrix calculated in NGLDraw
uniform mat3 normalMatrix;
// the animation time
uniform int time;

// the wavenumber frequency
uniform float K;
// the velocity of the wave sine wave animation
uniform float Velocity;
// the amplitude of the wave
uniform float Amp;
// the vertex position
out vec4 Position;
// the vertex normal
out vec3 Normal;
// the visibility of the vertex from the camera
out float visibility;


// density of the fog
const float density = 0.005; //0.01 works
// gradient fall-off of the fog
const float gradient = 3.0;

void main()
{
    // --------------------------- Vertex Displacement Animation -------------------------------//
    // Modified from :-
    // Wolff, David. Opengl 4 Shading Language Cookbook. Birmingham: Packt Publishing, 2013. Print.
    //
    vec4 pos = vec4(inVert, 1.0);
    //translate the y coord
    float u = K * (pos.x - Velocity * time);
    pos.y = Amp * sin(u);

    //compute the normal vector
    vec3 n = vec3(0.0);
    n.xy = normalize(vec2(-K * Amp * cos(u), 1.0));

    /// end of Citation

    // send position and normal (in camera coords) to fragment shader
    Position = MVP * pos;
    Normal = normalMatrix * n;

    // calculate the vertex position
    gl_Position = MVP * pos;

    // ------------------------------- Fog ---------------------------------------- //
    vec4 positionRelativeToCamera = gl_Position;
    float distance = length(positionRelativeToCamera.xyz);
    // calc exponential graph
    visibility = exp(-pow((distance * density),gradient));
    // clamp between 0 and 1
    visibility = clamp(visibility, 0.0, 1.0);

}
