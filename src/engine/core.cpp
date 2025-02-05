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
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(cameraRight) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(cameraRight) * cameraSpeed;
}

Javita::Rendeable rendeables[100];
int rendeablesCount = 0;
std::vector<std::function<void(float)>> updateCallbacks;

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

    for (int i = 0; i < rendeablesCount; i++)
    {
        rendeables[i].init();
    }
}

static void loop(GLFWwindow *window)
{
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        for (auto &callback : updateCallbacks)
        {
            callback(deltaTime);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT,
                                                0.1f, 100.0f);

        for (int i = 0; i < rendeablesCount; i++)
        {
            glUseProgram(rendeables[i].shader.shaderProgram);

            unsigned int viewLoc = glGetUniformLocation(rendeables[i].shader.shaderProgram, "view");
            unsigned int projLoc = glGetUniformLocation(rendeables[i].shader.shaderProgram, "projection");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            unsigned int modelLoc = glGetUniformLocation(rendeables[i].shader.shaderProgram, "model");
            glm::mat4 model = rendeables[i].transform.getModelMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            rendeables[i].draw();
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

Javita::Rendeable *Javita::Render::createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices)
{
    Rendeable newRendeable;
    newRendeable.vertices = std::move(vertices);
    newRendeable.indices = std::move(indices);
    newRendeable.transform = Javita::Transform();

    rendeables[rendeablesCount] = newRendeable;

    Rendeable *last = &rendeables[rendeablesCount];

    rendeablesCount++;

    return last;
}

Javita::Rendeable *Javita::Render::createRendeableObject(std::vector<float> vertices, std::vector<unsigned int> indices, glm::vec3 color)
{
    Rendeable newRendeable;
    newRendeable.vertices = std::move(vertices);
    newRendeable.indices = std::move(indices);
    newRendeable.color = color;

    rendeables[rendeablesCount] = newRendeable;

    Rendeable *last = &rendeables[rendeablesCount];

    rendeablesCount++;

    return last;
}

Javita::Rendeable *Javita::Render::Primitives::createTriangle()
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

Javita::Rendeable *Javita::Render::Primitives::createQuad()
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

void Javita::addOnUpdateCallback(std::function<void(float)> callback)
{
    updateCallbacks.push_back(callback);
}