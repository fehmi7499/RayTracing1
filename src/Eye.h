#pragma once
#include <glm/glm.hpp>

class Eye {
    public:
        explicit Eye(double x = 0.0, double y = 0.0, double z = 0.0);
        glm::vec3 getposition() const;

    private:
        glm::vec3 position;
};