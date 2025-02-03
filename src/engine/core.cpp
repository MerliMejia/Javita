#include "core.h"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
GLFWwindow *window = nullptr;

struct Rendeable
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Shader shader;

    void init()
    {
        shader = createShader("defaultShader.vs", "defaultShader.fs");

        glUseProgram(shader.shaderProgram);

        // Generate and bind the VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind the VBO, then fill it with the vertices
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        float *verticesData = vertices.data();

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), verticesData, GL_DYNAMIC_DRAW);

        // Generate and bind the EBO, then fill it with the indices
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        unsigned int *indicesData = indices.data();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indicesData, GL_DYNAMIC_DRAW);

        // Set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Unbind the VAO
        glBindVertexArray(0);
    }

    void draw()
    {
        glUseProgram(shader.shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
};

std::vector<Rendeable> rendeables;

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

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Javitas", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glDisable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_DEPTH_TEST);

    std::cout << "Javitas initialized successfully!\n\n"
              << std::endl;

    for (auto &rendeable : rendeables)
    {
        rendeable.init();
    }
}

static void loop(GLFWwindow *window)
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto &rendeable : rendeables)
        {
            rendeable.draw();
        }

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

Javita::RendeableObject Javita::Render::createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices)
{
    Rendeable rendeable;
    rendeable.vertices = vertices;
    rendeable.indices = indices;

    Javita::RendeableObject rendeableObject(rendeable.vertices, rendeable.indices, rendeable.shader);

    rendeables.push_back(rendeable);

    return rendeableObject;
}
