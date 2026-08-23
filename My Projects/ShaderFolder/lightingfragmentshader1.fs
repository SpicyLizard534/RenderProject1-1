#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    //both are normals because magnitude doesn't really matter just direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    //get diffuse(range 0-1) from dot product of face normal and direction of light, greater angle between the two = darker diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //ges how bright ambient light should be (darkest colors on model)
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //set specular strength (how big of effect it has), and also do calculations for specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}