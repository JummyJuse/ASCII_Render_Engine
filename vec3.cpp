#include "vec3.h"
#include <cmath>

vec3::vec3() : x(0), y(0), z(0) {};

vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {};

vec3 vec3::operator+(const vec3& v) const {
	return vec3(x + v.x, y + v.y, z + v.z);
}
vec3 vec3::operator-(const vec3& v) const {
	return vec3(x - v.x, y - v.y, z - v.z);
}
vec3 vec3::operator*(float scalar) const {
	return vec3(x * scalar, y * scalar, z * scalar);
}

float vec3::dot(const vec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}
vec3 vec3::cross(const vec3& v) const {
	return vec3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}
float vec3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}
vec3 vec3::normalize() const {
	float len = length();
	return vec3(x / len, y / len, z / len);
}
vec3 vec3::rotate(const vec3& r) const
{
	return vec3(
		(y * sin(r.x) * sin(r.y) * cos(r.z))
		- (z * cos(r.x) * sin(r.y) * cos(r.z))
		+ (y * cos(r.x) * sin(r.z))
		+ (z * sin(r.x) * sin(r.z))
		+ (x * cos(r.y) * cos(r.z)),
		(y * cos(r.x) * cos(r.z))
		+ (z * sin(r.x) * cos(r.z))
		- (y * sin(r.x) * sin(r.y) * sin(r.z))
		+ (z * cos(r.x) * sin(r.y) * sin(r.z))
		- (x * cos(r.y) * sin(r.z)),
		(z * cos(r.x) * cos(r.y))
		- (y * sin(r.x) * cos(r.y))
		+ (x * sin(r.y))
	);
}
