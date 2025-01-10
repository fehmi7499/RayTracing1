#include "Ray.h"

Ray::Ray(const glm::vec3& direction, const glm::vec3& origin)
    : direction(direction), origin(origin), sceneObject(nullptr) {}

glm::vec3 Ray::getRayDirection() const { return direction; }
glm::vec3 Ray::getRayOrigin() const { return origin; }
glm::vec3 Ray::getHitPoint() const { return hitPoint; }
Surface* Ray::getSceneObject() const { return sceneObject; }

void Ray::setRayDirection(const glm::vec3& dir) { direction = dir; }
void Ray::setRayOrigin(const glm::vec3& origin) { this->origin = origin; }
void Ray::setHitPoint(const glm::vec3& point) { hitPoint = point; }
void Ray::setSceneObject(Surface* obj) { sceneObject = obj; }