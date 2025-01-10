#include "Eye.h"

Eye::Eye(double x, double y, double z) : position(x, y, z) {}

glm::vec3 Eye::getposition() const {
    return position;
}