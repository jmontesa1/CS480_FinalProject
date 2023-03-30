#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"


static void cursorPositionCallBack(GLFWwindow*, double, double);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void observationMode();

class Engine
{
public:
    Engine(const char* name, int width, int height);

    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    
    void Display(GLFWwindow*, double);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    int h = 4;
    int w = 3;
private:
    // Window related variables
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
   

    

    bool m_running;

    Camera* m_camera;
};

#endif // ENGINE_H

