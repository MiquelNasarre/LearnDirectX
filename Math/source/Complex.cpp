#include "Complex.h"
#include "constants.h"
#include <string>

Complex::Complex(float x, float y, bool type)
{
	if (type == Rectangular) {
		a = x;
		b = y;
	}
	else {
		a = x * cos(y);
		b = x * sin(y);
	}
}

Complex::Complex(int x, int y)
{
	a = float(x);
	b = float(y);
}

Complex Complex::operator+(const Complex& other)
{
	return Complex(a + other.a, b + other.b);
}

Complex Complex::operator-(const Complex& other)
{
	return Complex(a - other.a, b - other.b);
}

Complex Complex::operator*(const Complex& other)
{
	return Complex(a * other.a - b * other.b, a * other.b + b * other.a);
}

Complex Complex::operator/(const Complex& other)
{
	Complex inverse(other.a / (other.a * other.a + other.b * other.b), -other.b / (other.a * other.a + other.b * other.b));
	return Complex(a * inverse.a - b * inverse.b, a * inverse.b + b * inverse.a);
}

Complex Complex::operator-()
{
	return Complex(-a, -b);
}

Complex Complex::operator*(const int& other)
{
	return Complex(float(other) * a, float(other) * b);
}

Complex Complex::operator*(const float& other)
{
	return Complex(other * a, other * b);
}

Complex Complex::operator*(const double& other)
{
	return Complex(float(other) * a, float(other) * b);
}

Complex Complex::operator+(const int& other)
{
	return Complex((float)other + a, b);
}

Complex Complex::operator+(const float& other)
{
	return Complex(other + a, b);
}

Complex Complex::operator+(const double& other)
{
	return Complex(float(other) + a, b);
}

Complex& Complex::operator+=(const Complex& other)
{
	a += other.a;
	b += other.b;
	return *this;
}

Complex Complex::exp(const Complex& exponent)
{
	float r = (float)pow(e, exponent.a);
	return Complex(r * cos(exponent.b), r * sin(exponent.b));
}

float Complex::abs()
{
	return sqrt(a * a + b * b);
}

std::string Complex::str()
{
	std::string String;
		String = std::to_string(a);
	if (b >= 0)
		String = String + " + " + std::to_string(b) + " i";
	else if (b < 0)
		String = String + " - " + std::to_string(-b) + " i";
	return String;
}

Complex operator*(const int& lhs, const Complex& rhs)
{
	return Complex(lhs * rhs.a, lhs * rhs.b);
}

Complex operator*(const float& lhs, const Complex& rhs)
{
	return Complex(lhs * rhs.a, lhs * rhs.b);
}

Complex operator*(const double& lhs, const Complex& rhs)
{
	return float(lhs) * rhs;
}

Complex operator+(const int& lhs, const Complex& rhs)
{
	return Complex(float(lhs) + rhs.a, rhs.b);
}

Complex operator+(const float& lhs, const Complex& rhs)
{
	return Complex(lhs + rhs.a, rhs.b);
}

Complex operator+(const double& lhs, const Complex& rhs)
{
	return Complex(float(lhs) + rhs.a, rhs.b);
}

Complex operator/(const int& lhs, const Complex& rhs)
{
	return Complex(lhs) / rhs;
}

Complex operator/(const float& lhs, const Complex& rhs)
{
	return Complex(lhs) / rhs;
}

Complex operator/(const double& lhs, const Complex& rhs)
{
	return Complex(float(lhs)) / rhs;
}

Complex operator-(const int& lhs, const Complex& rhs)
{
	return Complex(float(lhs) - rhs.a, - rhs.b);
}

Complex operator-(const float& lhs, const Complex& rhs)
{
	return Complex(lhs - rhs.a, -rhs.b);
}

Complex operator-(const double& lhs, const Complex& rhs)
{
	return float(lhs) - rhs;
}
