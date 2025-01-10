# pragma once 
# ifndef light_h 
# define light_h 

#include <vector> 
#include <glm/glm.hpp> 
#include <string> 
#include "Surface.h" 
#include "Ray.h" 
#include "Eye.h" 

using glm::vec3; using glm::vec4; 

class Light {
public: 
 vec3 directionVector; 
 vec3 lightIntensity; 
 float glossiness = 0.0f; 
 IlluminationType illuminationType; 

 void configureDirection(float x, float y, float z); 
 void configureIntensity(const vec4 &intensityVector); 
 vec3 retrieveIntensity() const; 
}; 

class DirectionalLight: public Light {
public: 
 DirectionalLight(const vec3 &direction); 
}; 

class SpotLight : public Light { 
 private: 
 vec3 lightPosition; 
 float angle = 0.0f; 
  
 public: 
  
 SpotLight(const vec3 &direction); 
 void configurePosition(float x, float y, float z); 
 void configureAngle(float angleValue); 
 float retrieveAngle() const; 
 vec3 retrievePosition() const; 
}; 
  
# endif 