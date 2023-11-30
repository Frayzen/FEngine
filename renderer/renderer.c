#include "renderer/renderer.h"
#include "mesh/mesh.h"
#include "renderer/shader.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define TITLE "ENGINE"
#define VERSION "beta 0.0"

#define UNUSED(X) ((void)X)


float vertices[] = {
    0.5f,  0.5f, 0.0f,  // top right
    0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  

#define NB_VERT sizeof(vertices) / sizeof(float)
#define NB_TRI sizeof(indices) / 3

float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    1.0f, 1.0f,   // top-center corner
    0.0f, 1.0f   // top-center corner
};

void frame_buffersize_callback(GLFWwindow* window, int width, int height)
{
    UNUSED(window);
    glViewport(0, 0, width, height);
}

void setWindowFPS (GLFWwindow* win)
{
    static int nbFrames;
    static double lastTime;
    // Measure speed
    double currentTime = glfwGetTime ();
    nbFrames++;

    if ( currentTime - lastTime >= 1.0 ){ // If last cout was more than 1 sec ago
        char title [256];
        title [255] = '\0';

        snprintf ( title, 255,
                  "%s %s - [FPS: %3.2f]",
                  TITLE, VERSION, (float)nbFrames );

        glfwSetWindowTitle (win, title);

        nbFrames = 0;
        lastTime += 1.0;
    }
}

unsigned int createTexture(const char *path)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (!data)
    {
        fprintf(stderr, "Something went wrong while creating the texture %s\n", path);
        return 0;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, texture);
    return texture;
}

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    UNUSED(source);
    UNUSED(type);
    UNUSED(id);
    UNUSED(length);
    UNUSED(userParam);
    switch (severity) {
        case GL_DEBUG_SEVERITY_MEDIUM:
            fprintf(stdout, "WARNING: %s \n", message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            fprintf(stdout, "ERROR: %s \n", message);
            exit(-1);
            break;
        default:
            // fprintf(stdout, "INFO: %s \n", message);
            return;
    }
}

unsigned int createBuffer(void)
{
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

GLFWwindow *createWindow(const char *name)
{
    GLFWwindow* window = glfwCreateWindow(800, 600, name, NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Error while creating the window\n");
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window); 
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Error while seting up glew\n");
        glfwTerminate();
        return NULL;

    }
    glViewport(0, 0, 800, 600);
    glClearColor(.5f, .5f, .5f, 1.f);
    glfwSetFramebufferSizeCallback(window, frame_buffersize_callback);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return window;
}

void defineVertexShader(renderer *rd, const char *path)
{
    if (rd->vertexShader)
    {
        glDetachShader(rd->shaderProgram, rd->vertexShader);
        glDeleteShader(rd->vertexShader);
    }
    rd->vertexShader = createShader(GL_VERTEX_SHADER, path);
    glAttachShader(rd->shaderProgram, rd->vertexShader);
}

void defineFragmentShader(renderer *rd, const char *path)
{
    if (rd->fragmentShader)
    {
        glDetachShader(rd->shaderProgram, rd->fragmentShader);
        glDeleteShader(rd->fragmentShader);
    }
    rd->fragmentShader = createShader(GL_FRAGMENT_SHADER, path);
    glAttachShader(rd->shaderProgram, rd->fragmentShader);
}

renderer *initRenderer(const char *windowsName)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    renderer *rd = calloc(1, sizeof(renderer));
    rd->window = createWindow(windowsName);
    if (!rd->window)
        return NULL;
    glDebugMessageCallback(debugCallback, NULL);
    rd->shaderProgram = glCreateProgram();
    return rd;
}

int startRendering(renderer *rd)
{
    glLinkProgram(rd->shaderProgram);
    glUseProgram(rd->shaderProgram);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while(!glfwWindowShouldClose(rd->window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        setWindowFPS(rd->window);
        //render
        glUseProgram(rd->shaderProgram);
        // glBindTexture(GL_TEXTURE_2D, texture);
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, NB_TRI * 3, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(rd->window);
        glfwPollEvents();    
    }
    return 0;
}

void destroyRenderer(renderer *rd)
{
    if (!rd)
        return;
    for (int i = 0; i < rd->mesh_nb; i++)
        destroy_mesh(rd->meshes + i);
    // glDeleteBuffers(1, &VertexBuffer);
    // glDeleteBuffers(1, &TextureBuffer);
    // glDeleteBuffers(1, &ElementBuffer);
    glDetachShader(rd->shaderProgram, rd->vertexShader);
    glDetachShader(rd->shaderProgram, rd->fragmentShader);
    glDeleteShader(rd->vertexShader);
    glDeleteShader(rd->fragmentShader);
    glDeleteProgram(rd->shaderProgram);
    glfwDestroyWindow(rd->window);
    glfwTerminate();
    free(rd);
}

