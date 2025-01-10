#pragma once
#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <fstream>
#include "Light.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

using glm::vec4;
using glm::vec3;

class SceneParser {
public:
    Eye* eye;
    vec4* globalIllumination;
    std::vector<Light*>* directionalLights;
    std::vector<SpotLight*>* focusedLights;
    std::vector<Sphere*>* sphereObjects;
    std::vector<Plane*>* planarObjects;
    std::vector<Surface*>* sceneObjects;

    SceneParser();

    void loadScene(const std::string& filePath);
    Light* createDirectionalLight(double x, double y, double z, double lightType);
};

#endif
