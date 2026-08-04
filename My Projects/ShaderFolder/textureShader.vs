#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//this one is for passing color stuyff to fragment shader, not in use right now
out vec3 ourColor;

out vec2 TexCoord;

//not in use right now
//uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    //not in use for now was used with transform
    //gl_Position = transform * vec4(aPos, 1.0f);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}