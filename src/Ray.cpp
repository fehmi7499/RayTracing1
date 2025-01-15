#include "Ray.h"

using namespace glm;

Ray::Ray(const vec3& direction, const vec3& origin)
    : direction(direction), origin(origin), hitPoint((origin + direction)), sceneObject((new Plane(0.0, 0.0, 0.0, 0.0, MaterialType::None))) {}

vec3 Ray::getRayDirection() const { return direction; }
vec3 Ray::getRayOrigin() const { return origin; }
vec3 Ray::getHitPoint() const { return hitPoint; }
Surface* Ray::getSceneObject() const { return sceneObject; }

void Ray::setRayDirection(const vec3& dir) { direction = dir; }
void Ray::setRayOrigin(const vec3& origin) { this->origin = origin; }
void Ray::setHitPoint(const vec3& point) { hitPoint = point; }
void Ray::setSceneObject(Surface* obj) { sceneObject = obj; }