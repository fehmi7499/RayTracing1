#ifndef RENDERER_H
#define RENDERER_H

#include <Scene.h>
#include <Ray.h>

// Main render function
void RenderScene(const Scene& scene, Camera* camera, int width, int height);

#endif