#pragma once
#include <glm/glm.hpp>
#include "Surface.h"

using namespace glm;

class Ray { //a ray may have a direction, originand a hit point.
    public:
        Ray(const vec3& direction, const vec3& origin);

        vec3 getRayDirection() const;
        vec3 getRayOrigin() const;
        vec3 getHitPoint() const;
        Surface* getSceneObject() const;

        void setRayDirection(const vec3& dir);
        void setRayOrigin(const vec3& origin);
        void setHitPoint(const vec3& point);
        void setSceneObject(Surface* obj);

    private:
        vec3 direction, origin, hitPoint;
        Surface* sceneObject;
};