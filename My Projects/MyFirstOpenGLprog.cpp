#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "MyFirstShaderClass.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// //sets up source codes for shaders unnecesary now that shader class created (code moved into fragment shader and vertex shader files)
// const char *vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "layout (location = 1) in vec3 aColor;"
//     "out vec3 ourColor;\n"                           //start back up at uniforms
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(aPos, 1.0);\n"
//     "   ourColor = aColor;\n"
//     "}\0";
// const char *fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "in vec3 ourColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(ourColor, 1.0);\n"
//     "}\n\0";

//sets mix value for how much we see of each image must be set outside of main so proccessInput function has access
//same for other variables here
    float mixValue = 0.2f;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    //vector for direction camera is facing
    glm::vec3 direction;
    //for turning left and right (value given is direction you start facing)
    float yaw = -90.0f;
    //for turning up and down (value given is direction you start facing)
    float pitch = 0.0f;
    float lastX = 400.0f, lastY = 300.0f;     //middle of window, current window size is 800x600 if changed this needs to change too
    bool firstMouse = true;
    float fov = 25.0f;

int main(void)
{
    //important opengl fact, bottom left is 0,0

    //set triangle vertices
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //      0.5f, -0.5f, 0.0f,
    //      0.0f, 0.5f, 0.0f
    // };

    //set vertices
    // float vertices[] = {
    //     //first 3 positions,    mid 3 colors            last 2 texture coordinates (if greater than 1 image wrapping occurs, if all 1s changed to 2s get 4 of each image, if less than 1 zooms in on image)
    //     0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 1.0f, //top right
    //     0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,//bottom right
    //     -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f,       0.0f, 0.0f,//bottom left
    //     -0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f,       0.0f, 1.0f//top left
    // };

    float vertices[] = {
        //new for da cube, first 3 positions last 2 texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    //this was used for element buffer object
    // unsigned int indices[] = {
    //     0, 1, 3, //first triangle
    //     1, 2, 3 //second triangle
    // };
    //not needed IG
    // float texCoords[] = {
    //     1.0f, 1.0f, //top right
    //     1.0f, 0.0f, //bottom right
    //     0.0f, 0.0f, //bottom left
    //     0.0f, 1.0f, //top left
    // };
    //positions for ten cubes because whats better than one (gives world space data if I'm correct)
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //calls mouse callback function every time mouse moves
    glfwSetCursorPosCallback(window, mouse_callback);

    //calls callback function every time you scroll
    glfwSetScrollCallback(window, scroll_callback);

    //tells GLFW to capture mouse (keeps in screen while tabbed in)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        exit(1);


    //build and compile shader program using shader class--------------------------------------------------------
    //Shader ourShader("ShaderFolder/VertexShader.vs", "ShaderFolder/FragmentShader.fs");
    //initialize as this on instead this one for upside down------------------------------------------------------
    //Shader ourShader("ShaderFolder/UpsideDownVertexShader.vs", "ShaderFolder/FragmentShader.fs");
    //initialize with these to display with offset---------------------------------------------------------------
    //Shader ourShader("ShaderFolder/OffsetVertexShader.vs", "ShaderFolder/FragmentShader.fs");
    //float offset = 0.5f;
    //initialize with this to set color by vertex position-------------------------------------------------------
    //Shader ourShader("ShaderFolder/VertexPosToColor.vs", "ShaderFolder/VertexPosToColor.fs");
    //initialize shader class with this vertex and fragment shader to texture the rectangle
    Shader ourShader("ShaderFolder/textureShader.vs", "ShaderFolder/textureShader.fs");

    
    glEnable(GL_DEPTH_TEST);

    //large chunk of code commented out as shader class handles all of it
    //creates shader object sotred as unsigned int and pass GL_VERTEX_SHADER to the create shader to create a vertex shader unnecessary now that shader class created
    // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //attach shader source code to object and compile it unnecessary now that shader class created
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);

    //checks if compiled correctly
    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    // }

    //sets up fragment shader, similar to setting up vertex shader unnecessary now that shader class created
    // unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);

    //same check for proper setup but for fragment shader
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    // }
    
    //link combiled shaders in shader program to be used for render calls
    // unsigned int shaderProgram = glCreateProgram();

    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);

    // //checks program was set up properly, very similar to shader check
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    // }

    // //program can now be activated using glUseProgram(shaderProgram);

    // //shader objects are unneeded once linked to program object
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    //set up triangle rendering (vertex buffer object)
    unsigned int VBO;
    //set up vertex array object
    unsigned int VAO;
    //set up element buffer object
    unsigned int EBO;


    //element buffer object not in use so commented out
    //generates buffer ID
    glGenBuffers(1, &VBO);
    //generates array ID
    glGenVertexArrays(1, &VAO);
    //generates buffer ID for element buffer
    //glGenBuffers(1, &EBO);

    //bind vertex array object
    glBindVertexArray(VAO);


    //triangle
    //binds new buffer to gl_array_buffer target which makes any calls on it effect VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //copy defined vertex data from GL_ARRAY_BUFFER into buffer memory                      for more info on this see hello triangle on opengl
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //rectangle
    //binds new buffer to gl_element_array_buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //copies defined vertex data intobuffer memory
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //sets a bunch of things for drawing vertices go reread opengl hello triangle to understand (position attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //sets color attribute not needed for da cube
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //sets texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //see example code
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //see example code again
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //load and create texture
    unsigned int texture;
    //takes as input how many textures are being stored then stores them in given variable (array if more than 1)
    glGenTextures(1, &texture);
    //has to be bound like any other object to be configured
    glBindTexture(GL_TEXTURE_2D, texture);
    //use to set if texture coordinates go over texture size (texture wrapping), s, t, and r are used in place of x, y, and z respectively
    //GL_REPEAT just repeats image, GL_MIRRORED_REPEAT repeats image mirrored, GL_CLAMP_TO_EDGE extends edge colors all the way to edge
    //GL_CLAMP_TO_BORDER doesn't extend image and fills in extra space with specified color
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //if GL_CLAMP_TO_BORDER used how to set color:
    //float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f }; //sets color of border
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //Texture filtering, used for both magnifying and minifying operations if texture is too small or too big for object (scaling the texture)
    //nearest filtering just takes texel coordinate points to, linear filtering takes surrounding texels color values and combines for color most similar to all
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //2nd parameter specifies for minifying, 3rd specifies using linear interpolation between two closest mipmaps and samples the interpolated level via linear interpolation
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //2nd paramter specifies for magnifying, 3rd  specifies for using linear filtering
    //load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //stbi_load takes location of image file then fills width height and number of color channels
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    //generate mipmap
    if(data)
    {
        //first argument is texture target, second argument specifies mipmap level, 0 is base, 3rd is format, image is rgb so will be stored as rgb
        //4th and 5th are width and height,  6th should pretty much just always be 0, 7th and 8th format and data type of source image(loaded as rgb stored as chars(bytes))
        //9th is just image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }
    //good practice to free memory
    stbi_image_free(data);

    //load and create second texture same process as first cept for second texture is png so rgba is needed in part
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true); //a way to flip the face when loading
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture" << endl;
    }

    //tell opengl which texture unit each shader sampler belongs to vby setting each sampler using glUniform1i
    ourShader.use(); //must activate shader before setting uniforms
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); //set manually
    ourShader.setInt("texture2", 1); //or set with shader class

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        /* Get escape input*/
        processInput(window);

        /* Render here */
        //clear color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 0.0f);
        //handled in new glClear
        //glClear(GL_COLOR_BUFFER_BIT);
        //clear depth buffer
        glClear(GL_DEPTH_BUFFER_BIT |GL_COLOR_BUFFER_BIT);

        /*renders stuff I think*/
        //glUseProgram(shaderProgram);      shader class takes care of use program
        ourShader.use();

        //new fancy 3d stuff
        //rotates on x-axis so it looks like it's on the floor
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //defines view matrix
        //this sets up where the camera is
        //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        //old camera code
        // glm::mat4 view = glm::mat4(1.0f);
        // view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));
        //sets where camera points (toward origin)
        // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        //set positive right vector that reps positive x axis in camera space, specify an up vector then get the cross product of that and the direction vector
        //new vec for moving camera
        // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        //which gives x-axis (direction is z axis)
        //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        //set up up positive y axis vector of camera, just cross of z and x
        //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        //set view using lookAt, see learn opengl page on how lookAt works
        glm::mat4 view;
        //giving lookAt a position, target, and up vector in that order
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           up);
        //make camera spin for fun
        // const float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        //define projection matrix, first parameter fov, 2nd aspect ratio, 3rd and 4th near and far plane of frustum
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (800.0f / 600.0f), 0.1f, 100.0f);
        //new 3d stuff, retrieve matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        //3 different ways to pass matrix unfiorms to shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        //this sets projection matrix each frame however since it rarely changes it's good practice to set it outside of the main loop so it just runs once
        ourShader.setMat4("projection", projection);
        
        //do transformation most of this is gray for now just to figure out 3d stuff
        //this just makes a vector then does matrix math on it for an example
        // glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        // glm::mat4 trans = glm::mat4(1.0f);
        // trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
        // vec = trans * vec;
        // cout << vec.x << vec.y << vec.z << endl;
        //transformation on the square
        // glm::mat4 trans = glm::mat4(1.0f);
        //if rotate first before translate then spins in a circle
        // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        
        //more transfrom stuff
        //this gets the uniform
        // unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        //this gives uniform the matrix data, 1st argument is uniform location, second how many matrices we are sending
        //third asks if we want to transpose our matrix which is no, last param is matrix data but glm stores different from opengl value_ptr used to make compatible
        // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        //use with other offset display set up for shader
        //ourShader.setFloat("xOffset", offset);
        //bind texture to be rendered
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        //set mix value in shader
        ourShader.setFloat("mixValue", mixValue);
        //bind vertex array to be rendered
        glBindVertexArray(VAO);

        //update shader uniform
        // double timeValue = glfwGetTime();
        // float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);      //sets color value based on time and changes continuously
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        //this drew triangles
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //was used for square
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);      
        //draw da cube
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        //draw da cubes
        for(unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i * (float)glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // second transformation also gray, was used for 2d now being used for 3d
        // ---------------------
        // trans = glm::mat4(1.0f); // reset it to identity matrix
        // trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        // float scaleAmount = static_cast<float>(sin(glfwGetTime()));
        // trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]); // this time take the matrix value array's first element as its memory pointer value (don't need 3rd cause it's z value and not messing with that layer I think)
        // now with the uniform matrix being replaced with new transformations, draw it again.
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram); unnecessary as is handled by shader class

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.0001f;
        if (mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.0001f;
        if (mixValue <= 0.0f)
        {
            mixValue = 0.0f;
        }
    }
    //camera controls
    //const float cameraSpeed = 0.05f; // no need for adjustment because delta time is awesome
    float cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed;
    }
}
// code that allows turning with mouse
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){

    //turns doubles into floats
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    //makes sure camera doesn't jump when program starts
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    //gets change in mouse position for turning camera, y is reversed because y coordinates increase going down
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    //updates last values to new values for next movement calculations
    lastX = xpos;
    lastY = ypos;

    //controls how sensitive turning is, big surprise
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    //sets variables that change the direction you're facing
    yaw += xoffset;
    pitch += yoffset;

    //clamps pitch preventing look at flip and weird inversion stuff
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    //sets direction vector with new ptch and yaw using this funny formula
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

//adjusts fov by scrolling
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

    //changes fov based on yoffset (how far you scroll ig)
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}