#pragma once
#include "Header.h"

enum ANGLE_TYPE
{
	RADIANS,
	DEGREES
};

class angle1;
class angle2;
class angle3;

class angle {
public:
	virtual double getRaw(ANGLE_TYPE) const = 0;
	double getMiles() const;
};

class angle1 : public angle
{
public:
	double angleDeg;

	angle1() = default;
	angle1(double angle, ANGLE_TYPE type);
	angle2 getAngle2() const;
	angle3 getAngle3() const;

	double getRaw(ANGLE_TYPE type) const override;
	std::string getString() const;

};

class angle2 : public angle
{
public:
	int angleDeg;
	double angleMin;

	angle2() = default;
	angle2(double angle, ANGLE_TYPE type);
	angle1 getAngle1() const;
	angle3 getAngle3() const;

	double getRaw(ANGLE_TYPE type) const override;
	std::string getString() const;
};

class angle3 : public angle
{
public:
	int angleDeg;
	int angleMin;
	double angleSec;

	angle3() = default;
	angle3(double angle, ANGLE_TYPE type);
	angle1 getAngle1() const;
	angle2 getAngle2() const;

	double getRaw(ANGLE_TYPE type) const override;
	std::string getString() const;
};

class Coordinate
{
public:
	angle1 latitude;
	angle1 Longitude;

	Coordinate();
	Coordinate(double lat, double Long, ANGLE_TYPE type);
	Coordinate(angle& latitude, angle& Longitude);

	std::string getString(UINT D = 3) const;
	void correct();
	Vector3d getVector() const;

	struct PLACES{
		static const Coordinate Greenwich;
		static const Coordinate London;
		static const Coordinate Barcelona;
		static const Coordinate NewYork;
		static const Coordinate Canary;
		static const Coordinate Grenada;
		static const Coordinate Gibraltar;
		static const Coordinate Auckland;
	};

};

double cos(const angle& angle);
double sin(const angle& angle);
double tan(const angle& angle);
float cosf(const angle& angle);
float sinf(const angle& angle);
float tanf(const angle& angle);

double getDistance(const Coordinate& Pos1, const Coordinate& Pos2);
double getDirection(const Coordinate& Pos1, const Coordinate& Pos2);