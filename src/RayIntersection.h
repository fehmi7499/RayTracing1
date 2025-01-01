#ifndef RAY_INTERSECTION_H
#define RAY_INTERSECTION_H

#include <glm/glm.hpp>
#include <Scene.h>
#include <Ray.h>

// Sphere intersection
bool IntersectSphere(const Ray& ray, const Sphere& sphere, float& t);

// Plane intersection
bool IntersectPlane(const Ray& ray, const Plane& plane, float& t);

// Closest intersection across all objects
bool ClosestIntersection(const Ray& ray, const Scene& scene, float& tMin, int& objectType, int& objectIndex);

// Determine if a point is in shadow
bool IsInShadow(const Ray& shadowRay, const Scene& scene, float lightDistance);

#endif