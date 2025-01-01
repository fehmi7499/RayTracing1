#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>

#include <Camera.h>

// Light classes 
struct AmbientLight 
{
    glm::vec3 color;
    float intensity;
    AmbientLight(glm::vec3 col, float inten) : color(col), intensity(inten) {}
};

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 intensity;
    DirectionalLight(glm::vec3 dir, glm::vec3 inten) : direction(glm::normalize(dir)), intensity(inten) {}
};

struct Spotlight
{
    glm::vec3 position;
    glm::vec3 direction;
    float cutoffAngle;   // In degrees
    glm::vec3 intensity;
    Spotlight(glm::vec3 pos, glm::vec3 dir, float cutoff, glm::vec3 inten)
        : position(pos), direction(glm::normalize(dir)), cutoffAngle(cutoff), intensity(inten) {}
};

// Sphere and plan objects
struct Sphere
{
    glm::vec3 center;
    float radius;
    glm::vec3 color;
    float shininess;
    float reflection = 0.0f;  // Reflaction coefficient ( 0 = no reflection, 1 = fully reflection)
    float refraction = 0.0f;  // Refraction index (0 = no refraction)
    Sphere(glm::vec3 cen, float rad, glm::vec3 col, float shine)
        : center(cen), radius(rad), color(col), shininess(shine) {}
};

struct Plane
{
    glm::vec3 point;
    glm::vec3 normal;
    glm::vec3 color;
    float shininess;
    float reflection = 0.0f;  // Reflaction coefficient ( 0 = no reflection, 1 = fully reflection)
    float refraction = 0.0f;  // Refraction index (0 = no refraction)
    Plane(glm::vec3 pt, glm::vec3 norm, glm::vec3 col, float shine)
        : point(pt), normal(glm::normalize(norm)), color(col), shininess(shine) {}
};

// Scene class
struct Scene
{
    Camera* camera; 
    AmbientLight ambientLight;
    std::vector<DirectionalLight> directionalLights;
    std::vector<Spotlight> spotlights;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;

    Scene(Camera* cam, AmbientLight amb)
        : camera(cam), ambientLight(amb) {}
};



#endif