#include <GL/glew.h>                        //same imports and functions as original with same operations
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "MyFirstShaderClass.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


//same variables set at beginning
float mixValue = 0.2f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
glm::vec3 direction;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f, lastY = 300.0f;     
bool firstMouse = true;
float fov = 25.0f;

//new variable for position of light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//its the main function

int main(void)
{
    //same initializing and configuring
    glfwInit();
    if (!glfwInit())
        return -1;

    GLFWwindow* window;

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        exit(1);

    glEnable(GL_DEPTH_TEST);

    //initialize shaders, lightsources have a different fragment shader so lightsources don't effect the appearance of light sources
    Shader lightingShader("ShaderFolder/lightingvertexshader1.vs", "ShaderFolder/lightingfragmentshader1.fs");
    Shader lightCubeShader("ShaderFolder/lighting(source)vertexshader1.vs", "ShaderFolder/lighting(source)fragmentshader1.fs");
    //lighting calculations done in vertex shader instead of fragment, called Gouraud method (replaces other above using Phong) (not very noticeable on cube but fun)
    //Shader lightingShader("ShaderFolder/lightingvertexshader1(Gouraud).vs", "ShaderFolder/lightingfragmentshader1(Gouraud).fs");

    //adding normals to vertices (could be calculated but we aren't for tutorial?)
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    unsigned int VBO;

    unsigned int cubeVAO;

    glGenVertexArrays(1, &cubeVAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    //position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightCubeVAO;

    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_DEPTH_BUFFER_BIT |GL_COLOR_BUFFER_BIT);

        //set shader and draw cube
        //this part sets light properties
        lightingShader.use();
        //these are no longer needed with structs
        // lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //color of light effects color of cube (no red light to reflect no color red to be seen, multiplies light color by color of object)
        // lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        //position of light given to get direction light is coming from for calculations
        // lightingShader.setVec3("lightPos", lightPos);
        //position of camera given to get direction of camera for calculations
        lightingShader.setVec3("viewPos", cameraPos);
        lightingShader.setVec3("light.position", lightPos);
        glm::vec3 lightColor;
        //rgb light
        lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
        lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
        lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
        //whit light
        // lightColor.x = static_cast<float>(1.0);
        // lightColor.y = static_cast<float>(1.0);
        // lightColor.z = static_cast<float>(1.0);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);      //decreases influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  //low influence again
        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


        //set cube materials
        lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 32.0f);

        glm::mat4 view;
        view = glm::lookAt(cameraPos,
                           cameraPos + cameraFront,
                           up);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (800.0f / 600.0f), 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //swap shaders and draw light cube
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        //set color of light
        lightCubeShader.setFloat("light.r", lightColor.x);
        lightCubeShader.setFloat("light.g", lightColor.y);
        lightCubeShader.setFloat("light.b", lightColor.z);
        //set light pos
        model = glm::mat4(1.0f);
        lightPos.x = sin((float)glfwGetTime()) * 2;
        lightPos.y = cos((float)glfwGetTime()) * 2;
        lightPos.z = cos((float)glfwGetTime()) * 2;
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //swaps front and back buffers and gets inputs from user
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate all resources now that they are done being used(prevents memory leaks and stuff I think, good habits)
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    //clears all other glfw allocated resources
    glfwTerminate();
    return 0;
};

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