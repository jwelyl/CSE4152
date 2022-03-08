#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

/**
*  f0(x0, x1) = (sin(x0 * x1) + sqrt(x0 * x0 * x1 * x1 + 1)) / cos(x0 - x1) + 2.8
*  f1(x0, x1) = (x0 * exp(x0 * x1 + (M_PI / 6)) - sin(x0 - x1)) / sqrt(x0 * x0 * x1 * x1 + 1) - 1.66
 **/


 //	HYBRJ1
void fcn_hw3_3_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/
		fvec[0] = x[0] + 10 * x[1] + 9;
		fvec[1] = sqrt(5) * (x[2] - x[3]) - 2 * sqrt(5);
		fvec[2] = (x[1] - (2 * x[2])) * (x[1] - (2 * x[2])) - 9;
		fvec[3] = sqrt(10) * (x[0] - x[3]) * (x[0] - x[3]) - 2 * sqrt(10);
		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/
		fjac[0] = 1;								fjac[4] = 10;							fjac[8] = 0;							fjac[12] = 0;
		fjac[1] = 0;								fjac[5] = 0;							fjac[9] = sqrt(5);						fjac[13] = -sqrt(5);
		fjac[2] = 0;								fjac[6] = 2 * (x[1] - (2 * x[2]));		fjac[10] = -4 * (x[1] - (2 * x[2]));	fjac[14] = 0;
		fjac[3] = 2 * sqrt(10) * (x[0] - x[3]);		fjac[7] = 0;							fjac[11] = 0;							fjac[15] = -2 * sqrt(10) * (x[0] - x[3]);
		/********************************/
	}
}

void homework3_3_1() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 20.0, 0.0 };
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-3_1.txt", "w");
	printf("\nhomework3_3_1\n");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3-2.txt");
		return;
	}

	fprintf(fp_w, "√ ±‚ (x, y) = (%lf, %lf)\n", x[0], x[1]);
	hybrj1_(fcn_hw3_3_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "±Ÿ (x, y) = (%lf, %lf)\n", x[0], x[1]);
	fprintf(fp_w, "f1(x, y) = %lf\n", x[0] + 10 * x[1] + 9);
	fprintf(fp_w, "f2(x, y) = %lf\n", sqrt(5) * (x[2] - x[3]) - 2 * sqrt(5));

	fclose(fp_w);
}