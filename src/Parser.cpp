#include "Parser.h"

using namespace std;
using namespace glm;

Parser::Parser() {
    this->eye = new Eye(0, 0, 0);
    this->ambientLight = new vec4(0.0f);
    this->lights = new vector<Light*>(); 
    this->spotlights = new vector<SpotLight*>();
    this->spheres = new vector<Sphere*>();
    this->planes = new vector<Plane*>();
};

MaterialType getObjectType(char objType) {
    switch (objType) {
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

Sphere* createSphere(char objType, double x, double y, double z, double r) {
    return new Sphere(x, y, z, r, getObjectType(objType));
}

Plane* createPlane(char objType, double x, double y, double z, double d) {
    return new Plane(x, y, z, d, getObjectType(objType));
}

void Parser::parse(const std::string& fileName) {
    int colorIdx = 0;
    int posIdx = 0;
    int intensityIdx = 0;
    char objType = 'a';
    double v1 = 1, v2 = 1, v3 = 1, v4 = 1;

    ifstream inputFile(fileName);
    if (!inputFile) {
        cerr << "Error opening file " << fileName << ": " << strerror(errno) << endl;
        return;
    }

    string line;
    while (std::getline(inputFile, line)) {
        char lineType = 'a';
        vector<double> params;

        istringstream lineStream(line);
        lineStream >> lineType;

        double number;

        while (lineStream >> number) {
            params.push_back(number);
        }

        if (params.size() >= 4) {
            v1 = params[0];
            v2 = params[1];
            v3 = params[2];
            v4 = params[3];
        }

        objType = lineType;

        switch (objType) {
        case 'e':
            this->eye = new Eye(v1, v2, v3);
            break;
        case 'a':
            this->ambientLight = new vec4(v1, v2, v3, v4);
            break;
        case 'd':
            parseLightDirection(v1, v2, v3, v4);
            break;
        case 'p':
            this->spotlights->at(posIdx)->setPosition(v1, v2, v3);
            this->spotlights->at(posIdx)->setAngle(v4);
            posIdx++;
            break;
        case 'i':
            this->lights->at(intensityIdx)->setIntensity(vec4(v1, v2, v3, v4));
            intensityIdx++;
            break;
        case 'c':
            this->objects->at(colorIdx)->setColor(vec4(v1, v2, v3, v4));
            this->objects->at(colorIdx)->setShininess(v4);
            colorIdx++;
            break;
        default:
            if (v4 > 0) {
                Sphere* sphere = createSphere(objType, v1, v2, v3, v4);
                this->spheres->push_back(sphere);
                this->objects->push_back(sphere);
            } else {
                Plane* plane = createPlane(objType, v1, v2, v3, v4);
                this->planes->push_back(plane);
                this->objects->push_back(plane);
            }
        }
    }
}

Light* Parser::parseLightDirection(double x, double y, double z, double lightType) {
    Light* light = nullptr; 
    if (lightType == 1) { //spotlight 
        light = new SpotLight(vec3(x, y, z));
        this->spotlights->push_back((SpotLight*)light);
    } else { //directional
        light = new DirectionalLight(vec3(x, y, z));
    }
    light->setDirection(x, y, z);
    this->lights->push_back(light);
    return light;
}