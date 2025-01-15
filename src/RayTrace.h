#pragma once
#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include "Parser.h"

class RenderEngine {
public:
    int height;
    int width;
    unsigned char* RenderImage(const char* sceneDescription, int width, int height);

private:
    Ray adjustRay(int row, int col, Surface* excludedSurface, bool isUpdated, Ray reflectedRay, Parser* input);
    vec3 computeNormal(const vec3& contactPoint, Surface* entity);

    float computeDiffuseLighting(const vec3& surfaceNormal, const Ray& ray, Light* lightSource);
    float computeSpecularLighting(const vec3& viewPoint, const Ray& ray, Light* lightSource);
    float computeShadowFactor(const Ray& ray, Light* lightSource, Parser* input);
    Ray applyRefractionSL(const Ray& ray, const vec3& surfaceNormal, const vec3& rayDirection, float refractiveIndex);
    vec4 calculatePixelColor(int x, int y, Ray ray, int depth, Parser* input);
};

#endif
