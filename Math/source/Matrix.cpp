#include "Matrix.h"
#include <math.h>

const Matrix Matrix::Identity = Matrix(1);

void Matrix::switchRows(unsigned int r0, unsigned int r1)
{
	Vector3f temp = row[0];
	row[0] = row[1];
	row[1] = temp;
}

Matrix::Matrix()
{
	row[0] = Vector3f(0.f, 0.f, 0.f);
	row[1] = Vector3f(0.f, 0.f, 0.f);
	row[2] = Vector3f(0.f, 0.f, 0.f);
}

Matrix::Matrix(float i)
{
	row[0] = { i,0.f,0.f };
	row[1] = { 0.f,i,0.f };
	row[2] = { 0.f,0.f,i };
}

Matrix::Matrix(const Vector3f& row0, const Vector3f& row1, const Vector3f& row2)
{
	row[0] = row0;
	row[1] = row1;
	row[2] = row2;
}

Matrix::Matrix(const Vector3f* pRows)
{
	row[0] = pRows[0];
	row[1] = pRows[1];
	row[2] = pRows[2];
}

Matrix::Matrix(const float* values)
{
	row[0] = { values[0],values[1] ,values[2] };
	row[1] = { values[3],values[4] ,values[5] };
	row[2] = { values[6],values[7] ,values[8] };
}

Matrix::Matrix(float x00, float x01, float x02, float x10, float x11, float x12, float x20, float x21, float x22)
{
	row[0] = { x00,x01 ,x02 };
	row[1] = { x10,x11 ,x12 };
	row[2] = { x20,x21 ,x22 };
}

Matrix Matrix::operator+(const Matrix& other) const
{
	return Matrix(row[0] + other.row[0], row[1] + other.row[1], row[2] + other.row[2]);
}

Matrix Matrix::operator-(const Matrix& other) const
{
	return Matrix(row[0] - other.row[0], row[1] - other.row[1], row[2] - other.row[2]);
}

Matrix Matrix::operator*(const Matrix& other) const
{
	return Matrix(
		row[0] ^ other.column(0), row[0] ^ other.column(1), row[0] ^ other.column(2),
		row[1] ^ other.column(0), row[1] ^ other.column(1), row[1] ^ other.column(2),
		row[2] ^ other.column(0), row[2] ^ other.column(1), row[2] ^ other.column(2)
	);
}

Matrix Matrix::operator-() const
{
	return Matrix(-row[0], -row[1], -row[2]);
}

Vector3f Matrix::operator*(const Vector3f& other) const
{
	return Vector3f(row[0] ^ other, row[1] ^ other, row[2] ^ other);
}

Matrix Matrix::operator*(const float& other) const
{
	return Matrix(other * row[0], other * row[1], other * row[2]);
}

Vector3f Matrix::column(unsigned int n) const
{
	if (n == 0u)
		return Vector3f(row[0].x, row[1].x, row[2].x);
	if (n == 1u)
		return Vector3f(row[0].y, row[1].y, row[2].y);
	if (n == 2u)
		return Vector3f(row[0].z, row[1].z, row[2].z);
	return Vector3f();
}

float Matrix::determinant() const
{
	return
		row[0].x * (row[1].y * row[2].z - row[1].z * row[2].y) -
		row[1].x * (row[0].y * row[2].z - row[0].z * row[2].y) + 
		row[2].x * (row[0].y * row[1].z - row[0].z * row[1].y);
}

Matrix Matrix::inverse() const
{
	float det = determinant();
	if (det == 0)
		return Matrix();
	
	Matrix copy = *this;
	Matrix Inverse = Identity;
	
	if (copy.row[0].x == 0.f) {
		copy.switchRows(0, 1);
		Inverse.switchRows(0, 1);
	}

	if (copy.row[0].x == 0.f) {
		copy.switchRows(0, 2);
		Inverse.switchRows(0, 2);
	}


	Inverse.row[0] /= copy.row[0].x;
	copy.row[0] /= copy.row[0].x;

	Inverse.row[1] -= Inverse.row[0] * copy.row[1].x;
	copy.row[1] -= copy.row[0] * copy.row[1].x;

	Inverse.row[2] -= Inverse.row[0] * copy.row[2].x;
	copy.row[2] -= copy.row[0] * copy.row[2].x;

	if (copy.row[1].y == 0.f) {
		copy.switchRows(1, 2);
		Inverse.switchRows(1, 2);
	}

	Inverse.row[1] /= copy.row[1].y;
	copy.row[1] /= copy.row[1].y;

	Inverse.row[2] -= Inverse.row[1] * copy.row[2].y;
	copy.row[2] -= copy.row[1] * copy.row[2].y;

	Inverse.row[2] /= copy.row[2].z;
	//copy.row[2] /= copy.row[2].z;

	// Matrix already diagonalized, now lets correct top right indexes

	Inverse.row[1] -= Inverse.row[2] * copy.row[1].z;
	//copy.row[1] -= copy.row[2] * copy.row[1].z;

	Inverse.row[0] -= Inverse.row[2] * copy.row[0].z;
	//copy.row[0] -= copy.row[2] * copy.row[0].z;

	Inverse.row[0] -= Inverse.row[1] * copy.row[0].y;
	//copy.row[0] -= copy.row[1] * copy.row[0].y;

	return Inverse;
}

Matrix Matrix::transposed() const
{
	return Matrix(column(0),column(1),column(2));
}

Matrix& Matrix::invert()
{
	*this = inverse();
	return *this;
}

Matrix& Matrix::transpose()
{
	*this = transposed();
	return *this;
}

_float4matrix Matrix::getMatrix4() const
{
	return _float4matrix({
		row[0].x, row[0].y ,row[0].z, 0.f,
		row[1].x, row[1].y ,row[1].z, 0.f,
		row[2].x, row[2].y ,row[2].z, 0.f,
		0.f,      0.f,      0.f,      0.f,
		});
}

//	Outside class functions

Matrix ProjectionMatrix(Vector3f V)
{
	if (!V.x && !V.y) {
		if (V.z > 0)
			return Matrix::Identity;
		else return ScalingMatrix(1.f,-1.f,-1.f);
	}

	Vector2f V2 = { V.x,V.y };
	float absXY = V2.abs();
	V2 /= absXY;
	Matrix rotationZ = {
		V2.y, V2.x, 0.f,
		-V2.x,V2.y, 0.f,
		0.f ,  0.f, 1.f,
	};
	float absV = V.abs();
	V /= absV;
	Matrix rotationX = {
		1.f, 0.f, 0.f,
		0.f,-V.z,absXY/absV,
		0.f,absXY/absV,V.z,
	};
	return rotationZ * rotationX;
}

Matrix XRotationMatrix(float angle)
{
	return Matrix(
			1.f,          0.f,         0.f,
			0.f, cosf(angle), -sinf(angle),
			0.f, sinf(angle),  cosf(angle)
		);
}

Matrix YRotationMatrix(float angle)
{
	return Matrix(
			 cosf(angle), 0.f, sinf(angle),
			0.f,          1.f,         0.f,
			-sinf(angle), 0.f, cosf(angle)
		);
}

Matrix ZRotationMatrix(float angle)
{
	return Matrix(
			cosf(angle), -sinf(angle), 0.f,
			sinf(angle),  cosf(angle), 0.f, 
			0.f,          0.f,         1.f
		);
}

Matrix ScalingMatrix(float x, float y, float z)
{
	return Matrix({
			x,0.f,0.f,
			0.f,y,0.f,
			0.f,0.f,z
		});
}

Matrix operator*(const float& x, const Matrix& M)
{
	return M * x;
}

Vector3f operator*(const Vector3f& V, const Matrix& M)
{
	return M.transposed() * V;
}
