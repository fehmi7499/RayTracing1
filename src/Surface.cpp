#include "Surface.h"
#include <cmath>

void Surface::assignColor(const vec4& colorWithAlpha) {
    surfaceColor = vec3(colorWithAlpha);
    glossiness = colorWithAlpha.w;    
}

void Surface::assignGlossiness(float gloss) {
    this->glossiness = gloss;
}

float Surface::retrieveGlossiness() const {
    return glossiness;
}

vec3 Surface::retrieveParameters() const {
    return parameters;
}

MaterialType Surface::retrieveMaterialType() const {
    return materialType;
}

GeometryType Surface::retrieveGeometryType() const {
    return geometryType;
}

vec3 Surface::retrievePosition() const {  
    return location;
}

Sphere::Sphere(double x, double y, double z, double r, MaterialType matType) {
    parameters = vec4(x, y, z, r);
    location = vec3(x, y, z);
    sphereRadius = r;
    materialType = matType;
    geometryType = GeometryType::Sphere;
}

void Sphere::setSphereRadius(double r) {
    sphereRadius = r;
}

vec3 Sphere::retrievePosition() const {
    return location;
}

float Sphere::retrieveRadius() const {
    return static_cast<float>(sphereRadius);
}

vec3 Sphere::calculateColor(const vec3& intersectionPoint) const {
    return surfaceColor;
}

Plane::Plane(double a, double b, double c, double d, MaterialType matType) {
    parameters = vec4(a, b, c, d);
    location = vec3(a, b, c);
    materialType = matType;
    geometryType = GeometryType::Plane;
}

vec3 Plane::retrievePosition() const {
    return location;
}

float Plane::retrieveD() const {
    return parameters.w;
}

vec3 Plane::calculateColor(const vec3& intersectionPoint) const {
    float patternScale = 0.5f;
    float chesspattern = 0.0f;

    chesspattern += (intersectionPoint.x < 0.0f) 
        ? floor((0.5f - intersectionPoint.x) / patternScale)
        : floor(intersectionPoint.x / patternScale);

    chesspattern += (intersectionPoint.y < 0.0f) 
        ? floor((0.5f - intersectionPoint.y) / patternScale)
        : floor(intersectionPoint.y / patternScale);

    chesspattern = fmod(chesspattern, 2.0f);
    return (chesspattern > 0.5f) ? 0.5f * surfaceColor : surfaceColor;
}