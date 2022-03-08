#include "my_solver.h"

extern double (*_f)(double);	//	���Լ�
extern double (*_fp)(double);	//	�ϰ赵�Լ�

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE* fp) {
	double x0, x1;
	int n;

	if (fp == NULL)
		return;

	//	�ʱⰪ �Է�
	printf("Input x0 : ");
	scanf("%lf", &x0);

	fprintf(fp, " n       xn1             |f(xn1)|\n");
	fprintf(fp, "%2d %.15e %.15e\n", 0, x0, fabs(_f(x0)));
	for (n = 1; n <= Nmax; n++) {
		x1 = x0;
		x1 -= _f(x0) / _fp(x0);
		fprintf(fp, "%2d %.15e %.15e\n", n, x1, fabs(_f(x1)));
		
		if (fabs(_f(x1) < DELTA || fabs(x1 - x0) < EPSILON))
			break;

		x0 = x1;
	}
	fprintf(fp, "\n");
}
