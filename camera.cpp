#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  //view = glm::lookAt( glm::vec3(x, y, z), //Eye Position
  //                    glm::vec3(0.0, 0.0, 0.0), //Focus point
  //                    glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
  
    view = glm::lookAt(glm::vec3(2.0f, 1.8f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    projection = glm::perspective( glm::radians(40.f), //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 1.0f, //Distance to the near plane, normally a small value like this
                                 10000000.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::Update(glm::vec3 cameraPos2, glm::vec3 cameraFront2, glm::vec3 cameraUp2)
{
    view = glm::lookAt(cameraPos2, cameraFront2 + cameraPos2, cameraUp2);
}

void Camera::Update2(glm::vec3 newFront)
{
    cameraFront = newFront;
    view = newv;

}

void Camera::UpdateProjection(glm::mat4 projectionUP)
{
    projection = projectionUP;
}

void Camera::planetView(glm::mat4 newView) {
    //view = glm::lookAt(glm::vec3(-8, 0, -8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    view *= newView;
}