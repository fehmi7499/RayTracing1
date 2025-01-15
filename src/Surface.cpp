#include "Surface.h"
#include <cmath>

MaterialType Surface::getMaterialType() const {
    return materialType;
}

GeometryType Surface::getGeometryType() const {
    return geometryType;
}

float Surface::getShininess() const {
    return shininess;
}

vec3 Surface::getPosition() const {  
    return location;
}

void Surface::setColor(const vec4& colorWithAlpha) {
    surfaceColor = vec3(colorWithAlpha.r, colorWithAlpha.g, colorWithAlpha.b);
    shininess = colorWithAlpha.w;    
}

void Surface::setShininess(float gloss) {
    this->shininess = gloss;
}

vec4 Surface::setParameters() const {
    return parameters;
}

float Sphere::getRadius() const {
    return static_cast<float>(sphereRadius);
}

void Sphere::setRadius(double r) {
    sphereRadius = r;
}

float Plane::getD() const {
    return this->parameters.w;
}

Sphere::Sphere(double x, double y, double z, double r, MaterialType matType) {
    parameters = vec4(x, y, z, r);
    location = vec3(x, y, z);
    sphereRadius = r;
    materialType = matType;
    geometryType = GeometryType::Sphere;
}

Plane::Plane(double a, double b, double c, double d, MaterialType matType) {
    this->parameters = vec4(a, b, c, d);
    this->location = vec3(a, b, c);
    this->materialType = matType;
    this->geometryType = GeometryType::Plane;
}

vec3 Sphere::calculateColor(const vec3& intersectionPoint) const {
    return surfaceColor;
}

vec3 Plane::calculateColor(const vec3& intersectionPoint) const { //shoo hadul??
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