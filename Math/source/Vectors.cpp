#include "Vectors.h"

#include <string>
#include <math.h>

//	Vector2d

Vector2d::Vector2d()
{
	x = 0.;
	y = 0.;
}

Vector2d::Vector2d(double x, double y)
{
	this->x = x;
	this->y = y;
}

Vector2d::Vector2d(float x, float y)
{
	this->x = (double)x;
	this->y = (double)y;
}

Vector2d::Vector2d(int x, int y)
{
	this->x = (double)x;
	this->y = (double)y;
}

Vector2d::Vector2d(const Vector2i& other)
{
	x = (double)other.x;
	y = (double)other.y;
}

Vector2d::Vector2d(const Vector2f& other)
{
	x = (double)other.x;
	y = (double)other.y;
}

Vector2d Vector2d::operator+(const Vector2d& other)
{
	return Vector2d(x + other.x, y + other.y);
}

Vector2d Vector2d::operator+(const Vector2f& other)
{
	return *this + (Vector2d)other;
}

Vector2d Vector2d::operator+(const Vector2i& other)
{
	return *this + (Vector2d)other;
}

Vector2d Vector2d::operator-(const Vector2d& other)
{
	return Vector2d(x - other.x, y - other.y);
}

Vector2d Vector2d::operator-(const Vector2f& other)
{
	return *this - (Vector2d)other;
}

Vector2d Vector2d::operator-(const Vector2i& other)
{
	return *this - (Vector2d)other;
}

Vector2d Vector2d::operator-()
{
	return Vector2d(-x, -y);
}

Vector2d Vector2d::operator*(const int& other)
{
	return *this * (double)other;
}

Vector2d Vector2d::operator*(const float& other)
{
	return *this * (double)other;
}

Vector2d Vector2d::operator*(const double& other)
{
	return Vector2d(other * x, other * y);
}

Vector2d Vector2d::operator/(const int& other)
{
	return *this / (double)other;
}

Vector2d Vector2d::operator/(const float& other)
{
	return *this / (double)other;
}

Vector2d Vector2d::operator/(const double& other)
{
	return Vector2d(x / other, y / other);
}

Vector2d& Vector2d::operator+=(const Vector2d& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2d& Vector2d::operator+=(const Vector2f& other)
{
	x += (double)other.x;
	y += (double)other.y;
	return *this;
}

Vector2d& Vector2d::operator+=(const Vector2i& other)
{
	x += (double)other.x;
	y += (double)other.y;
	return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2d& Vector2d::operator-=(const Vector2f& other)
{
	x -= (double)other.x;
	y -= (double)other.y;
	return *this;
}

Vector2d& Vector2d::operator-=(const Vector2i& other)
{
	x -= (double)other.x;
	y -= (double)other.y;
	return *this;
}

double Vector2d::abs()
{
	return sqrt(x * x + y * y);
}

std::string Vector2d::str()
{
	return "( " + std::to_string(x) + " , " + std::to_string(y) + " )";
}

//	Vector2f

Vector2f::Vector2f()
{
	x = 0.f;
	y = 0.f;
}

Vector2f::Vector2f(double x, double y)
{
	this->x = (float)x;
	this->y = (float)y;
}

Vector2f::Vector2f(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2f::Vector2f(int x, int y)
{
	this->x = (float)x;
	this->y = (float)y;
}

Vector2f::Vector2f(const Vector2i& other)
{
	x = (float)other.x;
	y = (float)other.y;
}

Vector2f::Vector2f(const Vector2d& other)
{
	x = (float)other.x;
	y = (float)other.y;
}

Vector2d Vector2f::operator+(const Vector2d& other)
{
	return (Vector2d)*this + other;
}

Vector2f Vector2f::operator+(const Vector2f& other)
{
	return Vector2f(x + other.x, y + other.y);
}

Vector2f Vector2f::operator+(const Vector2i& other)
{
	return *this + (Vector2f)other;
}

Vector2d Vector2f::operator-(const Vector2d& other)
{
	return (Vector2d)*this - other;
}

Vector2f Vector2f::operator-(const Vector2f& other)
{
	return Vector2f(x - other.x, y - other.y);
}

Vector2f Vector2f::operator-(const Vector2i& other)
{
	return *this - (Vector2f)other;
}

Vector2f Vector2f::operator-()
{
	return Vector2f(-x, -y);
}

Vector2f Vector2f::operator*(const int& other)
{
	return *this * (float)other;
}

Vector2f Vector2f::operator*(const float& other)
{
	return Vector2f(x * other, y * other);
}

Vector2d Vector2f::operator*(const double& other)
{
	return *this * (float)other;
}

Vector2f Vector2f::operator/(const int& other)
{
	return *this / (float)other;
}

Vector2f Vector2f::operator/(const float& other)
{
	return Vector2f(x / other, y / other);
}

Vector2d Vector2f::operator/(const double& other)
{
	return *this / (float)other;
}

Vector2f& Vector2f::operator+=(const Vector2f& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2f& Vector2f::operator+=(const Vector2i& other)
{
	x += (float)other.x;
	y += (float)other.y;
	return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2f& Vector2f::operator-=(const Vector2i& other)
{
	x -= (float)other.x;
	y -= (float)other.y;
	return *this;
}

float Vector2f::abs()
{
	return sqrtf(x * x + y * y);
}

std::string Vector2f::str()
{
	return "( " + std::to_string(x) + " , " + std::to_string(y) + " )";
}

//	Vector2i

Vector2i::Vector2i()
{
	x = 0;
	y = 0;
}

Vector2i::Vector2i(double x, double y)
{
	this->x = (int)x;
	this->y = (int)y;
}

Vector2i::Vector2i(float x, float y)
{
	this->x = (int)x;
	this->y = (int)y;
}

Vector2i::Vector2i(int x, int y)
{
	this->x = x;
	this->y = y;
}

Vector2i::Vector2i(const Vector2f& other)
{
	x = (int)other.x;
	y = (int)other.y;
}

Vector2i::Vector2i(const Vector2d& other)
{
	x = (int)other.x;
	y = (int)other.y;
}

Vector2d Vector2i::operator+(const Vector2d& other)
{
	return (Vector2d)*this + other;
}

Vector2f Vector2i::operator+(const Vector2f& other)
{
	return (Vector2f)*this + other;
}

Vector2i Vector2i::operator+(const Vector2i& other)
{
	return Vector2i(x + other.x, y + other.y);
}

Vector2d Vector2i::operator-(const Vector2d& other)
{
	return (Vector2d)*this - other;
}

Vector2f Vector2i::operator-(const Vector2f& other)
{
	return (Vector2f)*this - other;
}

Vector2i Vector2i::operator-(const Vector2i& other)
{
	return Vector2i(x - other.x, y - other.y);
}

Vector2i Vector2i::operator-()
{
	return Vector2i(-x, -y);
}

Vector2i Vector2i::operator*(const int& other)
{
	return Vector2i(x * other, y * other);
}

Vector2f Vector2i::operator*(const float& other)
{
	return (Vector2f)*this * other;
}

Vector2d Vector2i::operator*(const double& other)
{
	return (Vector2d)*this * other;
}

Vector2i Vector2i::operator/(const int& other)
{
	return Vector2i(x / other, y / other);
}

Vector2f Vector2i::operator/(const float& other)
{
	return (Vector2f)*this / other;
}

Vector2d Vector2i::operator/(const double& other)
{
	return (Vector2d)*this / other;
}

Vector2i& Vector2i::operator+=(const Vector2i& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2i& Vector2i::operator-=(const Vector2i& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

float Vector2i::abs()
{
	return sqrtf(float(x * x + y * y));
}

std::string Vector2i::str()
{
	return "( " + std::to_string(x) + " , " + std::to_string(y) + " )";
}

//	Rest of operators

Vector2i operator*(const int& lhs, const Vector2i& rhs)
{
	return Vector2i(lhs * rhs.x, lhs * rhs.y);
}

Vector2f operator*(const float& lhs, const Vector2i& rhs)
{
	return Vector2f(lhs * (float)rhs.x, lhs * (float)rhs.y);
}

Vector2d operator*(const double& lhs, const Vector2i& rhs)
{
	return Vector2d(lhs * (double)rhs.x, lhs * (double)rhs.y);
}

Vector2f operator*(const int& lhs, const Vector2f& rhs)
{
	return Vector2f((float)lhs * rhs.x, (float)lhs * rhs.y);
}

Vector2f operator*(const float& lhs, const Vector2f& rhs)
{
	return Vector2f(lhs * rhs.x, lhs * rhs.y);
}

Vector2d operator*(const double& lhs, const Vector2f& rhs)
{
	return Vector2d(lhs * (double)rhs.x, lhs * (double)rhs.y);
}

Vector2d operator*(const int& lhs, const Vector2d& rhs)
{
	return Vector2d((double)lhs * rhs.x, (double)lhs * rhs.y);
}

Vector2d operator*(const float& lhs, const Vector2d& rhs)
{
	return Vector2d((double)lhs * rhs.x, (double)lhs * rhs.y);
}

Vector2d operator*(const double& lhs, const Vector2d& rhs)
{
	return Vector2d(lhs * rhs.x, lhs * rhs.y);
}
