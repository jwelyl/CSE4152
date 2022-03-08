#include "my_solver.h"

#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

/**
*  f0(x0, x1) = (sin(x0 * x1 + M_PI / 6) + sqrt(x0 * x0 * x1 * x1 + 1)) / cos(x0 - x1) + 2.8
*  f1(x0, x1) = (x0 * exp(x0 * x1 + (M_PI / 6)) - sin(x0 - x1)) / sqrt(x0 * x0 * x1 * x1 + 1) - 1.66
 **/

 //	HYBRD1
void fcn_hw3_3(int* n, double* x, double* fvec, int* iflag) {
	/********************************/
	fvec[0] = ((sin(x[0] * x[1] + (M_PI / 6)) + sqrt(x[0] * x[0] * x[1] * x[1] + 1)) / cos(x[0] - x[1])) + 2.8;
	fvec[1] = ((x[0] * exp(x[0] * x[1] + (M_PI / 6)) - sin(x[0] - x[1])) / sqrt(x[0] * x[0] * x[1] * x[1] + 1)) - 1.66;
	/********************************/
}

void homework3_3() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 20.0, 0.0 };
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-3.txt", "w");
	printf("\nhomework3\n");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3_3.txt");
		return;
	}

	fprintf(fp_w, "초기 (x, y) = (%lf, %lf)\n", x[0], x[1]);
	hybrd1_(fcn_hw3_3, &n, x, fvec, &tol, &info, wa, &lwa);
	fprintf(fp_w, "근 (x, y) = (%lf, %lf)\n", x[0], x[1]);
	fprintf(fp_w, "f1(x, y) = %lf\n", ((sin(x[0] * x[1] + (M_PI / 6)) + sqrt(x[0] * x[0] * x[1] * x[1] + 1)) / cos(x[0] - x[1])) + 2.8);
	fprintf(fp_w, "f2(x, y) = %lf\n", ((x[0] * exp(x[0] * x[1] + (M_PI / 6)) - sin(x[0] - x[1])) / sqrt(x[0] * x[0] * x[1] * x[1] + 1)) - 1.66);

	if (!(((sin(x[0] * x[1] + (M_PI / 6)) + sqrt(x[0] * x[0] * x[1] * x[1] + 1)) / cos(x[0] - x[1])) + 2.8 == 0 && ((x[0] * exp(x[0] * x[1] + (M_PI / 6)) - sin(x[0] - x[1])) / sqrt(x[0] * x[0] * x[1] * x[1] + 1)) - 1.66))
		fprintf(fp_w, "구한 근 (%lf, %lf)가 올바른 근이 아님\n", x[0], x[1]);
	fclose(fp_w);
}