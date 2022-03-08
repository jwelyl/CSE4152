#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method -- HOMEWORK
**********************************************/
void program1_3(FILE *fp)
{
	double a0, b0, x0, x1 , temp;
	int n;

	if (fp == NULL)
		return;

	//	초기값 입력
	printf("Input a0, b0 : ");
	scanf("%lf %lf", &a0, &b0);

	x0 = (a0 + b0) / 2;
	fprintf(fp, " n       xn1             |f(xn1)|\n");
	fprintf(fp, "%2d %.15e %.15e\n", 0, x0, fabs(_f(x0)));

	for (int n = 1; n <= Nmax; n++) {
		if (_f(x0) * _f(a0) < 0)
			b0 = x0;
		else if (_f(x0) * _f(b0) < 0)
			a0 = x0;

		x1 = (a0 + b0) / 2;
		fprintf(fp, "%2d %.15e %.15e\n", 0, x1, fabs(_f(x1)));
	
		if (fabs(_f(x1) < DELTA || fabs(x1 - x0) < EPSILON))
			break;

		x0 = x1;
	}

	fprintf(fp, "\n");
}