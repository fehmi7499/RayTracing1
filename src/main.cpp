#include <string>
#include <Camera.h>
#include <Scene.h>
#include <SceneParser.h>
#include <Renderer.h>

#include <iostream>

int main(int argc, char* argv[])
{
    // Define scene parameters
    const std::string sceneFile = "scene1.txt";
    const int width = 800;
    const int height = 600;

    // Initializing the camera
    Camera camera(width, height);

    // Parse the scene from the file
    Scene scene(&camera, AmbientLight(glm::vec3(0.0f), 0.0f));  // Default ambient light
    try{
        scene = parseScene(sceneFile, &camera);
        std::cout << "Scene parsed succefully!" << std::endl;
    } catch (const std::exception& e) {
        std:: cerr << "Eroor : " << e.what() << std::endl;
        return -1; 
    }

    // Render the scene
    RenderScene(scene, &camera, width, height);

    std::cout << "Rendering complete. check 'output.png' for results." << std::endl;

    return 0;
}