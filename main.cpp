#include <iostream>
#include <sstream>
#include <conio.h>
#include <chrono>
#include <math.h>
#include <vector>
#include <thread>

#include "vec3.h"
#include "control.h"
#include "mesh.h"    

using namespace std;

const float pi = 3.14159f;

float distanceFromCamera = 10.0f;
float vFactor = 90.0f;
float scr_x = 100.0f;
float scr_y = 50.0f;

float A = 1.0f * pi / 2.0f, B = 0.0f, C = 0.0f;
float VA = 0.0f, VB = 0.0f, VC = 0.0f;

vec3 glv{ 0.0f, 1.0f, 0.0f };

vector<float> zbuffer;
vector<float> buffer;
vector<char> gradTable{ '$', '&', '@', '%', '#', '[', '*', '=', '+', '~', '-', ':', '`', '.', };

void wait() {
    this_thread::sleep_for(chrono::milliseconds(10));
}

float distanceBetween(float x0, float y0, float z0, float x1, float y1, float z1) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float dz = z1 - z0;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

void overflowAngle(float& angle) {
    if (angle > 2.0f * pi) {
        angle -= 2.0f * pi;
    }
    if (angle < 0.0f) {
        angle += 2.0f * pi;
    }
}

void storePointVectorWithNormal(const vec3& fin, const vec3& normalWorld) {
    vec3 finR = fin.rotate(vec3(A, B, C));
    vec3 normR = normalWorld.rotate(vec3(A, B, C)).normalize();

    finR.z = finR.z + distanceFromCamera;
    if (finR.z <= 0.1f) return;

    float depth = 1.0f / finR.z;

    int screenPos_x = static_cast<int>(round((scr_x / 2.0f) - finR.x * depth * vFactor * 2.0f));
    int screenPos_y = static_cast<int>(round((scr_y / 2.0f) - finR.y * depth * vFactor));
    int screenPos = screenPos_x + screenPos_y * static_cast<int>(scr_x);

    if (screenPos > 0 &&
        screenPos < static_cast<int>(scr_x * scr_y) &&
        screenPos_x > 0 && screenPos_x < static_cast<int>(scr_x)) {

        if (zbuffer[screenPos] < depth) {
            zbuffer[screenPos] = depth;

            vec3 glvNorm = glv.normalize();
            buffer[screenPos] = distanceBetween(normR.x, normR.y, normR.z,
                glvNorm.x, glvNorm.y, glvNorm.z);
        }
    }
}

void testMovements() {
    A = A + 0.1f * VA;
    B = B + 0.1f * VB;
    VA = VA / 1.001f;
    VB = VB / 1.001f;

    if (_kbhit()) {
        char key = _getche();

        ifchangedo(key, 'w', A, -0.1f);
        ifchangedo(key, 's', A, 0.1f);
        ifchangedo(key, 'a', B, 0.1f);
        ifchangedo(key, 'd', B, -0.1f);

        ifchangedo(key, 't', distanceFromCamera, 1.0f);
        ifchangedo(key, 'g', distanceFromCamera, -1.0f);
        ifchangedo(key, 'r', vFactor, 1.0f);
        ifchangedo(key, 'f', vFactor, -1.0f);
    }
}

int main(int argc, char* argv[]) {
    std::string objPath;

    if (argc > 1) {
        objPath = argv[1];
    }
    else {
        std::cout << "enterr the path to specified OBJ file (or press Enter for default model.obj): ";
        std::getline(std::cin, objPath);
        if (objPath.empty()) {
            objPath = "model.obj";
        }
    }

    if (!loadOBJ(objPath, meshVertices, meshFaces)) {
        std::cout << "failed to load the OBJ file: " << objPath << "\n";
        return 1;
    }


    while (true) {
        testMovements();

        size_t pixels = static_cast<size_t>(scr_x * scr_y);
        zbuffer.assign(pixels, 0.0f);
        buffer.assign(pixels, 3.0f);

        wait();

        for (size_t i = 0; i < meshFaces.size(); ++i) {
            const Face& f = meshFaces[i];

            vec3 v0 = meshVertices[f.a];
            vec3 v1 = meshVertices[f.b];
            vec3 v2 = meshVertices[f.c];

            vec3 nCam = f.normal.rotate(vec3(A, B, C));
            if (nCam.z >= 0.0f) continue;

            // ALWAYS 2 arguments:
            storePointVectorWithNormal(v0, f.normal);
            storePointVectorWithNormal(v1, f.normal);
            storePointVectorWithNormal(v2, f.normal);
        }

        system("cls");
        cout << "Mesh viewer (WASD rotate, T/G zoom, R/F FOV)\n";

        size_t k = 0;
        ostringstream line;
        for (int i = 0; i < static_cast<int>(scr_y); i++) {
            for (int j = 0; j < static_cast<int>(scr_x); j++) {
                float val = buffer[k] / 2.0f;

                if (val > 1.0f) {
                    line << ' ';
                }
                else {
                    if (val < 0.0f) val = 0.0f;
                    size_t idx = static_cast<size_t>(
                        floor(val * static_cast<float>(gradTable.size() - 1))
                        );
                    if (idx >= gradTable.size()) idx = gradTable.size() - 1;
                    line << gradTable[idx];
                }
                ++k;
            }
            line << "\n";
        }
        cout << line.str() << "\n";
  
        //A = A + 0.01f;
        //B = B + 0.05f;
        //C = C + 0.05f;
        overflowAngle(A);
        overflowAngle(B);
        overflowAngle(C);
    }

    return 0;
}
