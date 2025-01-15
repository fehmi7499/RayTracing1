#include "RayTrace.h"

using namespace glm;

//reflectedRay 
Ray RenderEngine::adjustRay(int row, int col, Surface* excludedSurface, bool isUpdated, Ray reflectedRay, Parser* input) {

    float pixelWidth = 2.0f / ((float) width); //why 2?
    float pixelHeight = 2.0f / ((float) height);

    if (!isUpdated) {
        vec3 topLeftPixel(-1 + pixelWidth / 2, 1 - pixelHeight / 2, 0);
        vec3 pixelPosition = topLeftPixel + vec3(row * pixelWidth, -1 * (col * pixelHeight), 0);
        vec3 cameraPosition = input->eye->getposition();
        vec3 rayDirection = normalize(pixelPosition - cameraPosition);

        reflectedRay.setRayDirection(rayDirection);
        reflectedRay.setRayOrigin(cameraPosition);
    }

    float closestIntersection = INFINITY;
    Surface* nearestSurface = new Plane(0.0, 0.0, 0.0, 0.0, MaterialType::None);
    reflectedRay.setHitPoint(reflectedRay.getRayOrigin() + reflectedRay.getRayDirection());
    reflectedRay.setSceneObject(nearestSurface);

    for (unsigned int objIdx = 0; objIdx < input->objects->size(); objIdx++) {
        float intersectionDistance = 0.0f;
        Surface* currentSurface = input->objects->at(objIdx);

        if (currentSurface != excludedSurface) {

            if (currentSurface->getGeometryType() == GeometryType::Plane) {
                float denominator = dot(reflectedRay.getRayDirection(), currentSurface->getPosition());

                if (abs(denominator) < 0.0001f) {
                    intersectionDistance = -1.0f;
                }
                
                intersectionDistance = -(dot(reflectedRay.getRayOrigin(), currentSurface->getPosition()) + ((Plane*)currentSurface)->getD()) / denominator;
                

            } else {
                vec3 originToCenter = reflectedRay.getRayOrigin() - currentSurface->getPosition();
                float a = dot(reflectedRay.getRayDirection(), reflectedRay.getRayDirection());
                float b = 2.0f * dot(originToCenter, reflectedRay.getRayDirection());
                float c = dot(originToCenter, originToCenter) - pow(((Sphere*)currentSurface)->getRadius(), 2);
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
                reflectedRay.setSceneObject(nearestSurface);
                reflectedRay.setHitPoint(reflectedRay.getRayOrigin() + reflectedRay.getRayDirection() * closestIntersection);
            }
        }
    }

    return reflectedRay;
}

vec3 RenderEngine::computeNormal(const vec3& contactPoint, Surface* entity) {
    if (entity->getGeometryType() == GeometryType::Plane) {
        return normalize(vec3(entity->setParameters()));
    }
    return normalize(contactPoint - ((Sphere*)entity)->getPosition());
}

float RenderEngine::computeDiffuseLighting(const vec3& surfaceNormal, const Ray& ray, Light* lightSource) {
    vec3 lightDirection = normalize(lightSource->direction);
    if (lightSource->illuminationType == IlluminationType::Directional) {
        float cosTheta = dot(surfaceNormal, (ray.getSceneObject()->getGeometryType() == GeometryType::Plane) ? lightDirection : -lightDirection);
        return glm::max(cosTheta, 0.0f);
    } else {
        vec3 spotlightDirection = normalize(ray.getHitPoint() - ((SpotLight*)lightSource)->getPosition());
        float cosTheta = dot(spotlightDirection, lightDirection);

        if (cosTheta < ((SpotLight*)lightSource)->getAngle()) {
            return 0.0f;
        } else {
            lightDirection = (ray.getSceneObject()->getGeometryType() == GeometryType::Plane) ? -spotlightDirection : spotlightDirection;
            cosTheta = dot(surfaceNormal, -lightDirection);
            return glm::max(cosTheta, 0.0f);
        }
    }
}

float RenderEngine::computeSpecularLighting(const vec3& viewPoint, const Ray& ray, Light* lightSource) {
    vec3 lightDirection = normalize(lightSource->direction);
    vec3 surfaceNormal = computeNormal(ray.getHitPoint(), ray.getSceneObject());

    if (lightSource->illuminationType == IlluminationType::Directional) {
        vec3 reflectedRay = lightDirection - 2.0f * surfaceNormal * dot(lightDirection, surfaceNormal);
        float cosTheta = dot(viewPoint, reflectedRay);
        return pow(glm::max(0.0f, cosTheta), ray.getSceneObject()->getShininess());
    } else {
        vec3 spotlightDirection = normalize(ray.getHitPoint() - ((SpotLight*)lightSource)->getPosition());
        float cosTheta = dot(spotlightDirection, lightDirection);

        if (cosTheta < ((SpotLight*)lightSource)->getAngle()) {
            return 0.0f;
        } else {
            lightDirection = spotlightDirection;
            vec3 reflectedRay = lightDirection - 2.0f * surfaceNormal * dot(lightDirection, surfaceNormal);
            cosTheta = dot(viewPoint, reflectedRay);
            return pow(glm::max(0.0f, cosTheta), ray.getSceneObject()->getShininess());
        }
    }
}

float RenderEngine::computeShadowFactor(const Ray& ray, Light* lightSource, Parser* input) {
    vec3 lightDirection = normalize(lightSource->direction);
    float nearestIntersection = INFINITY;

    if (lightSource->illuminationType == IlluminationType::Spotlight) {
        vec3 spotlightDirection = normalize(ray.getHitPoint() - ((SpotLight*)lightSource)->getPosition());
        float cosTheta = dot(spotlightDirection, lightDirection);

        if (cosTheta < ((SpotLight*)lightSource)->getAngle()) {
            return 0.0f;
        } else {
            lightDirection = spotlightDirection;
            nearestIntersection = length(((SpotLight*)lightSource)->getPosition() - ray.getHitPoint());
        }
    }

    for (unsigned int objIdx = 0; objIdx < input->objects->size(); ++objIdx) {
        Surface* entity = input->objects->at(objIdx);

        if (entity != ray.getSceneObject()) {
            Ray shadowRay(-lightDirection, ray.getHitPoint());
            float intersectionDist = 0.0f;

            if (entity->getGeometryType() == GeometryType::Plane) {
                float denominator = dot(shadowRay.getRayDirection(), entity->getPosition());
                if (abs(denominator) > 0.0001f) {
                    intersectionDist = -(dot(shadowRay.getRayOrigin(), entity->getPosition())) + ((Plane*)entity)->getD() / denominator;
                }
            } else if (entity->getGeometryType() == GeometryType::Sphere) {
                vec3 originToCenter = shadowRay.getRayOrigin() - entity->getPosition();
                float a = dot(shadowRay.getRayDirection(), shadowRay.getRayDirection());
                float b = 2.0f * dot(originToCenter, shadowRay.getRayDirection());
                float c = dot(originToCenter, originToCenter) - pow(((Sphere*)entity)->getRadius(), 2);
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

Ray RenderEngine::applyRefractionSL(const Ray& ray, const vec3& surfaceNormal, const vec3& rayDirection, float refractiveIndex) {
    float cosThetaIncident = dot(surfaceNormal, -ray.getRayDirection());
    float sinThetaIncident = sqrt(1.0f - cosThetaIncident * cosThetaIncident);
    float sinThetaRefracted = refractiveIndex * sinThetaIncident;
    float cosThetaRefracted = sqrt(1.0f - sinThetaRefracted * sinThetaRefracted);

    vec3 refractedDir = refractiveIndex * ray.getRayDirection() + (refractiveIndex * cosThetaIncident - cosThetaRefracted) * surfaceNormal;
    return Ray(refractedDir, ray.getHitPoint());
}

vec4 RenderEngine::calculatePixelColor(int x, int y, Ray ray, int depth, Parser* input) {
    vec3 outputColor(0.0f);

    if (ray.getSceneObject()->getMaterialType() == MaterialType::Object) {
        vec3 ambientColor = ray.getSceneObject()->calculateColor(ray.getHitPoint());
        vec3 ambientLight = vec3(input->ambientLight->r, input->ambientLight->g, input->ambientLight->b);

        vec3 cumulativeLighting(0.0f);

        for (unsigned int lightIdx = 0; lightIdx < input->lights->size(); ++lightIdx) {
            Light* lightSource = input->lights->at(lightIdx);
            vec3 specularReflectance = vec3(0.7f) * lightSource->getIntensity();
            vec3 diffuseReflectance = ray.getSceneObject()->calculateColor(ray.getHitPoint()) * lightSource->getIntensity();

            vec3 surfaceNormal = computeNormal(ray.getHitPoint(), ray.getSceneObject());
            vec3 viewVector = normalize(ray.getRayOrigin() - ray.getHitPoint());

            float diffuseIntensity = computeDiffuseLighting(surfaceNormal, ray, lightSource);
            float specularIntensity = computeSpecularLighting(viewVector, ray, lightSource);
            float shadowFactor = computeShadowFactor(ray, lightSource, input);

            cumulativeLighting += (diffuseReflectance * diffuseIntensity + specularReflectance * specularIntensity) * shadowFactor;
        }

        outputColor = ambientColor * ambientLight + cumulativeLighting;
    }

    if (ray.getSceneObject()->getMaterialType() == MaterialType::Reflective) {
        if (depth >= 5) {
            return vec4(0.0f);
        }

        vec3 surfaceNormal = computeNormal(ray.getHitPoint(), ray.getSceneObject());
        vec3 reflectionVector = ray.getRayDirection() - 2.0f * surfaceNormal * dot(ray.getRayDirection(), surfaceNormal);

        Ray reflectedRay(reflectionVector, ray.getHitPoint());
        reflectedRay = adjustRay(x, y, ray.getSceneObject(), true, reflectedRay, input);

        if (reflectedRay.getSceneObject()->getMaterialType() != MaterialType::None) {
            vec4 reflectedColor = calculatePixelColor(x, y, reflectedRay, depth + 1, input);
            outputColor = vec3(reflectedColor);
        }
    }

    if (ray.getSceneObject()->getMaterialType() == MaterialType::Transparent) {
        vec3 surfaceNormal = computeNormal(ray.getHitPoint(), ray.getSceneObject());
        float refractiveIndex = 1.0f / 1.5f;

        Ray refractedRay = applyRefractionSL(ray, surfaceNormal, ray.getRayDirection(), refractiveIndex);
        refractedRay = adjustRay(x, y, ray.getSceneObject(), true, refractedRay, input);

        if (refractedRay.getSceneObject()->getMaterialType() != MaterialType::None) {
            vec4 refractedColor = calculatePixelColor(x, y, refractedRay, depth + 1, input);
            outputColor = vec3(refractedColor);
        }
    }

    outputColor = clamp(outputColor, vec3(0.0f), vec3(1.0f));
    return vec4(outputColor, 1.0f);
}

unsigned char* RenderEngine::RenderImage(const char* sceneFile, int _width, int _height) {
    height = _height;
    width =  _width;
    Parser* scene = new Parser();
    scene->parse(sceneFile);
    auto* imageBuffer = new unsigned char[_height * _width * 4];

    for (int row = 0; row < _height; ++row) {
        for (int col = 0; col < _width; ++col) {
            Ray primaryRay(vec3(0.0f), vec3(0.0f));
            Plane* defaultSurface = new Plane(0.0f, 0.0f, 0.0f, 0.0f, MaterialType::None);
            Ray computedRay = adjustRay(col, row, defaultSurface, false, primaryRay, scene);
            vec4 pixelColor = calculatePixelColor(col, row, computedRay, 0, scene);

            int pixelIndex = (col + _width * row) * 4;
            imageBuffer[pixelIndex] = static_cast<unsigned char>(pixelColor.r * 255);
            imageBuffer[pixelIndex + 1] = static_cast<unsigned char>(pixelColor.g * 255);
            imageBuffer[pixelIndex + 2] = static_cast<unsigned char>(pixelColor.b * 255);
            imageBuffer[pixelIndex + 3] = static_cast<unsigned char>(pixelColor.a * 255);
        }
    }

    delete scene;
    return imageBuffer;
}
