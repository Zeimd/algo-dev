#include <stdio.h>
#include <math.h>

constexpr double pi = 3.14159265358979323846;
constexpr double deg2rad =  pi / 180.0;
constexpr double rad2deg = 180.0 / pi;

constexpr double minuteOfArc = 1.0 / 60.0;
constexpr double minuteOfArcInverse = 1.0 / minuteOfArc;

constexpr double tenthOfminuteOfArc = minuteOfArc * 0.1;
constexpr double tenthOfminuteOfArcInverse = 1.0 / tenthOfminuteOfArc;


double LatitudeNewtonMethod(double dec, double altitude, double zn, double assumedLat, double tolerance, int& out_iters);

double PrecisionArcMin(double x);
double PrecisionHalf(double x);

void PrintDegMins(double degrees)
{
	int degs = degrees;

	double frac = degrees - double(degs);

	// Round minutes to precision of 0.1'
	double mins = round(frac * 60.0 * 10.0) / 10.0;

	printf("deg = %i, mins = %lf\n", degs, mins);
}

int main()
{
	printf("minute of arc = %lf\n", minuteOfArc);

	double dec, altitude, zn;

	// alioth


	//dec = 55.83;	
	//altitude = 56.19;
	//zn = 71.58;

	// capella
	dec = 46.02;
	zn = 253.02;
	//zn = 252.5;
	altitude = 59.64;

	printf("input: dec = %lf, zn = %lf, altitude = %lf\n", dec, zn, altitude);

	dec = PrecisionArcMin(dec);
	altitude = PrecisionArcMin(altitude);
	
	zn = PrecisionHalf(zn);

	printf("rounded: dec = %lf, zn = %lf, altitude = %lf\n", dec, zn, altitude);

	double assumedLat = 0;

	int iters = 0;

	double lat = LatitudeNewtonMethod(dec, altitude, zn, assumedLat, tenthOfminuteOfArc, iters);

	printf("lat = ");
	PrintDegMins(lat);
	printf("\n");
	printf("Took %i iterations\n", iters);

	return 0;
}

double PrecisionArcMin(double x)
{
	return round(tenthOfminuteOfArcInverse * x) * tenthOfminuteOfArc;
}

double PrecisionHalf(double x)
{
	return round(x * 4.0) / 4.0;
}

double LatitudeNewtonMethod(double dec, double altitude, double zn, double assumedLat, double tolerance, int& out_iters)
{
	double a = sin(dec * deg2rad);
	double b = sin(altitude * deg2rad);

	double z = zn;
	if (zn > 180.0)
	{
		z = 360 - zn;
	}

	//printf("zn = %.15lf, z = %.15lf\n", zn, z);

	double c = cos(altitude * deg2rad) * cos(z * deg2rad);

	//printf("a = %.15lf\n", a);
	//printf("b = %.15lf\n", b);
	//printf("c = %.15lf\n", c);

	int iters = 0;

	double lat = assumedLat;

	double old;
	double error;

	do
	{
		++iters;

		old = lat;

		lat = lat - (a - b * sin(lat) - c * cos(lat)) / (-b * cos(lat) + c * sin(lat));

		error = fabs(old - lat);

		//printf("iter = %i\n", iters);
		//printf("lat = %.15lf (deg %.15lf)\n", lat,lat * rad2deg);
		//printf("old = %.15lf (deg %.15lf)\n", old,old * rad2deg);
		//printf("error = %.15lf (deg %.15lf)\n", error,error * rad2deg);


	} while (error > tolerance);


	out_iters = iters;
	return lat*rad2deg;
}