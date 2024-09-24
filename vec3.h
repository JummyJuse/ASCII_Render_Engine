#ifndef VEC3_H
#define VEC3_H

struct vec3 {
	float x, y, z;
	vec3();
	vec3(float x, float y, float z);
	
	vec3 operator+(const vec3& v) const;
	vec3 operator-(const vec3& v) const;
	vec3 operator*(float scalar) const;

	float dot(const vec3& v) const;
	vec3 cross(const vec3& v) const;

	float length() const;
	vec3 normalize() const;
	vec3 rotate(const vec3& r) const;
};
#endif
