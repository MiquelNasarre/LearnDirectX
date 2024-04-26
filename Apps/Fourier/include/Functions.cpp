#include "Functions.h"

float Functions::Yfunc(int l, int m, float phi, float theta)
{
	float NonTheta;

	if (m > 0)
		NonTheta = sqrtf(2 * (2 * l + 1) / float(DivFactorial(l + m, l - m))) * cosf(m * phi);
	else if (m == 0)
		NonTheta = sqrtf(float(2 * l + 1));
	else {
		m = -m;
		NonTheta = sqrtf(2 * (2 * l + 1) / float(DivFactorial(l + m, l - m))) * sinf(m * phi);
	}

	return NonTheta * Legendre(l, m, cosf(theta));
}

float Functions::Psifunc(int l, int m, Vector3f Center)
{
	return 0.0f;
}

float Functions::Legendre(int l, int m, float x)
{
	float sq = sqrtf(1 - x * x);
	int lc = m;


	float Plm = DFactorial(2 * m - 1);
	
	for (unsigned short i = 0; i < m; i++)
		Plm *= -sq;

	if (l == m)
		return Plm;
	
	float Pl1m = x * (2 * lc + 1) * Plm;
	lc++;
	float aux;

	while (lc < l) {
		aux = ((2 * lc + 1) * x * Pl1m - (lc + m) * Plm) / (lc - m + 1);
		lc++;
		Plm = Pl1m;
		Pl1m = aux;
	}

	return Pl1m;
}

Vector2f Functions::LegendreDif(int l, int m, float x)
{
	Vector2f Plm;

	if (l == 0)
	{
		Plm.x = 1;
		Plm.y = 0;
		return Plm;
	}

	float x12 = 1 - x * x;
	float sq = sqrtf(x12);
	int lc = m;


	Plm.x = DFactorial(2 * m - 1);

	for (unsigned short i = 0; i < m; i++)
		Plm.x *= -sq;

	if (l == m)
	{
		Plm.y = -x * l / x12 * Plm.x;
		return Plm;
	}


	float aux = x * (2 * lc + 1) * Plm.x;
	float Pl1m = Plm.x;
	Plm.x = aux;
	lc++;

	while (lc < l) {
		aux = ((2 * lc + 1) * x * Plm.x - (lc + m) * Pl1m) / (lc - m + 1);
		lc++;
		Pl1m = Plm.x;
		Plm.x = aux;
	}

	Plm.y = ((l + m) * Pl1m - l * x * Plm.x) / x12;
	return Plm;
}

float Functions::Tchev(int n, float x)
{
	if (!n)
		return 1.f;
	if (n == 1)
		return x;

	float Ti1 = 1.f;
	float Ti = x;

	int m = 1;
	float temp;
	while (m++ < n)
	{
		temp = 2 * x * Ti - Ti1;
		Ti1 = Ti;
		Ti = temp;
	}
	return Ti;
}

float Functions::Uchev(int n, float x)
{
	if (!n)
		return 1.f;
	if (n == 1)
		return 2 * x;

	float Ui1 = 1.f;
	float Ui = 2 * x;

	int m = 1;
	float temp;
	while (m++ < n)
	{
		temp = 2 * x * Ui - Ui1;
		Ui1 = Ui;
		Ui = temp;
	}
	return Ui;
}

int Functions::Factorial(int n)
{
	if (!n) return 1;
	int f = n;
	while (--n)
		f *= n;
	return f;
}

float Functions::DFactorial(int n)
{
	switch (n)
	{
	case 1:
		return 1.f;
	case 3:
		return 3.f;
	case 5:
		return 15.f;
	case 7:
		return 105.f;
	case 9:
		return 945.f;
	case 11:
		return 10395.f;
	case 13:
		return 135135.f;
	case 15:
		return 2027025.f;
	case 17:
		return 34459425.f;
	case 19:
		return 654729075.f;
	case 21:
		return 13749310575.f;

	default:
		break;
	}

	if (n < 1) return 1;
	float f = float(n);
	while (--n > 1) {
		--n;
		f *= n;
	}
	return f;
}

float Functions::DivFactorial(int n, int m)
{
	if (!n || n <= m) return 1;
	float f = float(n);

	while (--n > m)
		f *= n;

	return f;
}

float Functions::sqDivFactorial(int n, int m)
{
	if (!n || n <= m) return 1;
	float f = sqrtf(float(n));

	while (--n > m)
		f *= sqrtf(float(n));

	return f;
}

float Functions::efficientPow(float b, int e)
{
	float r = 1;
	for (unsigned short i = 0; i < e; i++)
		r *= b;
	return r;
}

Vector3f Functions::getCenter(Triangle T)
{
	return (T.V1 + T.V2 + T.V3) / 3.f;
}

float Functions::getArea(Triangle T)
{
	Vector3f V12 = (T.V1 * T.V2 * T.V1).normalize();
	Vector3f V13 = (T.V1 * T.V3 * T.V1).normalize();

	float angle1 = acosf(V13 ^ V12);

	Vector3f V21 = (T.V2 * T.V1 * T.V2).normalize();
	Vector3f V23 = (T.V2 * T.V3 * T.V2).normalize();

	float angle2 = acosf(V23 ^ V21);

	Vector3f V32 = (T.V3 * T.V2 * T.V3).normalize();
	Vector3f V31 = (T.V3 * T.V1 * T.V3).normalize();

	float angle3 = acosf(V31 ^ V32);

	return angle1 + angle2 + angle3 - pi;
}
