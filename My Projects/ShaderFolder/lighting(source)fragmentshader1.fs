#version 330 core
out vec4 FragColor;

struct Light{
    float r;
    float g;
    float b;
};

uniform Light light;

void main()
{
    FragColor = vec4(light.r, light.g, light.b, 1.0); // set all 4 vector values to 1.0
}