
#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
#include <string>

//Let's take a little about objects
//All objects (spheres and planes) have a material type that can be normal, reflective or transparent,   
//  so rays can either hit them pass through them 

using namespace glm;

enum class MaterialType { Object, Reflective, Transparent, None };
enum class GeometryType { Plane, Sphere };
enum class IlluminationType { Directional, Spotlight };

class Surface {
protected:
    MaterialType materialType;
    GeometryType geometryType = GeometryType::Plane;
    vec4 parameters = vec4(0, 0, 0, 0);
    vec3 surfaceColor = vec3(0, 0, 0);
    vec3 location = vec3(0, 0, 0);
    float shininess = 0;

public:
    MaterialType getMaterialType() const;
    GeometryType getGeometryType() const;
    float getShininess() const;
    vec3 getPosition() const;
    void setColor(const vec4& colorWithAlpha);
    void setShininess(float gloss);
    vec4 setParameters() const;
    virtual vec3 calculateColor(const vec3& intersectionPoint) const = 0;
};

class Sphere : public Surface {
private:
    double sphereRadius;

public:
    Sphere(double x, double y, double z, double r, MaterialType matType);
    float getRadius() const;
    void setRadius(double r);
    vec3 calculateColor(const vec3& intersectionPoint) const override;
};

class Plane : public Surface {
public:
    Plane(double a, double b, double c, double d, MaterialType matType);
    float getD() const;
    vec3 calculateColor(const vec3& intersectionPoint) const override;
};

#endif