#version 330
layout (location = 0) in vec3 vertexPosition;

uniform mat4 lightMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightMatrix * model * vec4(vertexPosition, 1.0);
}
