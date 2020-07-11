#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;


out vec4 color;

uniform mat4 transform;
uniform mat4 rotation;

void main()
{
    gl_Position = transform * rotation * vec4(aPos, 1.0f);
    //    gl_Position = vec4(aPos, 1.0f);
    color = aColor;
}