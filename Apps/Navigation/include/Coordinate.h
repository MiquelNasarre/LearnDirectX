#pragma once
#include "Angles.h"
#include "Math/Vectors.h"

class Coordinate
{
public:
	angle1 latitude;
	angle1 Longitude;

	Coordinate();
	Coordinate(double lat, double Long, ANGLE_TYPE type);
	Coordinate(angle& latitude, angle& Longitude);

	std::string getString(unsigned int D = 3) const;
	void correct();
	Vector3d getVector() const;

	struct PLACES {
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

double getDistance(const Coordinate& Pos1, const Coordinate& Pos2);
double getDirection(const Coordinate& Pos1, const Coordinate& Pos2);