#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

/**
*  f0(x0, x1, x2, x3) = x0 + 10 * x1 + 9
*  f1(x0, x1, x2, x3) = sqrt(5) * (x2 - x3) - 2 * sqrt(5)
*  f2(x0, x1, x2, x3) = (x1 - 2 * x2)^2 - 9
*  f3(x0, x1, x2, x3) = sqrt(10) * (x0 - x3)^2 - 2 * sqrt(10);
 **/

//	HYBRJ1
void fcn_hw3_2(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
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

void homework3_2() {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.9, -0.9, 1.25, -1.25 };
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-2.txt", "w");
	printf("\nhomework2\n");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3-2.txt");
		return;
	}

	fprintf(fp_w, "√ ±‚ (x, y, z, w) = (%lf, %lf, %lf, %lf)\n", x[0], x[1], x[2], x[3]);
	hybrj1_(fcn_hw3_2, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
	fprintf(fp_w, "±Ÿ (x, y, z, w) = (%lf, %lf, %lf, %lf)\n", x[0], x[1], x[2], x[3]);
	fprintf(fp_w, "f1(x, y, z, w) = %lf\n", x[0] + 10 * x[1] + 9);
	fprintf(fp_w, "f2(x, y, z, w) = %lf\n", sqrt(5) * (x[2] - x[3]) - 2 * sqrt(5));
	fprintf(fp_w, "f3(x, y, z, w) = %lf\n", (x[1] - (2 * x[2])) * (x[1] - (2 * x[2])) - 9);
	fprintf(fp_w, "f4(x, y, z, w) = %lf\n", sqrt(10) * (x[0] - x[3]) * (x[0] - x[3]) - 2 * sqrt(10));

	fclose(fp_w);
}