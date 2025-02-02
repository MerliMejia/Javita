#include "core.h"

namespace Javita
{
    int SCR_WIDTH = 800;
    int SCR_HEIGHT = 600;
    GLFWwindow *window = nullptr;
    unsigned int VAO;
    Shader shader;
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

    // Setting up what we're drawing

    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Javita::shader = createShader("defaultShader.vs", "defaultShader.fs");
    glUseProgram(Javita::shader.shaderProgram);

    // Generate and bind the VAO
    glGenVertexArrays(1, &Javita::VAO);
    glBindVertexArray(Javita::VAO);

    // Generate and bind the VBO, then fill it with the vertices
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Generate and bind the EBO, then fill it with the indices
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}

static void loop(GLFWwindow *window)
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(Javita::shader.shaderProgram);
        glBindVertexArray(Javita::VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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