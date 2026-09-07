#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//don't need these with new structs
// uniform vec3 objectColor;
// uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    //both are normals because magnitude doesn't really matter just direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    //get diffuse(range 0-1) from dot product of face normal and direction of light, greater angle between the two = darker diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    //ges how bright ambient light should be (darkest colors on model)
    //unneeded now that using materials?
    //float ambientStrength = 0.1;
    vec3 ambient = material.ambient * light.ambient;

    //set specular strength (how big of effect it has), and also do calculations for specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}