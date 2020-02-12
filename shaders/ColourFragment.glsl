#version 330 core

in vec4 vertColour;
/// @brief our output fragment colour
layout (location =0)out vec4 fragColour;

void main ()
{
    fragColour.rgba=vertColour;
}

