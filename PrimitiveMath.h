#include <math.h>
#ifndef _MATH_H_
//math
const float pi = 3.1415926535;
double FixedCos(float a) {
	double x = cos(a / 180.0 * pi);
	return x;
}
double FixedSin(float a) {
	double x = sin(a / 180.0 * pi);
	return x;
}

//vector initialising
struct Vector {
public:
	double X = 0;
	double Y = 0;
	Vector(double x = 0, double y = 0) {
		X = x;
		Y = y;
	}
};
Vector operator + (Vector v1, Vector v2) {
	Vector v3 = { v1.X + v2.X, v1.Y + v2.Y };
	return v3;
}
Vector operator - (Vector v1, Vector v2) {
	return Vector(v1.X - v2.X, v1.Y - v2.Y);
}
Vector operator * (Vector v1, float f) {
	Vector v3 = { v1.X * f, v1.Y * f };
	return v3;
}
Vector operator * (Vector v1, double f) {
	Vector v3 = { v1.X * f, v1.Y * f };
	return v3;
}

//vector functions
double VectorLengh(Vector v1) {
	return pow((v1.X * v1.X + v1.Y * v1.Y), 0.5);
}
Vector CreateVector(double x1, double y1, double x2, double y2) {
	Vector v1 = { x2 - x1, y2 - y1 };
	return v1;
}
Vector ParseVector(Vector v1, double x, double y) {
	return Vector(v1.X + x, v1.Y + y);
}
Vector RotateVector(Vector v1, float deg) {
	double x = v1.X * FixedCos(deg) - v1.Y * FixedSin(deg);
	double y = v1.X * FixedSin(deg) + v1.Y * FixedCos(deg);
	return Vector(x, y);
}
double VectorRotation(Vector v) {
	double res = atan(v.Y / v.X) / pi * 180;
	if (v.X < 0 and v.Y < 0) {
		res += 180;
	}
	if (v.X < 0 and v.Y > 0) {
		res -= 180;
	}
	if (res < 0) {
		res = 360 + res;
	}
	if (res >= 360) {
		res = res - 360;
	}
	return res;
}
double DOT(Vector v1, Vector v2) {
	return v1.X * v2.X + v1.Y * v2.Y;
}
bool BetweenX(double number, double between1, double between2) {
	if ((number >= between1 and number <= between2) or (number >= between2 and number <= between1)) return true;
	else return false;
}
bool BetweenV(Vector point, Vector between1, Vector between2) {
	if (BetweenX(point.X, between1.X, between2.X) and BetweenX(point.Y, between1.Y, between2.Y)) return true;
	else return false;
}
Vector normalise( Vector v ) {
	return Vector(v.X / VectorLengh(v), v.Y / VectorLengh(v));
}

//color
struct Color {
public:
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 255;
};
#endif