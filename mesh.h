#pragma once

#include <vector>
#include <string>
#include "vec3.h"

// Single definition of Face
struct Face {
    int a;
    int b;
    int c;        // indices into meshVertices
    vec3 normal;  // face normal

    Face() : a(0), b(0), c(0), normal() {}
};

//Global mesh data 
extern std::vector<vec3> meshVertices;
extern std::vector<Face> meshFaces;

// OBJ loader declaration
bool loadOBJ(const std::string& filename,
    std::vector<vec3>& vertices,
    std::vector<Face>& faces);
