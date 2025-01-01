
#include <RayIntersection.h>

bool IntersectSphere(const Ray& ray, const Sphere& sphere, float& t)
{
    glm::vec3 oc = ray.origin - sphere.center;  // origin to center Vector
    float a = glm::dot(ray.direction, ray.direction);   // The squared magnitude of the ray's direction vector
    float b = 2.0f * glm::dot(oc, ray.direction);       // Represent the linear term in the quadratic equation for intersection
    float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;  // Determine whether the ray intersects the sphere
    float discriminant = b * b - 4 * a * c;   // Calculate the dicriminant if the quadratic equation

    if (discriminant < 0)
    {
        return false;   // the ray doesn't intersect the sphere
    } else
    {
        t = (-b - sqrt(discriminant)) / (2.0f * a);  // Computes the smaller of the two intersection points
        return t > 0;  // Ensure the intersection is in front of the ray
    }
}

bool IntersectPlane(const Ray& ray, const Plane& plane, float& t)
{
    float denom = glm::dot(plane.normal, ray.direction);  // computes the dot product of the plane's normal vec and the ray's direction
    if(fabs(denom) > 1e-6)  // Ensurde denom is not too close to zero
    {
        glm::vec3 p010 = plane.point - ray.origin;  // Computes the vec from the ray's origin to a point on the plane
        t = glm::dot(p010, plane.normal) / denom;   // t  = distance along the normal /  denom
        return t >= 0; // only consider intersections in front of the ray
    }
    return false;
}

bool ClosestIntersection(const Ray& ray, const Scene& scene, float& tMin, int& objectType, int& objectIndex)
{
    tMin = FLT_MAX;   // Set tMin to a very large value
    bool hit = false;

    // Check spheres - Iterate over all spheres in the scene
    for (size_t i = 0; i < scene.spheres.size(); ++i)
    {
        float t;
        if(IntersectSphere(ray, scene.spheres[i], t) && t < tMin)
        {
            tMin = t;
            objectType = 0;  // Sphere
            objectIndex = i;
            hit = true;
        }
    }

    // Check planes - Iterate over all planes in the scene
    for (size_t i = 0; i < scene.spheres.size(); ++i)
    {
        float t;
        if(IntersectPlane(ray, scene.planes[i], t) && t < tMin)
        {
            tMin = t;
            objectType = 1;  // planes
            objectIndex = i;
            hit = true;
        }
    }

    return hit;
}

bool IsInShadow(const Ray& shadowRay, const Scene& scene, float lightDistance)
{
    float tMin;
    int objectType, objectIndex;

    // Check if the shadow ray intersects any object
    if(ClosestIntersection(shadowRay, scene, tMin, objectType, objectIndex))
    {
        // If the intersection is closer than the light, the point is in shadow
        return tMin < lightDistance;
    }

    return false;   // No occlusion
}



