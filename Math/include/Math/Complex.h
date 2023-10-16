#pragma once

#include <iostream>

#define Pi 3.14159265358979f
#define e  2.71828182845905f

struct Complex {
	static const bool Rectangular	= true;
	static const bool Polar			= false;

	float a, b;

	Complex() {};
	Complex(float x, float y = 0.f, bool type = Rectangular);
	Complex(int x, int y = 0);

	Complex operator+(const Complex& other);
	Complex operator-(const Complex& other);
	Complex operator*(const Complex& other);
	Complex operator/(const Complex& other);
	Complex operator-();
	Complex operator*(const int& other);
	Complex operator*(const float& other);
	Complex operator*(const double& other);
	Complex operator+(const int& other);
	Complex operator+(const float& other);
	Complex operator+(const double& other);
	Complex& operator+=(const Complex& other);

	static Complex exp(const Complex& exponent);

	float		abs();
	std::string str();
};

#define I Complex(0.f,1.f)

Complex operator*(const int& lhs, const Complex& rhs);
Complex operator*(const float& lhs, const Complex& rhs);
Complex operator*(const double& lhs, const Complex& rhs);
Complex operator+(const int& lhs, const Complex& rhs);
Complex operator+(const float& lhs, const Complex& rhs);
Complex operator+(const double& lhs, const Complex& rhs);
Complex operator/(const int& lhs, const Complex& rhs);
Complex operator/(const float& lhs, const Complex& rhs);
Complex operator/(const double& lhs, const Complex& rhs);
Complex operator-(const int& lhs, const Complex& rhs);
Complex operator-(const float& lhs, const Complex& rhs);
Complex operator-(const double& lhs, const Complex& rhs);