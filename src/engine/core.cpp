#include "core.h"

namespace Javita
{
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;
    GLFWwindow *window = nullptr;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
static void init()

{
    std::cout << "\n\nInitializing Javitas!"
              << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    Javita::window = glfwCreateWindow(Javita::SCR_WIDTH, Javita::SCR_HEIGHT, "Javitas", NULL, NULL);
    if (Javita::window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(Javita::window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, Javita::SCR_WIDTH, Javita::SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(Javita::window, framebuffer_size_callback);
    glDisable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_DEPTH_TEST);

    std::cout << "Javitas initialized successfully!\n\n"
              << std::endl;
}

static void loop(GLFWwindow *window)
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Javita::finish()
{
    glfwTerminate();
}

void Javita::run()
{
    init();
    loop(window);
    finish();
}