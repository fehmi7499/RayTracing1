#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <iostream>
#include "RayTrace.h"
#include <stb/stb_image_write.h>

int main(int argc, char* argv[])
{
    const unsigned int width = 800; //why 800?
    const unsigned int height = 800;
    RenderEngine r;
    unsigned char* data = r.RenderImage("res/Scenes/scene6.txt", width, height);
    stbi_write_png("../src/res/textures/scene.png", width, height, 4,data, width * 4);
    free(data);
    return 0;
}