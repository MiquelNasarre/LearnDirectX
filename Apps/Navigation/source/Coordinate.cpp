#include "Coordinate.h"
#include "Math/constants.h"

Coordinate::Coordinate()
{
	*this = PLACES::Greenwich;
}

Coordinate::Coordinate(double lat, double Long, ANGLE_TYPE type)
	: latitude(lat, type), Longitude(Long, type)
{
	correct();
}

Coordinate::Coordinate(angle& lat, angle& Long)
	: latitude(lat.getRaw(DEGREES), DEGREES), Longitude(Long.getRaw(DEGREES), DEGREES)
{
	correct();
}

std::string Coordinate::getString(unsigned int D) const
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
		latitude.angleDeg = -180 - latitude.angleDeg;
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