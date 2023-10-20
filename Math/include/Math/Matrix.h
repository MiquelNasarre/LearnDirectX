#pragma once

#ifdef MATH_BUILDER_
#include "Vectors.h"
#endif

struct _float4matrix {
	float indexes[16];
};

class Matrix {
private:
	void switchRows(unsigned int r0, unsigned int r1);

public:
	Vector3f row[3];

	Matrix();
	Matrix(float i);
	Matrix(const Vector3f& row0,const Vector3f& row1,const Vector3f& row2);
	Matrix(const Vector3f* pRows);
	Matrix(const float* values);
	Matrix(float x00, float x01, float x02, float x10, float x11, float x12, float x20, float x21, float x22);

	Matrix(const Matrix& other) = default;
	~Matrix() = default;

	Matrix operator+(const Matrix& other) const;
	Matrix operator-(const Matrix& other) const;
	Matrix operator*(const Matrix& other) const;
	Matrix operator-() const;
	Vector3f operator*(const Vector3f& other) const;
	Matrix operator*(const float& other) const;

	Vector3f column(unsigned int n) const;
	float determinant() const;
	Matrix inverse() const;
	Matrix transposed() const;

	Matrix& invert();
	Matrix& transpose();

	static const Matrix Identity;

	_float4matrix getMatrix4() const;

};

Matrix ProjectionMatrix(Vector3f V);
Matrix XRotationMatrix(float angle);
Matrix YRotationMatrix(float angle);
Matrix ZRotationMatrix(float angle);
Matrix ScalingMatrix(float x, float y, float z);

Matrix operator*(const float& x, const Matrix& M);
Vector3f operator*(const Vector3f& V, const Matrix& M);