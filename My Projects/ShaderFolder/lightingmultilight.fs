#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    float shininess;
    sampler2D specular;
};

//define structs for each type of lighting
struct DirLight {   //directional lighting ex: the sun
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight { //point lights ex: lightbulbs
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {  //spotlights ex: flashlight
    vec3 direction;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear; 
    float quadratic; 

    float innerCutOff;
    float outerCutOff;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;  //uniform to send dir light struct info to fragment shader
uniform SpotLight spotLight;       //uniform to send a spotlight to the fragment shader
#define NR_POINT_LIGHTS 4   //not really sure how this works but when the value is given to set the length of the array it sets it to 4
uniform PointLight pointLights[NR_POINT_LIGHTS];    //creates array of length 4 to send 4 point lights to fragment shader

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//set function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//same as old lighting fragment shaders but now modified to combine them

void main()
{ 
    //float distance = length(light.position - FragPos);
    //float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));
    //initial set up
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    //calculate directional lighting using function and store in result variable
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //calculate lighting effect of each point light and increment result by its effect
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    //calculate lighting effect of spot light and increment result by its effect
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    //output result
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)  //function that calculates effect of directional light on object
{ //functions the same as calculating just the directional light originally, just returns the value to be added to the total of every light effecting the object
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  //function calculates effect of directional light on object
{   //functions same as calculating point lights originally, just returns value to be added to total effects
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if(theta > light.outerCutOff)
    {
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

        ambient *= (attenuation * intensity);
        diffuse *= (attenuation * intensity);
        specular *= (attenuation * intensity);
        return (ambient + diffuse + specular);
    }
    else
    {
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        ambient *= attenuation * intensity;
        return(vec3(ambient));
    }
}