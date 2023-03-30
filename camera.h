#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"



class Camera
{
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void Update(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
    void Update2(glm::vec3 cameraFront);
    void UpdateProjection(glm::mat4 projectionUP);
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    //OBSERVATION MODE
    void planetView(glm::mat4 view);

private:
    glm::vec3 m_speed = glm::vec3(0., 0., 0.);
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 newmodel;
    glm::mat4 newv;
};

#endif /* CAMERA_H */