#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

//initializes texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
//uniform allows real time changing of mix value
uniform float mixValue;

void main()
{
    //texture function takes a sampler as first argument and coordinates as second
    //this one just does the box
    //FragColor = texture(exture1, TexCoord);
    //this one does the box in rainbow
    //FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);
    //combines the two textures being displayed
    //mix takes 2 values as input and linearly interpolates between them based on 3rd argument (0.0 all first), (1.0 all second), (0.2 80% first 20% second)
    //extra math on second texture flips direction smiley face is facing
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);
}