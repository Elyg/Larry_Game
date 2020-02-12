#version 420 core
#define PI 3.14159265
layout (location=0) out vec4 outColour;

/// modified from the OpenGL Shading Language Example "Orange Book"
/// Roost 2002

in vec3 surfaceNormal;
in vec4 ShadowCoord;
in vec3 eyeDirection;
in vec3 lightDir;
in vec3 halfVector;
in float visibility;
in vec2 vertexUV;
in vec4 Position;
in vec3 Normal;
flat in int stripeBoolFrag;

// for mac we need this
//uniform sampler2DShadow ShadowMap;
// for windows unix we need
layout(binding=0) uniform sampler2D ShadowMap;
layout(binding=1) uniform sampler2D NoiseMap;
//uniform sampler2D ShadowMap;
//uniform sampler2D NoiseMap;
//uniform sampler2D tex1;

/// @brief material structure
struct Materials
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  float shininess;
};

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


uniform Materials material;
uniform Lights light;
uniform vec3 skyColour;
uniform int checker;
// Disintegration thresholds
uniform float LowThreshold;
uniform float HighThreshold;

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

vec4 stripe (vec2 UV){
    vec4 stripes;
    float stripeSize = 16;
    vec3 color;
    vec2 position = UV * stripeSize;
    const vec4 white = vec4(1.0,1.0,0.9,1.0);
    vec4 green = material.diffuse; //vec4(0.0,0.5,0.5,1.0);

    float sawtooth = fract(0.5 * position.x);
    float triangle = (abs(2.0 * sawtooth - 1.0));

    // set the actual fragment colour.
    return stripes = mix(green, white, triangle);

}

// Lizzy start
vec4 checkers (vec2 outUV){
    vec4 checkers;
    float checkSize = 16;
    vec3 color;
    vec2 position = outUV * checkSize;
    float fmodResult = mod(floor(position.x) + floor(position.y),2.0);

    if (fmodResult < 1.0)
    {
        color = vec3(0, 0, 0);
    }
    else
    {
        color = vec3(1.0, 1.0, 1.0);
    }

    return checkers = vec4(color, 1.0);
}
//Lizzy stop

vec4 pointLight(vec4 colour)
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float ambientIntensity = 0.05;
    vec3 halfV;
    float nDotHV;
    vec3 N = normalize(surfaceNormal);
    vec3 E = normalize(eyeDirection);
    vec3 L = normalize(lightDir);
    float lambertTerm = clamp(dot(N,L), 0,1);
    halfV = normalize(halfVector);
    nDotHV = max(dot(N, halfV), 0.0);

    diffuse= colour * light.diffuse * lambertTerm;
    specular = material.specular * light.specular * pow(nDotHV, material.shininess);
    ambient = material.ambient * light.ambient * ambientIntensity;

    return ambient + diffuse + specular;
}


subroutine (RenderPassType)
void shadeWithShadow()
{
    if (stripeBoolFrag == 1)
    {
        vec4 stripes = stripe(vertexUV);
        outColour = pointLight(stripes);
        return;
    }


    else if (checker == 1)
    {
        vec4 check = checkers(vertexUV);
        vec4 outColourA = pointLight(check);
        outColour = mix(vec4(skyColour, 1.0), outColourA, visibility);
        return;
    }
    else
    {
    // ----------------- ambient component ---------------
    float ambientStr = 0.3;
    vec4 ambient = vec4(ambientStr * light.diffuse * material.ambient);///compute ambient *light.diffuse
    //vec4 diffSpec = diffAndSpec();

    //Do the shadow-map look up
    //We use the built-in texture access function textureProj to access the
    //shadow map texture shadowMap. it first will divide the first 3 components
    // of the text coords by the fourth component
    // (convert from homogeneous pos to a cartesian pos)
    // Also our sampler type is sampler2DShadow set at the top, so its treated as a
    // texture containing depth values
    // The first two components of ShadowCoord are used to access a depth value within the texture.
    // That value is compared against the value of the third component of ShadowCoord.
    // The GL_NEAREST in NGLScene is our interpolation mode, the result will be 1 or 0

    float shadeFactor = textureProj(ShadowMap, ShadowCoord).x;
    shadeFactor = shadeFactor; // + 0.75;
    float s = smoothstep(0.94, 1.0, shadeFactor);
    float t = clamp(s, 0.5, 1.0);

    vec4 noise = texture(NoiseMap, vertexUV);
    //outColour = noise;
    vec4 outColourA = vec4( t * material.diffuse.xyz, material.diffuse.a);// + ambient);

    vec4 noiseColour = mix(outColourA, noise, 0.05);

    vec4 objColour = pointLight(noiseColour);

    outColour = mix(vec4(skyColour, 1.0), objColour, visibility);
    }

}

subroutine (RenderPassType)
void recordDepth()
{
    // Do nothing, depth will be written automatrically
    // we only need to write the depth to the depth buffer
    // so in this pass there is nothing for the fragment shader to do
   // outColour = DepthColour;

}

subroutine (RenderPassType)
void shadeWithDisintegrate()
{

    float shadeFactor = textureProj(ShadowMap, ShadowCoord).x;
    shadeFactor = shadeFactor; // + 0.75;
    float s = smoothstep(0.94, 1.0, shadeFactor);
    float t = clamp(s, 0.5, 1.0);
    vec4 noise = texture(NoiseMap, vertexUV);

    vec4 outColourA = vec4( material.diffuse.xyz, material.diffuse.a);// + ambient);

    if (stripeBoolFrag == 1)
    {
        vec4 stripes = stripe(vertexUV);
        outColourA = pointLight(stripes);
    }

    vec4 noiseColour = mix(outColourA, noise, 0.1);

    //Disintegration/Spread technique, if the value is outside the thershold, discard the fragment
    if( noise.x < LowThreshold || noise.x > HighThreshold)
    {
       noiseColour.r = HighThreshold;
       discard;
    }

    outColour = noiseColour;




}

void main ()
{
  //float shadeFactor = textureProj(ShadowMap,ShadowCoord).x;
   RenderPass();

}
