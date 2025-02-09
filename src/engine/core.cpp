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
std::vector<std::function<void()>> guiCallbacks;

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

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Initialize ImGui backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

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

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (auto &callback : guiCallbacks)
        {
            callback();
        }

        // Render ImGui
        ImGui::Render();

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

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Javita::finish()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

Javita::Rendeable *Javita::Render::Primitives::createCube()
{
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f, 0.5f, // 0
        0.5f, -0.5f, 0.5f,  // 1
        0.5f, 0.5f, 0.5f,   // 2
        -0.5f, 0.5f, 0.5f,  // 3

        // Back face
        -0.5f, -0.5f, -0.5f, // 4
        0.5f, -0.5f, -0.5f,  // 5
        0.5f, 0.5f, -0.5f,   // 6
        -0.5f, 0.5f, -0.5f   // 7
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,

        // Right face
        1, 5, 6, 6, 2, 1,

        // Back face
        7, 6, 5, 5, 4, 7,

        // Left face
        4, 0, 3, 3, 7, 4,

        // Bottom face
        4, 5, 1, 1, 0, 4,

        // Top face
        3, 2, 6, 6, 7, 3};

    return createRendeableObject(vertices, indices);
}

Javita::Rendeable *Javita::Render::Primitives::createLine(std::vector<glm::vec3> points)
{
    std::vector<float> vertices;

    for (auto &point : points)
    {
        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);
    }

    std::vector<unsigned int> indices;

    for (int i = 0; i < points.size(); i++)
    {
        indices.push_back(i);
    }

    Rendeable *rendeable = createRendeableObject(vertices, indices);
    rendeable->renderMode = RenderMode::LINES;

    return rendeable;
}

void Javita::addOnUpdateCallback(std::function<void(float)> callback)
{
    updateCallbacks.push_back(callback);
}

void Javita::addGuiCallback(std::function<void()> callback)
{
    guiCallbacks.push_back(callback);
}

void Javita::Rendeable::init()
{
    {
        shader = createShader("defaultShader.vs", "defaultShader.fs");

        glUseProgram(shader.shaderProgram);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        std::vector<float> transformedVertices;
        transformedVertices.reserve(vertices.size() + (vertices.size() / 3 * 3)); // Reserve space for efficiency

        for (size_t i = 0; i < vertices.size(); i += 3)
        {
            // Copy position
            transformedVertices.push_back(vertices[i]);
            transformedVertices.push_back(vertices[i + 1]);
            transformedVertices.push_back(vertices[i + 2]);

            // Append color values
            transformedVertices.push_back(color.r);
            transformedVertices.push_back(color.g);
            transformedVertices.push_back(color.b);
        }

        float *verticesData = transformedVertices.data();

        glBufferData(GL_ARRAY_BUFFER, transformedVertices.size() * sizeof(float), verticesData, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        unsigned int *indicesData = indices.data();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indicesData, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
}
