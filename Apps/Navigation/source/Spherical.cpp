#include "Spherical.h"
#include <string>

//	Angle

double angle::getMiles() const
{
	return getRaw(DEGREES) * 60;
}

//	angle1

angle1::angle1(double angle, ANGLE_TYPE type)
{
	switch (type)
	{
	case RADIANS:
		angleDeg = angle * 360 / 2 / pi;
		break;
	case DEGREES:
		angleDeg = angle;
		break;
	}
	while (angleDeg >= 360.)
		angleDeg -= 360.;
	while (angleDeg <= 0.)
		angleDeg += 360.;
}

angle2 angle1::getAngle2() const
{
	return angle2(getRaw(DEGREES), DEGREES);
}

angle3 angle1::getAngle3() const
{
	return angle3(getRaw(DEGREES), DEGREES);
}

double angle1::getRaw(ANGLE_TYPE type) const
{
	switch (type)
	{
	case RADIANS:
		return angleDeg * 2 * pi / 360;
	case DEGREES:
		return angleDeg;
	default:
		return 0.;
	}
}

std::string angle1::getString() const
{
	return std::to_string(angleDeg) + char(248);
}

//	angle2

angle2::angle2(double angle, ANGLE_TYPE type)
{
	double temp;
	switch (type)
	{
	case RADIANS:
		temp = angle * 360 / 2 / pi;
		break;
	case DEGREES:
		temp = angle;
		break;
	}
	while (temp >= 360.)
		temp -= 360.;
	while (temp <= 0.)
		temp += 360.;

	angleDeg = int(temp);
	angleMin = (temp - angleDeg) * 60;
}

angle1 angle2::getAngle1() const
{
	return angle1(getRaw(DEGREES), DEGREES);
}

angle3 angle2::getAngle3() const
{
	return angle3(getRaw(DEGREES), DEGREES);
}

double angle2::getRaw(ANGLE_TYPE type) const
{
	double temp = angleDeg + angleMin / 60.;
	switch (type)
	{
	case RADIANS:
		return temp * 2 * pi / 360.;
	case DEGREES:
		return temp;
	default:
		return 0.;
	}
}

std::string angle2::getString() const
{
	return std::to_string(angleDeg) + char(248) + ' ' + std::to_string(angleMin) + '\'';
}

//	angle3

angle3::angle3(double angle, ANGLE_TYPE type)
{
	double temp;
	switch (type)
	{
	case RADIANS:
		temp = angle * 360 / 2 / pi;
		break;
	case DEGREES:
		temp = angle;
		break;
	}
	while (temp >= 360.)
		temp -= 360.;
	while (temp <= 0.)
		temp += 360.;

	angleDeg = int(temp);
	angleMin = int((temp - angleDeg) * 60);
	angleSec = ((temp - angleDeg) * 60 - angleMin) * 60;
}

angle1 angle3::getAngle1() const
{
	return angle1(getRaw(DEGREES), DEGREES);
}

angle2 angle3::getAngle2() const
{
	return angle2(getRaw(DEGREES), DEGREES);
}

double angle3::getRaw(ANGLE_TYPE type) const
{
	double temp = angleDeg + angleMin / 60. + angleSec / 3600.;
	switch (type)
	{
	case RADIANS:
		return temp * 2 * pi / 360.;
	case DEGREES:
		return temp;
	default:
		return 0.;
	}
}

std::string angle3::getString() const
{
	return std::to_string(angleDeg) + char(248) + ' ' + std::to_string(angleMin) + '\'' + ' ' + std::to_string(angleSec) + '\"';
}

//	Coordinates

const Coordinate Coordinate::PLACES::Greenwich	(51.4780	, 0.0014	, DEGREES);
const Coordinate Coordinate::PLACES::London		(51.5098	, -0.1180	, DEGREES);
const Coordinate Coordinate::PLACES::Barcelona	(41.3874	, 2.1686	, DEGREES);
const Coordinate Coordinate::PLACES::NewYork	(40.7831	, -73.9712	, DEGREES);
const Coordinate Coordinate::PLACES::Canary		(27.9202	, -15.5474	, DEGREES);
const Coordinate Coordinate::PLACES::Grenada	(12.1165	, -61.6790	, DEGREES);
const Coordinate Coordinate::PLACES::Gibraltar	(36.1408	, -5.3536	, DEGREES);
const Coordinate Coordinate::PLACES::Auckland	(-36.8509	, 174.7645	, DEGREES);

Coordinate::Coordinate()
{
	*this = PLACES::Greenwich;
}

Coordinate::Coordinate(double lat, double Long, ANGLE_TYPE type)
	: latitude(lat,type), Longitude(Long, type)
{
	correct();
}

Coordinate::Coordinate(angle& lat, angle& Long)
	: latitude(lat.getRaw(DEGREES), DEGREES), Longitude(Long.getRaw(DEGREES), DEGREES)
{
	correct();
}

std::string Coordinate::getString(UINT D) const
{
	angle1 lat = latitude;
	angle1 Long = Longitude;
	bool North = true;
	bool East = true;
	if (lat.angleDeg < 0) {
		lat.angleDeg *= -1;
		North = false;
	}
	if (Long.angleDeg < 0) {
		Long.angleDeg *= -1;
		East = false;
	}
	std::string Slat;
	std::string SLong;
	switch (D)
	{
	case 1:
		if (North) Slat = lat.getString() + " N";
		else Slat = lat.getString() + " S";
		if (East)SLong = Long.getString() + " E";
		else SLong = Long.getString() + " W";
		return Slat + '\n' + SLong;
	case 2:
		if (North) Slat = lat.getAngle2().getString() + " N";
		else Slat = lat.getAngle2().getString() + " S";
		if (East)SLong = Long.getAngle2().getString() + " E";
		else SLong = Long.getAngle2().getString() + " W";
		return Slat + '\n' + SLong;
	case 3:
		if (North) Slat = lat.getAngle3().getString() + " N";
		else Slat = lat.getAngle3().getString() + " S";
		if (East)SLong = Long.getAngle3().getString() + " E";
		else SLong = Long.getAngle3().getString() + " W";
		return Slat + '\n' + SLong;
	default:
		return "";
	}
}

void Coordinate::correct()
{
	while (Longitude.angleDeg > 360)
		Longitude.angleDeg -= 360;
	while (latitude.angleDeg > 360)
		latitude.angleDeg -= 360;
	while (Longitude.angleDeg < 0)
		Longitude.angleDeg += 360;
	while (latitude.angleDeg < 0)
		latitude.angleDeg += 360;

	if (latitude.angleDeg > 180)
		latitude.angleDeg = latitude.angleDeg - 360;
	if (latitude.angleDeg > 90) {
		Longitude.angleDeg += 180;
		latitude.angleDeg = 180 - latitude.angleDeg;
	}
	if (latitude.angleDeg < -90) {
		Longitude.angleDeg += 180;
		latitude.angleDeg = - 180 - latitude.angleDeg;
	}
	if (Longitude.angleDeg > 360)
		Longitude.angleDeg -= 360;
	if (Longitude.angleDeg > 180)
		Longitude.angleDeg -= 360;


}

Vector3d Coordinate::getVector() const
{
	return { cos(latitude) * cos(Longitude), cos(latitude) * sin(Longitude), sin(latitude) };
}

//	Functions

double cos(const angle& angle)
{
	return cos(angle.getRaw(RADIANS));
}

double sin(const angle& angle)
{
	return sin(angle.getRaw(RADIANS));
}

double tan(const angle& angle)
{
	return sin(angle) / cos(angle);
}

float cosf(const angle& angle)
{
	return cosf((float)angle.getRaw(RADIANS));
}

float sinf(const angle& angle)
{
	return sinf((float)angle.getRaw(RADIANS));
}

float tanf(const angle& angle)
{
	return sinf(angle) / cosf(angle);
}

double getDistance(const Coordinate& Pos1, const Coordinate& Pos2)
{
	return 360 * 60 / 2 / pi * acos(Pos1.getVector() ^ Pos2.getVector());
}

double getDirection(const Coordinate& Pos1, const Coordinate& Pos2)
{
	Vector3d North = Pos1.getVector() * Vector3d(0, 0, 1) * Pos1.getVector();
	Vector3d Route = Pos1.getVector() * Pos2.getVector() * Pos1.getVector();
	North.normalize();
	Route.normalize();

	if ((Pos1.getVector() * North ^ Route) > 0)
		return 360 / 2 / pi * acos(North ^ Route);
	else
		return 360 - 360 / 2 / pi * acos(North ^ Route);
}
