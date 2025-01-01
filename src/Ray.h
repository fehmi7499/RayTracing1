#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 origin;   // Starting point of the view
    glm::vec3 direction;  // Direction the ray is traveling (normalized)

    Ray(const glm::vec3& orig, const glm::vec3& dir)
        : origin(orig), direction(glm::normalize(dir)) {}
};


#endif