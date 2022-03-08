#define _USE_MATH_DEFINES
#include <math.h>

double l = 89;
double h = 49;
double D = 55;
double B1 = 11.5;
double A = l * sin(B1 * M_PI / 180);
double B = l * cos(B1 * M_PI / 180);
double C = (h + 0.5 * D) * sin(B1 * M_PI / 180) - 0.5 * D * tan(B1 * M_PI / 180);
double E = (h + 0.5 * D) * cos(B1 * M_PI / 180) - 0.5 * D;

double _f1(double x) {
	return pow(x, 2) - 4 * x + 4 - log(x);
}

double _fp1(double x) {
	return 2 * x - 4 - double(1) / x;
}

double _f2(double x) {
	return x + 1 - 2 * sin(M_PI * x);
}

double _fp2(double x) {
	return 1 - 2 * M_PI * cos(M_PI * x);
}

double _f3(double x) {
	return pow(x, 4) - 11.0 * pow(x, 3) + 42.35 * pow(x, 2) - 66.55 * x + 35.1384;
}

double _fp3(double x) {
	return 4 * pow(x, 3) - 33.0 * pow(x, 2) + 84.70 * x - 66.55;
}

double _f_sqrt(double x) {
	return x * x - 2.0;
}

double _fp_sqrt(double x) {
	return 2.0 * x;
}

double _f_vehicle(double x) {
	return A * sin(x) * cos(x) + B * sin(x) * sin(x) - C * cos(x) - E * sin(x);
}

double _fp_vehicle(double x) {
	return A * cos(2 * x) + 2 * B * sin(x) * cos(x) + C * sin(x) - E * cos(x);
}

double _f_comp(double x) {
	return log(x) - 1;
}

double _fp_comp(double x) {
	return double(1) / x;
}
