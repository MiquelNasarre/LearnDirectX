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

	angle3(double angle, ANGLE_TYPE type);
	angle1 getAngle1() const;
	angle2 getAngle2() const;

	double getRaw(ANGLE_TYPE type) const override;
	std::string getString() const;
};

class Coordinates
{
public:
	angle1 latitude;
	angle1 Longitude;

	Coordinates(double lat, double Long, ANGLE_TYPE type);
	Coordinates(angle& latitude, angle& Longitude);

	std::string getString(UINT D = 3) const;
	void correct();
	Vector3d getVector() const;

	static const Coordinates Greenwich;
	static const Coordinates London;
	static const Coordinates Barcelona;
	static const Coordinates NewYork;
	static const Coordinates Canary;
	static const Coordinates Grenada;
	static const Coordinates Gibraltar;

};

double cos(const angle& angle);
double sin(const angle& angle);
double tan(const angle& angle);

double getDistance(const Coordinates& Pos1, const Coordinates& Pos2);
double getDirection(const Coordinates& Pos1, const Coordinates& Pos2);