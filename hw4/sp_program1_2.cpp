#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
	float x0, x1, temp;
	int n;

	if (fp == NULL)
		return;

	if (fp == NULL)
		return;

	//	초기값 입력
	printf("Input x0, x1 : ");
	scanf("%f %f", &x0, &x1);

	fprintf(fp, " n       xn1             |f(xn1)|\n");
	fprintf(fp, "%2d %.15e %.15e\n", 0, x0, fabs(_sp_f(x0)));
	fprintf(fp, "%2d %.15e %.15e\n", 1, x1, fabs(_sp_f(x1)));
	for (n = 2; n <= Nmax; n++) {
		temp = x1 - _sp_f(x1) * (x1 - x0) / (_sp_f(x1) - _sp_f(x0));
		fprintf(fp, "%2d %.15e %.15e\n", n, temp, fabs(_sp_f(temp)));

		if (fabs(_sp_f(temp)) < DELTA || fabs(temp - x1) < EPSILON)
			break;

		x0 = x1;
		x1 = temp;
	}
	fprintf(fp, "\n");
}
