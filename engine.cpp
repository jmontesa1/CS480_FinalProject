
#include "engine.h"


Graphics* m_graphics;
Window* m_window;
Engine* m_engine;
glm::vec3 cameraPos = glm::vec3(.0, 1.3, 14.0);
glm::vec3 cameraFront = glm::vec3(.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direc;

Engine::Engine(const char* name, int width, int height)
{
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize()
{

    // Start a window
    m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
    if (!m_window->Initialize())
    {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics();
    if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
    {
        printf("The graphics failed to initialize.\n");
        return false;
    }

    
    glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);
   

    return true;
}

void Engine::Run()
{
    m_running = true;

    while (!glfwWindowShouldClose(m_window->getWindow()))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        ProcessInput();
        Display(m_window->getWindow(), glfwGetTime());
        glfwPollEvents();

        m_graphics->getCamera()->Update(cameraPos, cameraFront, cameraUp);

    }
    m_running = false;

}



void Engine::ProcessInput()
{
    

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    double xpos, ypos;
    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(m_window->getWindow(), &xpos, &ypos);

    bool fMouse = true;
    float lX = 400;
    float lY = 300;
    float fov = 45.0f;
    float offsetX = xpos - lX;
    float offsetY = lY - ypos;
    const float sensitivity = 0.1f;
    float yaw = -90.0f;
    float pitch = 0.0f;

    lX = xpos;
    lY = ypos;

    offsetX *= sensitivity;
    offsetY *= sensitivity;
    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        front.y = sin(glm::radians(pitch)),
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);


    float cameraSpeed = 10.f * deltaTime;
    //OBSERVATION MODE
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_M) == GLFW_PRESS)
    {
        observationMode();
    }
    else {
        m_graphics->switchMode = false;
    }




    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //speed up
        cameraSpeed *= 3.0;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //slow down
        cameraSpeed *= 0.4;

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    glfwSetScrollCallback(m_window->getWindow(), scroll_callback);

}

unsigned int Engine::getDT()
{
    //long long TimeNowMillis = GetCurrentTimeMillis();
    //assert(TimeNowMillis >= m_currentTimeMillis);
    //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
    //m_currentTimeMillis = TimeNowMillis;
    //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
    //timeval t;
    //gettimeofday(&t, NULL);
    //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    //return ret;
    return (long long)glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();

    // update graphics -> updates model
    //m_graphics->Update(time);
    m_graphics->HierarchicalUpdate2(time);
}


float fov = 45.0f;

static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 100.0f)
        fov = 100.0f;
    m_graphics->getCamera()->UpdateProjection(glm::perspective(glm::radians(fov), 800.0f / 600.0f, 1.0f, 10000000.0f));

}

void observationMode() {
    m_graphics->switchMode = true;

    m_graphics->getCamera()->planetView(m_graphics->planetView[0]);

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_1) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[9]);

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_2) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[2]);

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_3) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[3]);
    
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_4) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[4]);
    
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_5) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[5]);
    
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_6) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[6]);
    
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_7) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[7]);
    
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_8) == GLFW_PRESS)
        m_graphics->getCamera()->planetView(m_graphics->planetView[8]);



}