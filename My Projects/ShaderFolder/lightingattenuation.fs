#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    float shininess;
    sampler2D specular;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; //new floats added to light struct, constant sets base starting value (1 alost always)
    float linear;   //how fast the linear decrease of the light intensity is
    float quadratic;       //how fast the quadratic decrease of light intensity is
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//same as lightingfragmentshader1 new parts are commented on

void main()
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance))); //quadratic function that decreases light intensity exponentially based on distance from source

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;
    diffuse *= attenuation; //changes intensity of diffuse based on distance from light

    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    ambient *= attenuation; //changes intensity of diffuse based on distance from light

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
    specular *= attenuation; //changes intensity of specular based on distance from light

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}