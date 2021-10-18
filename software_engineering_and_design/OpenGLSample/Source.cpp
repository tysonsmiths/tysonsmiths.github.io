#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // Image loading Utility functions
#include <unordered_map>
//#include <pair>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "camera.h" // Camera class

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Tyson Smith CS330 Final Project"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;


    enum Projection_Type {
        ORTHO,
        PERSPECTIVE,
    };
    Projection_Type projectionType = PERSPECTIVE;

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Texture
    enum class TextureKey { 
        carpet, 
        wood, 
        rug, 
        off_white, 
        dark, 
        macbook_silver, 
        keyboard, 
        mac_screen, 
        left_screen, 
        right_screen 
    };
    unordered_map<TextureKey, pair<const char*, GLenum>> textureSetupMap = {
        {TextureKey::carpet, {"images/carpet.jpg", GL_REPEAT}},
        {TextureKey::wood, {"images/wood.jpg", GL_REPEAT}},
        {TextureKey::rug, {"images/rug.png", GL_CLAMP_TO_BORDER}},
        {TextureKey::off_white, {"images/offWhite.jpg", GL_REPEAT}},
        {TextureKey::dark, {"images/dark.jpg", GL_REPEAT}},
        {TextureKey::macbook_silver, {"images/macbookSilver.jpg", GL_REPEAT}},
        {TextureKey::keyboard, {"images/keyboard.jpg", GL_REPEAT}},
        {TextureKey::mac_screen, {"images/macScreen.png", GL_REPEAT}},
        {TextureKey::left_screen, {"images/leftScreen.jpg", GL_REPEAT}},
        {TextureKey::right_screen, {"images/rightScreen.jpg", GL_REPEAT}}
    };
    unordered_map<TextureKey, GLuint> textureMap;
    // Shader program
    GLuint gProgramId;
    // camera
    Camera gCamera(glm::vec3(0.0f, 3.0f, 10.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // primary Light color, position, and scale
    glm::vec3 gPrimaryLightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 gPrimaryLightPosition(-5.0f, 3.0, 0.0f);
    float gPrimaryLightScale(1.5f);

    glm::vec3 gSecondaryLightColor(0.96f, 0.74f, 0.12f);
    glm::vec3 gSecondaryLightPosition(5.0f, 10.0f, 5.0f);
    float gSecondaryLightScale(0.5f);
}
#include "renderUtils.h" // helpful util functions used for drawing

#include "Shape.h"
#include "Sphere.h" // Sphere
#include "Cylinder.h" // Cylinder
#include "Cuboid.h" // Cuboid
#include "Plane.h" // Plane

#include "renderObjects.h" // File that draws the objects in the scene.

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char*[], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessInput(GLFWwindow* window);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
bool UCreateTexture(const char* filename, TextureKey texture_key, GLenum textWrapMode);
void UDestroyTexture(GLuint textureId);
void URender();
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId);
void UDestroyShaderProgram(GLuint programId);

/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal; // VAP position 1 for normals
    layout(location = 2) in vec2 textureCoordinate;

    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
    out vec2 vertexTextureCoordinate;


    //Global variables for the transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

        vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

        vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
        vertexTextureCoordinate = textureCoordinate;
    }
);


/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,
    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 primaryLightColor;
    uniform vec3 primaryLightPos;
    uniform float primaryLightScale;
    uniform vec3 secondaryLightColor;
    uniform vec3 secondaryLightPos;
    uniform float secondaryLightScale;
    float specularIntensity;
    uniform sampler2D uTextureBase;
    uniform sampler2D uTextureExtra;
    uniform float uvExtra;
    uniform vec2 uvScaleBase;
    uniform vec2 uvScaleExtra;
    uniform bool multipleTextures;
    uniform vec3 viewPosition;

    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/
    vec3 getPhong(vec3 lightColor, vec3 lightPos, float lightScale, float specularIntensity) {

        //Calculate Ambient lighting*/
        float ambientStrength = 0.3f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor * lightScale; // Generate ambient light color

        //Calculate Diffuse lighting*/
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        //Calculate Specular lighting*/
        float highlightSize = 16.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        //Calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        //// Texture holds the color to be used for all three components
        //vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);
        vec4 textureColor = texture(uTextureBase, vertexTextureCoordinate * uvScaleBase);
        if (multipleTextures)
        {
            vec4 extraTextureColor = texture(uTextureExtra, vertexTextureCoordinate * uvScaleExtra + uvExtra);
            if (extraTextureColor.a != 0.0)
                textureColor = extraTextureColor;
        }

        // Calculate phong result
        vec3 phong = (ambient + diffuse + specular) * textureColor.xyz * lightScale;
        return phong;
    }


    void main()
    {
        vec3 primaryPhong = getPhong(primaryLightColor, primaryLightPos, primaryLightScale, specularIntensity);
        vec3 secondaryPhong = getPhong(secondaryLightColor, secondaryLightPos, secondaryLightScale, specularIntensity);
        fragmentColor = vec4(primaryPhong + secondaryPhong, 1.0); // Send lighting results to GPU
    }
);

int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;


    // Release textures
    bool failure = false;
    for (auto i : textureSetupMap) {
                                      // filename, textureKey, textureWrapMode
        if (!UCreateTexture(i.second.first, i.first, i.second.second)) {
            cout << "Failed to load texture " << i.second.first << endl;
            failure = true;
        }
    }
    if (failure) {
        return EXIT_FAILURE;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(gProgramId);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureBase"), 0);
    // We set the texture as texture unit 1
    glUniform1i(glGetUniformLocation(gProgramId, "uTextureExtra"), 1);

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;
        // input
        // -----
        UProcessInput(gWindow);

        // Render this frame
        URender();

        glfwPollEvents();
    }


    // Release textures
    for (auto i : textureMap) {
        UDestroyTexture(i.second);
    }

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);

    // GLEW: initialize
    // ----------------
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}


bool pAlreadyPressed = false;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessInput(GLFWwindow* window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UP, gDeltaTime);
     
    bool pIsPressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
    if (pIsPressed && pAlreadyPressed == false) {
        pAlreadyPressed = true;
        switch (projectionType) {
        case ORTHO:
            projectionType = PERSPECTIVE;
            break;
        case PERSPECTIVE:
            projectionType = ORTHO;
            break;
        }
    } else {
        pAlreadyPressed = pIsPressed;
    }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// Functioned called to render a frame
void URender()
{   
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);
    
    // Clear the frame and z buffers
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();
       
    // switch the project type based on keyboard input
    glm::mat4 projection;
    switch (projectionType) {
        case PERSPECTIVE:
            projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
            break;
        case ORTHO:
            projection = glm::ortho(-5.0f, 5.0f, -2.0f, 8.0f, 0.1f, 100.0f);
            break;
    }

    // Primary Light
    GLint primaryLightColorLoc = glGetUniformLocation(gProgramId, "primaryLightColor");
    GLint primaryLightPositionLoc = glGetUniformLocation(gProgramId, "primaryLightPos");
    GLint primaryLightScaleLoc = glGetUniformLocation(gProgramId, "primaryLightScale");

    glUniform3f(primaryLightColorLoc, gPrimaryLightColor.r, gPrimaryLightColor.g, gPrimaryLightColor.b);
    glUniform3f(primaryLightPositionLoc, gPrimaryLightPosition.x, gPrimaryLightPosition.y, gPrimaryLightPosition.z);
    glUniform1f(primaryLightScaleLoc, gPrimaryLightScale);

    // Secondary Light
    GLint secondaryLightColorLoc = glGetUniformLocation(gProgramId, "secondaryLightColor");
    GLint secondaryLightPositionLoc = glGetUniformLocation(gProgramId, "secondaryLightPos");
    GLint secondaryLightScaleLoc = glGetUniformLocation(gProgramId, "secondaryLightScale");

    glUniform3f(secondaryLightColorLoc, gSecondaryLightColor.r, gSecondaryLightColor.g, gSecondaryLightColor.b);
    glUniform3f(secondaryLightPositionLoc, gSecondaryLightPosition.x, gSecondaryLightPosition.y, gSecondaryLightPosition.z);
    glUniform1f(secondaryLightScaleLoc, gSecondaryLightScale);

    // Update the position
    GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPosition");
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


    // Set the shader to be used
    glUseProgram(gProgramId);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    GLint projLoc = glGetUniformLocation(gProgramId, "projection");

    resetModel();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw all the objects in the scene
    renderObjects();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
void flipImageVertically(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}

/*Generate and load the texture*/
bool UCreateTexture(const char* filename, TextureKey texture_key, GLenum textWrapMode)
{
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    GLuint textureID;

    if (image)
    {
        flipImageVertically(image, width, height, channels);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textWrapMode);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture


        textureMap[texture_key] = textureID;

        return true;
    }

    // Error loading the image
    return false;
}


void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}




// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint &programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

