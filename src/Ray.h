#pragma once
#include <glm/glm.hpp>
#include "Surface.h"

class Ray {
    public:
        Ray(const glm::vec3& direction, const glm::vec3& origin);

        glm::vec3 getRayDirection() const;
        glm::vec3 getRayOrigin() const;
        glm::vec3 getHitPoint() const;
        Surface* getSceneObject() const;

        void setRayDirection(const glm::vec3& dir);
        void setRayOrigin(const glm::vec3& origin);
        void setHitPoint(const glm::vec3& point);
        void setSceneObject(Surface* obj);

    private:
        glm::vec3 direction, origin, hitPoint;
        Surface* sceneObject;
};