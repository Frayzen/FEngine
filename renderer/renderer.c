#include "maths/matrix.h"
#include "renderer/renderer.h"
#include "renderer/shader/shader.h"
#include "renderer/world/world.h"
#include "timer/timer.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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
        snprintf ( title, 255, "[FPS: %3.2f]", (float)nbFrames );
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

GLFWwindow *createWindow(void)
{
    GLFWwindow* window = glfwCreateWindow(800, 600, "", NULL, NULL);

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
    glViewport(0, 0, 1280, 720);
    glClearColor(.5f, .5f, .5f, 1.f);
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
    glLinkProgram(rd->shaderProgram);
    glUseProgram(rd->shaderProgram);
}

renderer *initRenderer(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    renderer *rd = calloc(1, sizeof(renderer));
    rd->window = createWindow();
    if (!rd->window)
        return NULL;
    rd->shaderProgram = glCreateProgram();
    return rd;
}


int startRendering(renderer *rd, world *w)
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    registerEvents();
    GLint transformLocation = glGetUniformLocation(rd->shaderProgram, "transform");
    GLint projectionLocation = glGetUniformLocation(rd->shaderProgram, "projection");
    GLint viewLocation = glGetUniformLocation(rd->shaderProgram, "view");
    float lastTime = 0;
    while(!glfwWindowShouldClose(rd->window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        setWindowFPS(rd->window);
        updateWorld(w);
        mat4 view = mat4Transform(CURRENT_CAMERA(w)->transform);
        mat4 projection = CURRENT_CAMERA(w)->projection;
        //render
        for (unsigned int i = 0; i < w->mesh_count; i++) {
            mesh *m = w->meshes[i];
            glUseProgram(rd->shaderProgram);
            // glBindTexture(GL_TEXTURE_2D, texture);
            mat4 transform = mat4Transform(m->transform);
            glUniformMatrix4fv(transformLocation, 1, GL_FALSE, (float *) &transform);
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (float *) &view);
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (float *) &projection);
            glBindVertexArray(m->graphic.VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->graphic.EBO);
            glDrawElements(GL_TRIANGLES, m->tris_count * 3, GL_UNSIGNED_INT, 0);
        }
        glfwSwapBuffers(rd->window);
        updateFrame(w, glfwGetTime() - lastTime);
        lastTime = glfwGetTime();
        glfwPollEvents();
    }
    return 0;
}

void destroyRenderer(renderer *rd)
{
    if (!rd)
        return;
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

