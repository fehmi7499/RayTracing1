# pragma once 
# ifndef light_h 
# define light_h 

#include <vector> 
#include <glm/glm.hpp> 
#include <string> 
#include "Surface.h" 
#include "Ray.h" 
#include "Eye.h" 

using namespace glm;

class Light {
    public: 
        vec3 direction; 
        vec3 intensity; //color
        float shininess = 0.0f; 
        IlluminationType illuminationType;

        void setDirection(float x, float y, float z); 
        void setIntensity(const vec4 &intensity);
        vec3 getIntensity() const; 
}; 

class DirectionalLight : public Light {
    public: 
        DirectionalLight(const vec3 &direction); 
}; 

class SpotLight : public Light { 
    private: 
        vec3 position; 
        float angle = 0;; 

    public: 
        SpotLight(const vec3 &direction); 
        void setPosition(float x, float y, float z); 
        vec3 getPosition() const; 
        void setAngle(float angleValue); 
        float getAngle() const; 
}; 
  
# endif 