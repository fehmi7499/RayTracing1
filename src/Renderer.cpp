#include <Renderer.h>
#include <RayIntersection.h>
#include <Lighting.h>
#include <vector>
#include <glm/glm.hpp>
#include <stb_image_write.h>


void RenderScene(const Scene& scene, Camera* camera, int width, int height)
{
    std::vector<glm::vec3> framebuffer(width * height);

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            glm::vec3 pixelColor(0.0f);

            // Generate ray for the current pixel
            glm::vec3 rayDir = glm::normalize(glm::vec3(x - width / 2.0f, y - height / 2.0f, -1.0f));
            Ray ray(camera->GetPosition(), rayDir);

            //Find closest intersection
            float tMin;
            int objectType, objectIndex;
            if (ClosestIntersection(ray, scene, tMin, objectType, objectIndex))
            {
                glm::vec3 hitPoint = ray.origin + ray.direction * tMin;
                pixelColor = CalculateLighting(ray, scene, objectType, objectIndex, hitPoint);
            }

            framebuffer[y * width + x] = glm::clamp(pixelColor, 0.0f, 1.0f);

        }
    }

    // Write the frambuffer to an image
    std::vector<unsigned char> imageData(width * height * 3);
    for (size_t i = 0; i < framebuffer.size(); i++)
    {
        imageData[i * 3 + 0] = (unsigned char)(framebuffer[i].r * 255.0f);
        imageData[i * 3 + 1] = (unsigned char)(framebuffer[i].r * 255.0f);
        imageData[i * 3 + 2] = (unsigned char)(framebuffer[i].r * 255.0f);
    }
    stbi_write_png("output.png", width, height, 3,imageData.data(), width * 3);
}
