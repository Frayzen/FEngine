#include "renderer/renderer.h"
#include "mesh/mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define TITLE "ENGINE"
#define VERSION "beta 0.0"

#define UNUSED(X) ((void)X)

const char *vertexShaderSource = "\n"
    "#version 330 core\n"
    "layout (location = 1) in vec2 texCoord;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec2 exTexCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "exTexCoord=texCoord;\n"
    "}\n";

const char *fragmentShaderSource = "\n"
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 exTexCoord;\n"
    "uniform sampler2D text;\n"
    "\n"
    "void main()\n"
    "{\n"
    " FragColor=texture(text, exTexCoord);\n"
    "}\n";


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
            fprintf(
                stderr,
                "WARNING: %s \n",
                message
            );
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            fprintf(
                stderr,
                "ERROR: %s \n",
                message
            );
            exit(-1);
            break;
        default:
            // fprintf(
            //     stdout,
            //     "INFO: %s \n",
            //     message
            // );
            return;
    }
}

unsigned int loadShader(GLenum shaderType, const char *source)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(shaderType);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    return vertexShader;
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

struct Renderer*init_renderer(const char *windowsName)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    struct Renderer *rd = calloc(1, sizeof(struct Renderer));

    GLFWwindow *window = createWindow(windowsName);
    if (!window)
        return NULL;
    //create buffers
    glDebugMessageCallback(debugCallback, NULL);

    unsigned int VertexBuffer = createBuffer();
    unsigned int TextureBuffer = createBuffer();
    unsigned int ElementBuffer = createBuffer();

    //create shaders
    unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int shaderProgram = glCreateProgram();
    // unsigned int texture = createTexture("./assets/block.png");

    //compile and attach
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //create VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //begin VAO
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, TextureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //end VAO

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "text"), 0);
    return rd;
}

void destroyRenderer(struct Renderer *r)
{
    if (!r)
        return;
    for (int i = 0; i < r->mesh_nb; i++)
        destroy_mesh(r->meshes + i);
    free(r);
}

