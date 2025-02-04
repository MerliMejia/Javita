#include "core.h"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
GLFWwindow *window = nullptr;

// Camera stuff.

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window)
{
    // Calculate deltaTime for smooth movement
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Camera movement speed (units per second)
    float cameraSpeed = 2.5f * deltaTime;

    // Move forward (W) and backward (S)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    // Move left (A) and right (D)
    // Compute the right vector via cross product
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(cameraRight) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(cameraRight) * cameraSpeed;
}

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
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create view matrix (camera transformation)
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // Create projection matrix (perspective projection)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                0.1f, 100.0f);

        for (auto &rendeable : rendeables)
        {
            // Pass matrices to the shader
            unsigned int viewLoc = glGetUniformLocation(rendeable.shader.shaderProgram, "view");
            unsigned int projLoc = glGetUniformLocation(rendeable.shader.shaderProgram, "projection");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // (Optional) Create a model matrix if you need one, e.g. identity matrix:
            glm::mat4 model = glm::mat4(1.0f);
            unsigned int modelLoc = glGetUniformLocation(rendeable.shader.shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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

Javita::RendeableObject Javita::Render::Primitives::createTriangle()
{
    std::vector<float> vertices = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2 // triangle
    };

    return createRendeableObject(vertices, indices);
}

Javita::RendeableObject Javita::Render::Primitives::createQuad()
{
    std::vector<float> vertices = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    return createRendeableObject(vertices, indices);
}
