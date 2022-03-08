#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Newton-Rapson Method
**********************************************/
void sp_program1_1(FILE* fp) {
	float x0, x1;
	int n;

	if (fp == NULL)
		return;

	//	초기값 입력
	printf("Input x0 : ");
	scanf("%f", &x0);

	fprintf(fp, " n       xn1             |f(xn1)|\n");
	fprintf(fp, "%2d %.15e %.15e\n", 0, x0, fabs(_sp_f(x0)));
	for (n = 1; n <= Nmax; n++) {
		x1 = x0;
		x1 -= _sp_f(x0) / _sp_fp(x0);
		fprintf(fp, "%2d %.15e %.15e\n", n, x1, fabs(_sp_f(x1)));

		if (fabs(_sp_f(x1)) < DELTA || fabs(x1 - x0) < EPSILON)
			break;

		x0 = x1;
	}
	fprintf(fp, "\n");

}
