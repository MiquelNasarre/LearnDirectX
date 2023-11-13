#include "Angles.h"
#include "Math/constants.h"

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
