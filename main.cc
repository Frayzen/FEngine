#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Constants
#define WIDTH 800
#define HEIGHT 800

// Macros
#define FAIL_ON(Condition, Message)                                            \
    if (Condition) {                                                           \
        std::cerr << Message << std::endl;                                     \
        clean_exit(-1);                                                        \
    }

void clean_exit(int code) {
    glfwTerminate();
    if (glfwGetCurrentContext() != nullptr)
        glfwDestroyWindow(glfwGetCurrentContext());
    exit(code);
}

int main() {
    glfwInit();
    // Define required version of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Specify that the project uses modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creation of the window
    GLFWwindow *win = glfwCreateWindow(WIDTH, HEIGHT, "FEngine", nullptr, nullptr);
    FAIL_ON(win == nullptr, "An error occured while creating the window");
    glfwMakeContextCurrent(win);

    // Background color
    gladLoadGL();
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(win);

    //Main loop
    while(!glfwWindowShouldClose(win))
    {
        //Take care of events
        glfwPollEvents();
    }
    clean_exit(0);
}
