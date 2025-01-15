#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Eye {
    public:
        explicit Eye(double x = 0.0, double y = 0.0, double z = 0.0);
        vec3 getposition() const;

    private:
        vec3 position;
};