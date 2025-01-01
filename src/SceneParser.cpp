#include <fstream>
#include <sstream>

#include <SceneParser.h>

Scene parseScene(const std::string& filename, Camera* camera)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("could not open scene file");
    }


    AmbientLight ambientLight(glm::vec3(1,1,1), 1.0f);
    std::vector<DirectionalLight> directionalLight;
    std::vector<Spotlight> spotlights;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // Camera position
        if(type == "e")   
        {
            float x,y,z;
            iss >> x >> y >>z;
            camera->SetPosition(glm::vec3(x, y, z));
        }
        // Camera orientation (look-at direction)
        else if(type == "l")
        {
            float x, y, z;
            iss >> x >> y >>z;
            camera->SetOrientation(glm::vec3(x, y, z));
        }
        // Ambient light
        else if(type == "a")
        {
            float r, g, b, ignore;
            iss >> r >> g >> b >> ignore;
            ambientLight = AmbientLight(glm::vec3(r, g, b), 1.0f); //use only the RGB values
        }
        // Directional light
        else if(type == "d")
        {
            float x, y, z, typeFlag;
            iss >> x >> y >> z >> typeFlag;
            if(typeFlag == 0.0f)
            { 
                directionalLight.emplace_back(glm::vec3(x, y, z), glm::vec3(1.0f, 1.0f, 1.0f)); // Default intensity
            }else if(typeFlag == 1.0f)
            {
                spotlights.emplace_back(glm::vec3(0, 0, 0), glm::vec3(x, y, z), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f)); // placeholder values   
            }
        }
        // Spotlight
        else if(type == "p")
        {
            float x, y, z, cutoffCos;
            iss >> x >> y >> z >> cutoffCos;
            if(!spotlights.empty())
            {
                spotlights.back().position = glm::vec3(x, y, z);
                spotlights.back().cutoffAngle = cutoffCos;
            }
        }
        // Sphere or plane object
        else if(type == "o")
        {
            float x, y, z, w;
            iss >> x >> y >> z >> w;
            if(w > 0.0f) // Sphere
            {
                spheres.emplace_back(glm::vec3(x, y, z), w, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f); // Default color and shininess
            } else   // Plane
            {
                planes.emplace_back(glm::vec3(x, y, z), glm::normalize(glm::vec3(x, y, z)),glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);  // Default color and shininess
            }
        }
        // reflective object
        else if(type == "r") 
        {   // reflective objects don't add new spheres/planes but modify the last added object
            if(!spheres.empty())
            {
                spheres.back().reflection = 1.0f;  // Fully reflective
            } else if(!spheres.empty())
            {
                planes.back().reflection = 1.0f;  // Fully reflective
            }
        }
        // Transparent object
        else if(type == "t")
        {
            if(!spheres.empty())
            {
                spheres.back().refraction = 1.5f;  // 1.5 glass refracrion
            } else if(!planes.empty())
            {
                planes.back().refraction = 1.5f;  // 1.5 glass refraction
            }
        }
        // color and shininess
        else if(type == "c")
        {
            float r, g, b, shinniness;
            iss >> r >> g >> b >> shinniness;
            if(!spheres.empty())
            {
                spheres.back().color = glm::vec3(r, g, b); 
                spheres.back().shininess = shinniness;
            } else if(!planes.empty())
            {
                planes.back().color = glm::vec3(r, g, b);
                planes.back().shininess = shinniness;
            }
        }
    }
    return Scene(camera, ambientLight);
}
