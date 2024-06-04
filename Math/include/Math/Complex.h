#pragma once

#define _COMPLEX_ADDED

struct Complex {
	static const bool Rectangular	= true;
	static const bool Polar			= false;

	float a, b;

	Complex() {};
	Complex(float x, float y = 0.f, bool type = Rectangular);
	Complex(int x, int y = 0);

	Complex operator+(const Complex& other) const;
	Complex operator-(const Complex& other) const;
	Complex operator*(const Complex& other) const;
	Complex operator/(const Complex& other) const;
	Complex operator-() const;
	Complex operator*(const int& other) const;
	Complex operator*(const float& other) const;
	Complex operator*(const double& other) const;
	Complex operator+(const int& other) const;
	Complex operator+(const float& other) const;
	Complex operator+(const double& other) const;
	Complex& operator+=(const Complex& other);

	static Complex exp(const Complex& exponent);

	float		abs() const;
	const char* str() const;
};

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