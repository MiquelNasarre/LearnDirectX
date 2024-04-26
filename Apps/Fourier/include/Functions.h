#pragma once

#include "Math/Vectors.h"
#include "Math/constants.h"

class Functions
{
	struct Triangle
	{
		Vector3f V1;
		Vector3f V2;
		Vector3f V3;

		Triangle(Vector3f V1, Vector3f V2, Vector3f V3): V1 { V1 }, V2{ V2 }, V3{ V3 }{}
		Triangle(Vector3f* V): V1 { V[0] }, V2{ V[1] }, V3{ V[2] } {}
	};

public:
	static float Yfunc(int l, int m, float phi, float theta);
	static float Psifunc(int l, int m, Vector3f Center);
	static float Legendre(int l, int m, float x);
	static Vector2f LegendreDif(int l, int m, float x);

	static float Tchev(int n, float x);
	static float Uchev(int n, float x);

	static int Factorial(int n);
	static float DFactorial(int n);
	static float DivFactorial(int n, int m);
	static float sqDivFactorial(int n, int m);
	static float efficientPow(float b, int e);

	static Vector3f getCenter(Triangle T);
	static float getArea(Triangle T);
};
