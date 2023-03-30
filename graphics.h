#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }


    //observation mode variables
    bool switchMode;
    glm::mat4 planetView[15];

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;


    //skysphere
    Sphere* m_skysphere;

    Sphere* m_sun;
    Sphere* m_mercury;
    Sphere* m_venus;
    Sphere* m_earth;
    Sphere* m_mars;
    Sphere* m_jupiter;
    Sphere* m_saturn;
    Mesh* m_satring;
    Sphere* m_uranus;
    Sphere* m_neptune;
    //moons
    Sphere* m_moons[20];

    Mesh* m_comet;
    Mesh* m_mesh2;
    Mesh* m_asteroid[20];
    Mesh* m_ast[20];
    Mesh* m_as[20];
    Mesh* m_a[20];

};

#endif /* GRAPHICS_H */
