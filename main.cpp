//render mesh
	//prepare mesh
		//triangulate vertices
			//find indices of each vertex (closest point and the closest point between them)
			//delete duplicate indices
		//find normals of each face index (normalize them as well)
	//run camera transformations
		//displace all vertices to local space
		//rotate all vertices and normals
	//render polygon
		//find distance from center
		//interpolate using distance
			//render each point
				//if screen place is written by point closer to camera, dont render point
				//convert all other vertexes to point space
				//find light value
					//find light level
						//if light vector is in between vertices in a normal, disregard light source
						//luminence summation of (normalized light vector - normal vector)

#include "camera.h"
#include "vec3.h"
#include<iostream>
#include<sstream>
#include<conio.h>
#include<chrono>
#include<math.h>
#include<vector>
#include<thread>

#include <fstream>

using namespace std;
bool screenshot = false;
const float pi = 3.14159;
const int torusRadius = 40,
torusThickness = 15;
const float subdA = 0.01,
subdB = 0.05;
float distanceFromCamera = 355,
vFactor = 90,
scr_x = 60,
scr_y = 30;
float A = 1 * pi / 2, B = 0, C = 0;
float VA, VB, VC;
vec3 glv{ 0.0f, 1.0f, 0.0f };
vector<float> zbuffer;
vector<float> buffer;
vector<char> gradTable{ '$', '&', '@', '%', '#', '[', '*', '=', '+', '~', '-', ':', '`', '.', };
bool ifchangedo(char actual, char expected, float& var, float amount) {
	if (actual == expected) {
		var = var + amount;
		return true;
	}
	else {
		return false;
	}
}
void wait() {
	this_thread::sleep_for(10ms);
}

float distanceBetween(float x0, float y0, float z0, float x1, float y1, float z1) {
	float dx = x1 - x0,
		dy = y1 - y0,
		dz = z1 - z0;
	return sqrt(dx * dx + dy * dy + dz * dz);
}
void storePointVector(const vec3& fin, const vec3& cen) {
	vec3 finR = fin.rotate(vec3(A, B, C));
	vec3 cenR = cen.rotate(vec3(A, B, C));
	finR.z = finR.z + distanceFromCamera;
	cenR.z = cenR.z + distanceFromCamera;

	float depth = 1 / finR.z;
	vec3 norm = vec3(finR - cenR).normalize();
	int screenPos_x = round((scr_x / 2) - finR.x * depth * vFactor * 2),
		screenPos_y = round((scr_y / 2) - finR.y * depth * vFactor);
	int screenPos = screenPos_x + screenPos_y * scr_x;
	if (screenPos > 0 && screenPos < scr_x * scr_y && screenPos_x > 0 && screenPos_x < scr_x) {
		if (zbuffer[screenPos] < depth) {
			zbuffer[screenPos] = depth;
			buffer[screenPos] = distanceBetween(norm.x, norm.y, norm.z, glv.x, glv.y, glv.z);
		}
	}
}
void overflowAngle(float& angle) {
	if (angle > 2 * pi) {
		angle = 0;
	}
}
void testMovements() {
	A = A + 0.1 * VA;
	B = B + 0.1 * VB;
	VA = VA / 1.001;
	VB = VB / 1.001;
	if (_kbhit()) {
		char key = _getche();
		//ifchangedo(key, 'w', glv[0], 0.1);
		//ifchangedo(key, 's', glv[0], -0.1);
		//ifchangedo(key, 'a', glv[2], 0.1);
		//ifchangedo(key, 'd', glv[2], -0.1);
		ifchangedo(key, 'w', A, -0.1);
		ifchangedo(key, 's', A, 0.1);
		ifchangedo(key, 'a', B, 0.1);
		ifchangedo(key, 'd', B, -0.1);

		ifchangedo(key, 't', distanceFromCamera, 1);
		ifchangedo(key, 'g', distanceFromCamera, -1);
		ifchangedo(key, 'r', vFactor, 1);
		ifchangedo(key, 'f', vFactor, -1);
		if (key == 'p') {
			screenshot = true;
		}
	}
}
void main() {

	while (true) {
		testMovements();
		zbuffer.clear();
		buffer.clear();
		for (int i = 0; i < scr_x * scr_y * 2; i++) {
			zbuffer.push_back(0.0);
			buffer.push_back(3.0);
		}
		wait();
		for (float slice = 0; slice < 2 * pi; slice = slice + subdA) {
			storePointVector(vec3(torusRadius * cos(slice), 0, torusRadius * sin(slice)), vec3(0, 0, 0));
			float x_cen = torusRadius * cos(slice),
				z_cen = torusRadius * sin(slice),
				y_cen = 0;
			for (float splice = 0; splice < 2 * pi; splice = splice + subdB) {
				float x_fin = (torusRadius + torusThickness * cos(splice)) * cos(slice),
					y_fin = torusThickness * sin(splice),
					z_fin = (torusRadius + torusThickness * cos(splice)) * sin(slice);
				//if ((slice == 0) or ((slice > pi-subdA) and (slice < pi+subdA)) or ((slice > pi / 2 - subdA) and (slice < pi / 2 + subdA))) {
				storePointVector(vec3(x_fin, y_fin, z_fin), vec3(x_cen, y_cen, z_cen));
				//}
			}
		}
		system("cls");
		cout << "I made a donut cus im hungry\nu can use 'wasd' to make the donut dance\n";
		unsigned int k = 0;
		std::ofstream outFile("screenshot.txt");

		for (unsigned int i = 0; i < scr_y; i++) {
			ostringstream line;
			for (unsigned int j = 0; j < scr_x; j++) {
				if (ceil(buffer[k] / 2) > 1) {
					line << ' ';
				}
				else {
					line << gradTable[ceil((buffer[k] / 2) * (gradTable.size() - 1))];
				}
				k++;
			}

			//_debug_break();

			string line2 = "asdfasdfasd  jghfjghfjghfv\nghhfjghfgjhf";
			if (screenshot) {
				//std::to_string()
				//outFile << line2 << "\n";
			}
			cout << line.str() << "\n";
		}
		if (screenshot) {
			screenshot = false;
		}
		outFile.close();
		cout << "A: " << A * 360 / (2 * pi) << "\nB: " << B * 360 / (2 * pi) << "\nC: " << C * 360 / (2 * pi);
		A = A + 0.01;
		B = B + 0.05;
		C = C + 0.05;
		overflowAngle(A);
		overflowAngle(B);
		overflowAngle(C);
	}
}