#include <Lighting.h>
#include <RayIntersection.h>

glm::vec3 CalculateLighting(const Ray& ray, const Scene& scene, int objectType, int objectIndex, const glm::vec3& hitPoint)
{
    glm::vec3 color(0.0f);  // Start with no light (black)

    // Ambient lighting
    color += scene.ambientLight.color * scene.ambientLight.intensity;

    // Diffuse and specular lighting for each light source
    for(const auto& light : scene.directionalLights)
    {
        glm::vec3 lightDir = glm::normalize(-light.direction);

        // Cast shadow ray
        Ray shadowray(hitPoint + 1e-4f * lightDir, lightDir); // Offset to avoid self-intersection
        if(IsInShadow(shadowray, scene, FLT_MAX))
        {
            continue;   // skip this light if the point is in shadow
        }

        
        if(objectType == 0)  // Sphere
        {
            const Sphere& sphere = scene.spheres[objectIndex];
            glm::vec3 normal = glm::normalize(hitPoint - sphere.center);

            // Diffuse lighting
            float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
            color += sphere.color * light.intensity * diff;

            // Specular lighting
            glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
            float spec = pow(glm::max(glm::dot(ray.direction, reflectDir),0.0f), sphere.shininess);
            color += light.intensity * spec;
        } 
        else if(objectType == 1)   // Plane
        {
            const Plane& plane = scene.planes[objectIndex];
            glm::vec3 normal = plane.normal;

            // Diffuse lighting
            float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
            color += plane.color * light.intensity * diff;

            // Specular lighting
            glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
            float spec = pow(glm::max(glm::dot(ray.direction, reflectDir),0.0f), plane.shininess);
            color += light.intensity * spec;

        }
    }

    return glm::clamp(color, 0.0f, 1.0f);  // Clamp color ti valid range [0, 1]
}