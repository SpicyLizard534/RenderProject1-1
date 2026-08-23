#version 330 core
out vec4 FragColor;

in vec3 LightingColor; 

uniform vec3 objectColor;

void main()
{
   FragColor = vec4(LightingColor * objectColor, 1.0);
}

//for Gouraud most math is moved to vertex shader, I'm being lazy right now so I'm not paying attention to if anything is different