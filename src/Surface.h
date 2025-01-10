
#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
#include <string>

using glm::vec3;
using glm::vec4;

enum class MaterialType { Object, Reflective, Transparent, None };
enum class GeometryType { Plane, Sphere };

enum class IlluminationType { Directional, Spotlight };

class Surface {
protected:
    MaterialType materialType;
    GeometryType geometryType = GeometryType::Plane;
    vec4 parameters = vec4(0.0f);
    vec3 surfaceColor = vec3(0.0f);
    vec3 location = vec3(0.0f);
    float glossiness = 0.0f;

public:
    void assignColor(const vec4& colorWithAlpha);
    void assignGlossiness(float gloss);
    float retrieveGlossiness() const;
    vec3 retrieveParameters() const;
    MaterialType retrieveMaterialType() const;
    GeometryType retrieveGeometryType() const;
    vec3 retrievePosition() const;
    virtual vec3 calculateColor(const vec3& intersectionPoint) const = 0;
};

class Sphere : public Surface {
private:
    double sphereRadius;

public:
    Sphere(double x, double y, double z, double r, MaterialType matType);
    void setSphereRadius(double r);
    vec3 retrievePosition() const;
    float retrieveRadius() const;
    vec3 calculateColor(const vec3& intersectionPoint) const override;
};

class Plane : public Surface {
public:
    Plane(double a, double b, double c, double d, MaterialType matType);
    vec3 retrievePosition() const;
    float retrieveD() const;
    vec3 calculateColor(const vec3& intersectionPoint) const override;
};

#endif