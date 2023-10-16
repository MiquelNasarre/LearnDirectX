#pragma once

#include <iostream>

struct Vector2i;
struct Vector2f;
struct Vector2d;

struct Vector2d {
	double x;
	double y;

	Vector2d();
	Vector2d(double x, double y);
	Vector2d(float x, float y);
	Vector2d(int x, int y);
	Vector2d(const Vector2i& other);
	Vector2d(const Vector2f& other);

	Vector2d operator+(const Vector2d& other);
	Vector2d operator+(const Vector2f& other);
	Vector2d operator+(const Vector2i& other);
	Vector2d operator-(const Vector2d& other);
	Vector2d operator-(const Vector2f& other);
	Vector2d operator-(const Vector2i& other);
	Vector2d operator-();
	Vector2d operator*(const int& other);
	Vector2d operator*(const float& other);
	Vector2d operator*(const double& other);
	Vector2d operator/(const int& other);
	Vector2d operator/(const float& other);
	Vector2d operator/(const double& other);
	Vector2d& operator+=(const Vector2d& other);
	Vector2d& operator+=(const Vector2f& other);
	Vector2d& operator+=(const Vector2i& other);
	Vector2d& operator-=(const Vector2d& other);
	Vector2d& operator-=(const Vector2f& other);
	Vector2d& operator-=(const Vector2i& other);

	double		abs();
	std::string str();
};

struct Vector2f {
	float x;
	float y;

	Vector2f();
	Vector2f(double x, double y);
	Vector2f(float x, float y);
	Vector2f(int x, int y);
	Vector2f(const Vector2i& other);
	Vector2f(const Vector2d& other);

	Vector2d operator+(const Vector2d& other);
	Vector2f operator+(const Vector2f& other);
	Vector2f operator+(const Vector2i& other);
	Vector2d operator-(const Vector2d& other);
	Vector2f operator-(const Vector2f& other);
	Vector2f operator-(const Vector2i& other);
	Vector2f operator-();
	Vector2f operator*(const int& other);
	Vector2f operator*(const float& other);
	Vector2d operator*(const double& other);
	Vector2f operator/(const int& other);
	Vector2f operator/(const float& other);
	Vector2d operator/(const double& other);
	Vector2f& operator+=(const Vector2f& other);
	Vector2f& operator+=(const Vector2i& other);
	Vector2f& operator-=(const Vector2f& other);
	Vector2f& operator-=(const Vector2i& other);

	float		abs();
	std::string str();
};

struct Vector2i {
	int x;
	int y;

	Vector2i();
	Vector2i(double x, double y);
	Vector2i(float x, float y);
	Vector2i(int x, int y);
	Vector2i(const Vector2f& other);
	Vector2i(const Vector2d& other);

	Vector2d operator+(const Vector2d& other);
	Vector2f operator+(const Vector2f& other);
	Vector2i operator+(const Vector2i& other);
	Vector2d operator-(const Vector2d& other);
	Vector2f operator-(const Vector2f& other);
	Vector2i operator-(const Vector2i& other);
	Vector2i operator-();
	Vector2i operator*(const int& other);
	Vector2f operator*(const float& other);
	Vector2d operator*(const double& other);
	Vector2i operator/(const int& other);
	Vector2f operator/(const float& other);
	Vector2d operator/(const double& other);
	Vector2i& operator+=(const Vector2i& other);
	Vector2i& operator-=(const Vector2i& other);

	float		abs();
	std::string str();
};

Vector2i operator*(const int& lhs, const Vector2i& rhs);
Vector2f operator*(const float& lhs, const Vector2i& rhs);
Vector2d operator*(const double& lhs, const Vector2i& rhs);

Vector2f operator*(const int& lhs, const Vector2f& rhs);
Vector2f operator*(const float& lhs, const Vector2f& rhs);
Vector2d operator*(const double& lhs, const Vector2f& rhs);

Vector2d operator*(const int& lhs, const Vector2d& rhs);
Vector2d operator*(const float& lhs, const Vector2d& rhs);
Vector2d operator*(const double& lhs, const Vector2d& rhs);