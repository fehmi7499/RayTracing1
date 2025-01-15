#include "Light.h"

void Light::setDirection(float x, float y, float z) {
    direction = vec3(x, y, z);
}

void Light::setIntensity(const vec4& _intensity) {
    intensity = vec3(_intensity.r, _intensity.g, _intensity.b);
    shininess = _intensity.w; 
}

vec3 Light::getIntensity() const {
    return intensity;
}

//----------------------------------------

DirectionalLight::DirectionalLight(const vec3& _direction) {
    illuminationType = IlluminationType::Directional;
    direction = _direction;
}

//----------------------------------------

SpotLight::SpotLight(const vec3& _direction) {
    illuminationType = IlluminationType::Spotlight;
    direction = _direction;
    angle = 0.0f;
    position = vec3(0, 0, 0);
}

void SpotLight::setPosition(float x, float y, float z) {
    position = vec3(x, y, z);
}

vec3 SpotLight::getPosition() const {
    return position;
}

void SpotLight::setAngle(float angleValue) {
    angle = angleValue;
}

float SpotLight::getAngle() const { //fixed
    return angle;
}

