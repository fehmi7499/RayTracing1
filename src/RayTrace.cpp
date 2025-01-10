#include "RayTrace.h"
#include "Parser.h"

Ray RenderEngine::AdjustRay(int row, int col, Surface* excludedSurface, bool isUpdated, Ray incomingRay, Parser* sceneData) {

    float pixelWidth = 2.0f / 800.0f;
    float pixelHeight = 2.0f / 800.0f;

    if (!isUpdated) {
        vec3 topLeftPixel(-1.0f + pixelWidth / 2, 1.0f - pixelHeight / 2, 0);
        vec3 pixelPosition = topLeftPixel + vec3(col * pixelWidth, -row * pixelHeight, 0);
        vec3 cameraPosition = sceneData->eye->getposition();
        vec3 rayDirection = normalize(pixelPosition - cameraPosition);

        incomingRay.setRayDirection(rayDirection);
        incomingRay.setRayOrigin(cameraPosition);
    }

    float closestIntersection = INFINITY;
    Surface* nearestSurface = new Plane(0.0f, 0.0f, 0.0f, 0.0f, MaterialType::None);
    incomingRay.setHitPoint(incomingRay.getRayOrigin() + incomingRay.getRayDirection());
    incomingRay.setSceneObject(nearestSurface);

    for (unsigned int objIdx = 0; objIdx < sceneData->objects->size(); objIdx++) {
        float intersectionDistance = 0.0f;
        Surface* currentSurface = sceneData->objects->at(objIdx);

        if (currentSurface != excludedSurface) {

            if (currentSurface->retrieveGeometryType() == GeometryType::Plane) {
                float denominator = glm::dot(incomingRay.getRayDirection(), currentSurface->retrievePosition());

                if (abs(denominator) < 0.0001f) {
                    intersectionDistance = -1.0f;
                } else {
                    intersectionDistance = -(glm::dot(incomingRay.getRayOrigin(), currentSurface->retrievePosition()) + ((Plane*)currentSurface)->retrieveD()) / denominator;
                }

            } else {
                vec3 originToCenter = incomingRay.getRayOrigin() - currentSurface->retrievePosition();
                float a = dot(incomingRay.getRayDirection(), incomingRay.getRayDirection());
                float b = 2.0f * dot(originToCenter, incomingRay.getRayDirection());
                float c = dot(originToCenter, originToCenter) - pow(((Sphere*)currentSurface)->retrieveRadius(), 2);
                float discriminant = b * b - 4 * a * c;

                if (discriminant < 0) {
                    intersectionDistance = -1.0f;
                } else {
                    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
                    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

                    if (t1 < 0 && t2 < 0) {
                        intersectionDistance = -1.0f;
                    } else {
                        intersectionDistance = (t1 >= 0) ? t1 : t2;

                        if (intersectionDistance <= 0.0001f) {
                            intersectionDistance = (t1 >= 0 && t2 >= 0) ? glm::max(t1, t2) : -1.0f;
                        }
                    }
                }
            }

            if ((intersectionDistance >= 0) && intersectionDistance < closestIntersection) {
                nearestSurface = currentSurface;
                closestIntersection = intersectionDistance;
                incomingRay.setSceneObject(nearestSurface);
                incomingRay.setHitPoint(incomingRay.getRayOrigin() + incomingRay.getRayDirection() * closestIntersection);
            }
        }
    }

    return incomingRay;
}

vec3 RenderEngine::ComputeNormal(const vec3& contactPoint, Surface* entity) {
    if (entity->retrieveGeometryType() == GeometryType::Plane) {
        return normalize(vec3(entity->retrieveParameters()));
    }
    return normalize(contactPoint - ((Sphere*)entity)->retrievePosition());
}

float RenderEngine::ComputeDiffuseLighting(const vec3& surfaceNormal, const Ray& ray, Light* lightSource) {
    vec3 lightDirection = normalize(lightSource->directionVector);
    if (lightSource->illuminationType == IlluminationType::Directional) {
        float cosTheta = dot(surfaceNormal, (ray.getSceneObject()->retrieveGeometryType() == GeometryType::Plane) ? lightDirection : -lightDirection);
        return glm::max(cosTheta, 0.0f);
    } else {
        vec3 spotlightDirection = normalize(ray.getHitPoint() - ((SpotLight*)lightSource)->retrievePosition());
        float cosTheta = dot(spotlightDirection, lightDirection);

        if (cosTheta < ((SpotLight*)lightSource)->retrieveAngle()) {
            return 0.0f;
        } else {
            lightDirection = (ray.getSceneObject()->retrieveGeometryType() == GeometryType::Plane) ? -spotlightDirection : spotlightDirection;
            cosTheta = dot(surfaceNormal, -lightDirection);
            return glm::max(cosTheta, 0.0f);
        }
    }
}

float RenderEngine::ComputeSpecularLighting(const vec3& observerDirection, const Ray& ray, Light* lightSource) {
    vec3 lightDirection = normalize(lightSource->directionVector);
    vec3 surfaceNormal = ComputeNormal(ray.getHitPoint(), ray.getSceneObject());

    if (lightSource->illuminationType == IlluminationType::Directional) {
        vec3 reflectedRay = lightDirection - 2.0f * surfaceNormal * dot(lightDirection, surfaceNormal);
        float cosTheta = dot(observerDirection, reflectedRay);
        return pow(glm::max(0.0f, cosTheta), ray.getSceneObject()->retrieveGlossiness());
    } else {
        vec3 spotlightDirection = normalize(ray.getHitPoint() - ((SpotLight*)lightSource)->retrievePosition());
        float cosTheta = dot(spotlightDirection, lightDirection);

        if (cosTheta < ((SpotLight*)lightSource)->retrieveAngle()) {
            return 0.0f;
        } else {
            lightDirection = spotlightDirection;
            vec3 reflectedRay = lightDirection - 2.0f * surfaceNormal * dot(lightDirection, surfaceNormal);
            cosTheta = dot(observerDirection, reflectedRay);
            return pow(glm::max(0.0f, cosTheta), ray.getSceneObject()->retrieveGlossiness());
        }
    }
}

float RenderEngine::ComputeShadowFactor(const Ray& ray, Light* lightSource, Parser* sceneData) {
    vec3 lightDirection = normalize(lightSource->directionVector);
    float nearestIntersection = INFINITY;

    if (lightSource->illuminationType == IlluminationType::Spotlight) {
        vec3 spotlightDirection = normalize(ray.getHitPoint() - ((SpotLight*)lightSource)->retrievePosition());
        float cosTheta = dot(spotlightDirection, lightDirection);

        if (cosTheta < ((SpotLight*)lightSource)->retrieveAngle()) {
            return 0.0f;
        } else {
            lightDirection = spotlightDirection;
            nearestIntersection = glm::length(((SpotLight*)lightSource)->retrievePosition() - ray.getHitPoint());
        }
    }

    for (unsigned int objIdx = 0; objIdx < sceneData->objects->size(); ++objIdx) {
        Surface* entity = sceneData->objects->at(objIdx);

        if (entity != ray.getSceneObject()) {
            Ray shadowRay(-lightDirection, ray.getHitPoint());
            float intersectionDist = 0.0f;

            if (entity->retrieveGeometryType() == GeometryType::Plane) {
                float denominator = glm::dot(shadowRay.getRayDirection(), entity->retrieveParameters());
                if (abs(denominator) > 0.0001f) {
                    intersectionDist = -(glm::dot(shadowRay.getRayOrigin(), entity->retrieveParameters())) + ((Plane*)entity)->retrieveD() / denominator;
                }
            } else if (entity->retrieveGeometryType() == GeometryType::Sphere) {
                vec3 originToCenter = shadowRay.getRayOrigin() - entity->retrievePosition();
                float a = dot(shadowRay.getRayDirection(), shadowRay.getRayDirection());
                float b = 2.0f * dot(originToCenter, shadowRay.getRayDirection());
                float c = dot(originToCenter, originToCenter) - pow(((Sphere*)entity)->retrieveRadius(), 2);
                float discriminant = b * b - 4 * a * c;

                if (discriminant >= 0) {
                    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
                    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
                    intersectionDist = (t1 > 0) ? t1 : t2;
                }
            }

            if ((intersectionDist > 0) && (intersectionDist < nearestIntersection)) {
                return 0.0f;
            }
        }
    }
    return 1.0f;
}

Ray RenderEngine::ApplyRefractionSL(const Ray& ray, const vec3& normal, const vec3& rayDirection, float refractiveIndex) {
    float cosThetaIncident = dot(normal, -ray.getRayDirection());
    float sinThetaIncident = sqrt(1.0f - cosThetaIncident * cosThetaIncident);
    float sinThetaRefracted = refractiveIndex * sinThetaIncident;
    float cosThetaRefracted = sqrt(1.0f - sinThetaRefracted * sinThetaRefracted);

    vec3 refractedDir = refractiveIndex * ray.getRayDirection() + (refractiveIndex * cosThetaIncident - cosThetaRefracted) * normal;
    return Ray(refractedDir, ray.getHitPoint());
}

vec4 RenderEngine::CalculatePixelColor(int x, int y, Ray ray, int depth, Parser* scene) {
    vec3 outputColor(0.0f);

    if (ray.getSceneObject()->retrieveMaterialType() == MaterialType::Object) {
        vec3 ambientColor = ray.getSceneObject()->calculateColor(ray.getHitPoint());
        vec3 ambientLight = vec3(scene->ambientLight->r, scene->ambientLight->g, scene->ambientLight->b);

        vec3 cumulativeLighting(0.0f);

        for (unsigned int lightIdx = 0; lightIdx < scene->lights->size(); ++lightIdx) {
            Light* lightSource = scene->lights->at(lightIdx);
            vec3 specularReflectance = vec3(0.7f) * lightSource->retrieveIntensity();
            vec3 diffuseReflectance = ray.getSceneObject()->calculateColor(ray.getHitPoint()) * lightSource->retrieveIntensity();

            vec3 surfaceNormal = ComputeNormal(ray.getHitPoint(), ray.getSceneObject());
            vec3 viewVector = normalize(ray.getRayOrigin() - ray.getHitPoint());

            float diffuseIntensity = ComputeDiffuseLighting(surfaceNormal, ray, lightSource);
            float specularIntensity = ComputeSpecularLighting(viewVector, ray, lightSource);
            float shadowFactor = ComputeShadowFactor(ray, lightSource, scene);

            cumulativeLighting += (diffuseReflectance * diffuseIntensity + specularReflectance * specularIntensity) * shadowFactor;
        }

        outputColor = ambientColor * ambientLight + cumulativeLighting;
    }

    if (ray.getSceneObject()->retrieveMaterialType() == MaterialType::Reflective) {
        if (depth >= 5) {
            return vec4(0.0f);
        }

        vec3 surfaceNormal = ComputeNormal(ray.getHitPoint(), ray.getSceneObject());
        vec3 reflectionVector = ray.getRayDirection() - 2.0f * surfaceNormal * dot(ray.getRayDirection(), surfaceNormal);

        Ray reflectedRay(reflectionVector, ray.getHitPoint());
        reflectedRay = AdjustRay(x, y, ray.getSceneObject(), true, reflectedRay, scene);

        if (reflectedRay.getSceneObject()->retrieveMaterialType() != MaterialType::None) {
            vec4 reflectedColor = CalculatePixelColor(x, y, reflectedRay, depth + 1, scene);
            outputColor = vec3(reflectedColor);
        }
    }

    if (ray.getSceneObject()->retrieveMaterialType() == MaterialType::Transparent) {
        vec3 surfaceNormal = ComputeNormal(ray.getHitPoint(), ray.getSceneObject());
        float refractiveIndex = 1.0f / 1.5f;

        Ray refractedRay = ApplyRefractionSL(ray, surfaceNormal, ray.getRayDirection(), refractiveIndex);
        refractedRay = AdjustRay(x, y, ray.getSceneObject(), true, refractedRay, scene);

        if (refractedRay.getSceneObject()->retrieveMaterialType() != MaterialType::None) {
            vec4 refractedColor = CalculatePixelColor(x, y, refractedRay, depth + 1, scene);
            outputColor = vec3(refractedColor);
        }
    }

    return vec4(glm::clamp(outputColor, vec3(0.0f), vec3(1.0f)), 1.0f);
}

unsigned char* RenderEngine::RenderImage(const char* sceneFile) {
    Parser* scene = new Parser();
    scene->parse(sceneFile);
    auto* imageBuffer = new unsigned char[800 * 800 * 4];

    for (int row = 0; row < 800; ++row) {
        for (int col = 0; col < 800; ++col) {
            Ray primaryRay(vec3(0.0f), vec3(0.0f));
            Plane* defaultSurface = new Plane(0.0f, 0.0f, 0.0f, 0.0f, MaterialType::None);
            Ray computedRay = AdjustRay(col, row, defaultSurface, false, primaryRay, scene);
            vec4 pixelColor = CalculatePixelColor(col, row, computedRay, 0, scene);

            int pixelIndex = (col + 800 * row) * 4;
            imageBuffer[pixelIndex] = static_cast<unsigned char>(pixelColor.r * 255);
            imageBuffer[pixelIndex + 1] = static_cast<unsigned char>(pixelColor.g * 255);
            imageBuffer[pixelIndex + 2] = static_cast<unsigned char>(pixelColor.b * 255);
            imageBuffer[pixelIndex + 3] = static_cast<unsigned char>(pixelColor.a * 255);
        }
    }

    delete scene;
    return imageBuffer;
}
