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
};

namespace PLACES {
	const Coordinate Greenwich	(51.4780	, 0.0014	, DEGREES);
	const Coordinate London		(51.5098	, -0.1180	, DEGREES);
	const Coordinate Barcelona	(41.3874	, 2.1686	, DEGREES);
	const Coordinate NewYork	(40.7831	, -73.9712	, DEGREES);
	const Coordinate Canary		(27.9202	, -15.5474	, DEGREES);
	const Coordinate Grenada	(12.1165	, -61.6790	, DEGREES);
	const Coordinate Gibraltar	(36.1408	, -5.3536	, DEGREES);
	const Coordinate Auckland	(-36.8509	, 174.7645	, DEGREES);
}

double getDistance(const Coordinate& Pos1, const Coordinate& Pos2);
double getDirection(const Coordinate& Pos1, const Coordinate& Pos2);