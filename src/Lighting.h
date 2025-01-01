#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>
#include <Scene.h>
#include <Ray.h>

// Function to calculate lighting for a specific hit point
glm::vec3 CalculateLighting(const Ray& ray, const Scene& scene, int objectType, int opjectIndex, const glm::vec3& hitPoint);

#endif