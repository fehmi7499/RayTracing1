#include "Light.h"

void Light::configureDirection(float x, float y, float z) {
    directionVector = vec3(x, y, z);
}

void Light::configureIntensity(const vec4& intensityVector) {
    lightIntensity = vec3(intensityVector.r, intensityVector.g, intensityVector.b);
    glossiness = intensityVector.w;
}

vec3 Light::retrieveIntensity() const {
    return lightIntensity;
}

DirectionalLight::DirectionalLight(const vec3& direction) {
    illuminationType = IlluminationType::Directional;
    directionVector = direction;
}

SpotLight::SpotLight(const vec3& direction) {
    illuminationType = IlluminationType::Spotlight;
    directionVector = direction;
    angle = 0.0f;
    lightPosition = vec3(0.0f);
}

void SpotLight::configurePosition(float x, float y, float z) {
    lightPosition = vec3(x, y, z);
}

void SpotLight::configureAngle(float angleValue) {
    angle = angleValue;
}

float SpotLight::retrieveAngle() const {
    return angle;
}

vec3 SpotLight::retrievePosition() const {
    return lightPosition;
}