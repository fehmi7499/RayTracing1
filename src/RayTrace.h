#pragma once
#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "Parser.h"

class RenderEngine {
public:
    unsigned char* RenderImage(const char* sceneDescription);

private:
    Ray AdjustRay(int row, int column, Surface* surface, bool modify, Ray reflectionRay, Parser* sceneData);
    vec3 ComputeNormal(const vec3& contactPoint, Surface* entity);
    float ComputeDiffuseLighting(const vec3& surfaceNormal, const Ray& incomingRay, Light* illumination);
    float ComputeSpecularLighting(const vec3& observerDirection, const Ray& incomingRay, Light* illumination);
    float ComputeShadowFactor(const Ray& incomingRay, Light* illumination, Parser* sceneData);
    Ray ApplyRefractionSL(const Ray& incomingRay, const vec3& surfaceNormal, const vec3& rayPath, float refractiveIndex);
    vec4 CalculatePixelColor(int row, int column, Ray incomingRay, int recursionDepth, Parser* sceneData);
    
};

#endif
