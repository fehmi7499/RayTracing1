#include "Parser.h"
#include "Surface.h"

MaterialType determineObjectType(char objectChar) {
    std::cout << "Determining object type for: " << objectChar << std::endl;
    switch (objectChar) {
        case 'r':
            return MaterialType::Reflective;
        case 't':
            return MaterialType::Transparent;
        case 'o':
            return MaterialType::Object;
        default:
            return MaterialType::None;
    }
}

Sphere* generateSphere(char typeChar, double x, double y, double z, double radius) {
    std::cout << "Generating sphere with parameters: " << typeChar << ", " << x << ", " << y << ", " << z << ", " << radius << std::endl;
    MaterialType objType = determineObjectType(typeChar);
    Sphere* sphere = new Sphere(x, y, z, radius, objType);
    sphere->setSphereRadius(radius);
    return sphere;
}

Plane* generatePlane(char typeChar, double nx, double ny, double nz, double distance) {
    std::cout << "Generating plane with parameters: " << typeChar << ", " << nx << ", " << ny << ", " << nz << ", " << distance << std::endl;
    MaterialType objType = determineObjectType(typeChar);
    Plane* plane = new Plane(nx, ny, nz, distance, objType);
    return plane;
}

SceneParser::SceneParser() {
    std::cout << "Initializing SceneParser" << std::endl;
    this->eye = new Eye(0, 0, 0);
    this->globalIllumination = new vec4(0.0f);
    this->directionalLights = new std::vector<Light*>();
    this->focusedLights = new std::vector<SpotLight*>();
    this->sphereObjects = new std::vector<Sphere*>();
    this->planarObjects = new std::vector<Plane*>();
    this->sceneObjects = new std::vector<Surface*>();
}

void SceneParser::loadScene(const std::string& filePath) {
    std::cout << "Loading scene from file: " << filePath << std::endl;
    int colorIdx = 0, positionIdx = 0, intensityIdx = 0;

    char objectChar = 'a';
    double param1 = 1, param2 = 1, param3 = 1, param4 = 1;

    std::ifstream sceneFile(filePath);
    if (!sceneFile) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(sceneFile, line)) {
        std::cout << "Processing line: " << line << std::endl;
        char identifier = 'a';
        std::vector<double> values;

        std::istringstream lineStream(line);
        lineStream >> identifier;

        double value;
        while (lineStream >> value) {
            values.push_back(value);
        }

        if (values.size() >= 4) {
            param1 = values[0];
            param2 = values[1];
            param3 = values[2];
            param4 = values[3];
        }

        objectChar = identifier;
        std::cout << "Parsed object type: " << objectChar << std::endl;

        switch (objectChar) {
            case 'e':
                std::cout << "Creating camera at position: " << param1 << ", " << param2 << ", " << param3 << std::endl;
                this->eye = new Eye(param1, param2, param3);
                break;
            case 'a':
                std::cout << "Setting ambient light to: " << param1 << ", " << param2 << ", " << param3 << ", " << param4 << std::endl;
                this->globalIllumination = new vec4(param1, param2, param3, param4);
                break;
            case 'd':
                std::cout << "Creating directional light" << std::endl;
                createDirectionalLight(param1, param2, param3, param4);
                break;
            case 'p':
                std::cout << "Updating spotlight position and angle" << std::endl;
                this->focusedLights->at(positionIdx)->configurePosition(param1, param2, param3);
                this->focusedLights->at(positionIdx)->configureAngle(param4);
                positionIdx++;
                break;
            case 'i':
                std::cout << "Setting light intensity" << std::endl;
                this->directionalLights->at(intensityIdx)->configureIntensity(vec4(param1, param2, param3, param4));
                intensityIdx++;
                break;
            case 'c':
                std::cout << "Setting object color and shininess" << std::endl;
                this->sceneObjects->at(colorIdx)->assignColor(vec4(param1, param2, param3, param4));
                this->sceneObjects->at(colorIdx)->assignGlossiness(param4);
                colorIdx++;
                break;
            default:
                std::cout << "Creating object of type: " << objectChar << std::endl;
                if (param4 > 0) {
                    Sphere* sphere = generateSphere(objectChar, param1, param2, param3, param4);
                    this->sphereObjects->push_back(sphere);
                    this->sceneObjects->push_back(sphere);
                } else {
                    Plane* plane = generatePlane(objectChar, param1, param2, param3, param4);
                    this->planarObjects->push_back(plane);
                    this->sceneObjects->push_back(plane);
                }
        }
    }
}

Light* SceneParser::createDirectionalLight(double x, double y, double z, double lightType) {
    std::cout << "Creating directional or spotlight with parameters: " << x << ", " << y << ", " << z << ", type: " << lightType << std::endl;
    Light* light = nullptr;
    if (lightType == 1) {
        light = new SpotLight(vec3(x, y, z));
        this->focusedLights->push_back(static_cast<SpotLight*>(light));
    } else {
        light = new DirectionalLight(vec3(x, y, z));
    }
    light->configureDirection(x, y, z);
    this->directionalLights->push_back(light);
    return light;
}
