#pragma once
#include <iostream>
#include "Vectors.h"

#define QUATERNION_ADDED

struct Quaternion
{
	float r, i, j, k;

	Quaternion(int r, int i = 0, int j = 0, int k = 0);
	Quaternion(float r = 0.f, float i = 0.f, float j = 0.f, float k = 0.f);
	Quaternion(double r, double i = 0., double j = 0., double k = 0.);
	Quaternion(Vector3f v);

	Quaternion operator+(const Quaternion& other) const;
	Quaternion operator-(const Quaternion& other) const;
	Quaternion operator*(const Quaternion& other) const;
	Quaternion operator/(const Quaternion& other) const;

	Quaternion operator-() const;
	Quaternion operator+(const int& other) const;
	Quaternion operator+(const float& other) const;
	Quaternion operator+(const double& other) const;
	Quaternion operator*(const int& other) const;
	Quaternion operator*(const float& other) const;
	Quaternion operator*(const double& other) const;
	Quaternion operator/(const int& other) const;
	Quaternion operator/(const float& other) const;
	Quaternion operator/(const double& other) const;
	Quaternion operator+=(const Quaternion& other);
	Quaternion operator*=(const Quaternion& other);

	Quaternion	inv() const;
	float		abs() const;
	std::string str() const;
	Vector3f getVector() const;
	_float4vector getVector4() const;
};

Quaternion operator*(const int& lhs, const Quaternion& rhs);
Quaternion operator*(const float& lhs, const Quaternion& rhs);
Quaternion operator*(const double& lhs, const Quaternion& rhs);
Quaternion operator+(const int& lhs, const Quaternion& rhs);
Quaternion operator+(const float& lhs, const Quaternion& rhs);
Quaternion operator+(const double& lhs, const Quaternion& rhs);
Quaternion operator/(const int& lhs, const Quaternion& rhs);
Quaternion operator/(const float& lhs, const Quaternion& rhs);
Quaternion operator/(const double& lhs, const Quaternion& rhs);
Quaternion operator-(const int& lhs, const Quaternion& rhs);
Quaternion operator-(const float& lhs, const Quaternion& rhs);
Quaternion operator-(const double& lhs, const Quaternion& rhs);

Quaternion rotationQuaternion(Vector3f axis, float angle);