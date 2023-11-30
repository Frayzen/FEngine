#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define TITLE "ENGINE"
#define VERSION "beta 0.0"

#define UNUSED(X) ((void)X)

const char *vertexShaderSource = "\n"
    "#version 330 core\n"
    "layout (location = 1) in vec4 col;\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 exColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "exColor=col;\n"
    "}\n";

const char *fragmentShaderSource = "\n"
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 exColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    " FragColor=exColor;\n"
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

const float colors[] = {
    1,0,0,1,
    0,1,0,1,
    1,0,0,1,
    0,1,0,1,
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

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Engine", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Error while creating the window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Error while seting up glew\n");
        glfwTerminate();
        return -1;

    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, frame_buffersize_callback);
    glClearColor(.5f, .5f, .5f, 1.f);

    unsigned int VertexBuffer = createBuffer();
    unsigned int ColorBuffer = createBuffer();
    unsigned int ElementBuffer = createBuffer();

    printf("[OPENGL LOADED] %s\n", glGetString(GL_VERSION));
    glDebugMessageCallback(debugCallback, NULL);

    unsigned int vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    //begin VAO

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //end VAO
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        setWindowFPS(window);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, NB_TRI * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glDeleteBuffers(1, &VertexBuffer);
    glDeleteBuffers(1, &ColorBuffer);
    glDeleteBuffers(1, &ElementBuffer);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
