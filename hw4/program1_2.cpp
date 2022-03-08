#include "my_solver.h"

extern double (*_f)(double);	//	원함수
extern double (*_fp)(double);	//	일계도함수

/*********************************************
  Secant Method
**********************************************/
void program1_2(FILE* fp) {
	double x0, x1, temp;
	int n;

	if (fp == NULL)
		return;

	//	초기값 입력
	printf("Input x0, x1 : ");
	scanf("%lf %lf", &x0, &x1);

	fprintf(fp, " n       xn1             |f(xn1)|\n");
	fprintf(fp, "%2d %.15e %.15e\n", 0, x0, fabs(_f(x0)));
	fprintf(fp, "%2d %.15e %.15e\n", 1, x1, fabs(_f(x1)));
	for (n = 2; n <= Nmax; n++) {
		temp = x1 - _f(x1) * (x1 - x0) / (_f(x1) - _f(x0));
		fprintf(fp, "%2d %.15e %.15e\n", n, temp, fabs(_f(temp)));

		if (fabs(_f(temp)) < DELTA || fabs(temp - x1) < EPSILON)
			break;

		x0 = x1;
		x1 = temp;
	}
	fprintf(fp, "\n");
}
