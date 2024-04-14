#include <iostream>
#include <GL/glew.h>
#include "mesh/mesh.hh"
#include "object/object.hh"
#include "shader.hh"
#include "tools.hh"
#include <GLFW/glfw3.h>
#include <string.h>

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *msg, const void *userParam) {
    (void)source;
    (void)type;
    (void)id;
    (void)severity;
    (void)length;
    (void)userParam;
    std::cout << msg << '\n';
    std::flush(std::cout);
}

int main() {
    glfwInit();
    // Define required version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Specify that the project uses modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Debug
    /* glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); */

    // Creation of the window
    GLFWwindow *win =
        glfwCreateWindow(WIDTH, HEIGHT, "FEngine", nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    glewInit();
    glViewport(0, 0, WIDTH, HEIGHT);

    /* glEnable(GL_DEBUG_OUTPUT); */
    /* glDebugMessageCallback(DebugCallback, 0); */

    Shader shader =
        Shader("assets/shaders/default.vert", "assets/shaders/default.frag");

    float vertices[] = {
        1.0f,  1.0f,  0.0f, // Vertex 1
        -1.0f, 1.0f,  0.0f, // Vertex 2
        0.0f,  -1.0f, 0.0f  // Vertex 3
    };
    GLuint VAO, VBO;
    // Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Vertex Array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
    glEnableVertexAttribArray(0);

    // Main loop
    while (!glfwWindowShouldClose(win)) {
        // Take care of events
        glfwPollEvents();
        // Background color
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.activate();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /* o.render(shader.getProgram()); */

        glfwSwapBuffers(win);
    }

    clean_exit(0);
}
