#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_7(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	fvec[0] = 2 * x[0] * x[0] * x[0] - 12 * x[0] - x[1] - 1;
	fvec[1] = 3 * x[0] * x[0] - 6 * x[1] - x[0] - 3;
	/********************************/
}

void practice3_7(void)
{
	int n = SOLNUMS;
	//double x[SOLNUMS] = { -4.0, 5.0 };	//need to initilize x0
	double x[SOLNUMS];
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_3-7.txt", "w");
	printf("\npractice7\n");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_3-7.txt");
		return;
	}

	for (int i = -4; i <= 4; i++) {
		for (int j = -5; j <= 5; j++) {
			x[0] = double(i);
			x[1] = double(j);
			hybrd1_(fcn3_7, &n, x, fvec, &tol, &info, wa, &lwa);
			fprintf(fp_w, "±Ù (x1, x2) = (%lf, %lf)\n", x[0], x[1]);
			fprintf(fp_w, "f1(x1, x2) = %lf\n", 2 * x[0] * x[0] * x[0] - 12 * x[0] - x[1] - 1);
			fprintf(fp_w, "f2(x1, x2) = %lf\n\n", 3 * x[0] * x[0] - 6 * x[1] - x[0] -3);
		}
	}

	/********************************/










	/********************************/

	fclose(fp_w);
}