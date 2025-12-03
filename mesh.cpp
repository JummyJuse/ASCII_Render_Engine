#include "mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::vector<vec3> meshVertices;
std::vector<Face> meshFaces;

bool loadOBJ(const std::string& filename,
    std::vector<vec3>& vertices,
    std::vector<Face>& faces)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return false;
    }

    std::vector<vec3> tempVerts;
    std::vector<Face> tempFaces;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            tempVerts.emplace_back(x, y, z);
        }
        else if (prefix == "f") {
            std::string s1, s2, s3;
            iss >> s1 >> s2 >> s3;
            if (s1.empty() || s2.empty() || s3.empty()) continue;

            auto parseIndex = [](const std::string& s) {
                size_t slash = s.find('/');
                std::string num = (slash == std::string::npos) ? s : s.substr(0, slash);
                return std::stoi(num);
                };

            int i1 = parseIndex(s1);
            int i2 = parseIndex(s2);
            int i3 = parseIndex(s3);

            Face f;
            f.a = i1 - 1;   // OBJ is 1-based
            f.b = i2 - 1;
            f.c = i3 - 1;
            tempFaces.push_back(f);
        }
    }

    file.close();

    if (tempVerts.empty() || tempFaces.empty()) {
        std::cerr << "OBJ has no vertices or faces.\n";
        return false;
    }

    //compute face normals
    for (size_t i = 0; i < tempFaces.size(); ++i) {
        Face& f = tempFaces[i];
        vec3 v0 = tempVerts[f.a];
        vec3 v1 = tempVerts[f.b];
        vec3 v2 = tempVerts[f.c];
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        f.normal = e1.cross(e2).normalize();
    }

    vertices = tempVerts;
    faces = tempFaces;
    return true;
}
