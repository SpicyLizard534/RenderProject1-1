#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    float shininess;
    sampler2D specular;
};

struct Light {
    vec3 direction; //needs direction of spotlight for calculations
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear; 
    float quadratic; 

    float innerCutOff; //how wide spotlight is
    float outerCutOff; //how wide spotlight edge fade is
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//same as lightingattenuation.fs new parts are commented on

void main()
{ 
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    vec3 lightDir = normalize(light.position - FragPos);

    float theta = dot(lightDir, normalize(-light.direction)); //dot product of direction to light and direction of flashlight(way you are looking) negated because we want sides facing you not away from you
    float epsilon = light.innerCutOff - light.outerCutOff;  //calcuate area on edge of light
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //calculation to make light less intense the further from the inner cut off it is

    if(theta > light.outerCutOff) //greater than because 0 degrees cosine is 1 so the further away the lower the number gets
    {

        vec3 norm = normalize(Normal);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;
        diffuse *= intensity;
        diffuse *= attenuation;

        vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
        ambient *= attenuation;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;
        specular *= intensity;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
        ambient *= attenuation;

        FragColor = vec4(ambient, 1.0);
    }

}