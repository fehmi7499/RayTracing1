#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <string>
#include <Scene.h>
#include <Camera.h>

// Parser a scene file and populates the Scene objects
Scene parseScene(const std::string& filename, Camera* camera);

#endif