#include "my_solver.h"
#define SOLNUMS	4
#define MATCOLS	SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

double C, b;
double a[20];

//	HYBRJ1
void fcn_hw3_1_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag) {
	if (*iflag == 1) {
		/********************************/
		fvec[0] = (x[0] - a[0]) * (x[0] - a[0]) + (x[1] - a[4]) * (x[1] - a[4]) + (x[2] - a[8]) * (x[2] - a[8]) - C * C * (a[16] + x[3] - a[12]) * (a[16] + x[3] - a[12]);
		fvec[1] = (x[0] - a[1]) * (x[0] - a[1]) + (x[1] - a[5]) * (x[1] - a[5]) + (x[2] - a[9]) * (x[2] - a[9]) - C * C * (a[17] + x[3] - a[13]) * (a[17] + x[3] - a[13]);
		fvec[2] = (x[0] - a[2]) * (x[0] - a[2]) + (x[1] - a[6]) * (x[1] - a[6]) + (x[2] - a[10]) * (x[2] - a[10]) - C * C * (a[18] + x[3] - a[14]) * (a[18] + x[3] - a[14]);
		fvec[3] = (x[0] - a[3]) * (x[0] - a[3]) + (x[1] - a[7]) * (x[1] - a[7]) + (x[2] - a[11]) * (x[2] - a[11]) - C * C * (a[19] + x[3] - a[15]) * (a[19] + x[3] - a[15]);
		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/
		fjac[0] = 2 * (x[0] - a[0]);		fjac[4] = 2 * (x[1] - a[4]);		fjac[8] = 2 * (x[2] - a[8]);			fjac[12] = -2 * C * C * (a[16] + x[3] - a[12]);
		fjac[1] = 2 * (x[0] - a[1]);		fjac[5] = 2 * (x[1] - a[5]);		fjac[9] = 2 * (x[2] - a[9]);			fjac[13] = -2 * C * C * (a[17] + x[3] - a[13]);
		fjac[2] = 2 * (x[0] - a[2]);		fjac[6] = 2 * (x[1] - a[6]);		fjac[10] = 2 * (x[2] - a[10]);			fjac[14] = -2 * C * C * (a[18] + x[3] - a[14]);
		fjac[3] = 2 * (x[0] - a[3]);		fjac[7] = 2 * (x[1] - a[7]);		fjac[11] = 2 * (x[2] - a[11]);			fjac[15] = -2 * C * C * (a[19] + x[3] - a[15]);
		/********************************/
	}
}

//	HYBRD1
void fcn_hw3_1_2(int* n, double* x, double* fvec, int* iflag) {
	fvec[0] = (x[0] - a[0]) * (x[0] - a[0]) + (x[1] - a[4]) * (x[1] - a[4]) + (x[2] - a[8]) * (x[2] - a[8]) - C * C * (a[16] + x[3] - a[12]) * (a[16] + x[3] - a[12]);
	fvec[1] = (x[0] - a[1]) * (x[0] - a[1]) + (x[1] - a[5]) * (x[1] - a[5]) + (x[2] - a[9]) * (x[2] - a[9]) - C * C * (a[17] + x[3] - a[13]) * (a[17] + x[3] - a[13]);
	fvec[2] = (x[0] - a[2]) * (x[0] - a[2]) + (x[1] - a[6]) * (x[1] - a[6]) + (x[2] - a[10]) * (x[2] - a[10]) - C * C * (a[18] + x[3] - a[14]) * (a[18] + x[3] - a[14]);
	fvec[3] = (x[0] - a[3]) * (x[0] - a[3]) + (x[1] - a[7]) * (x[1] - a[7]) + (x[2] - a[11]) * (x[2] - a[11]) - C * C * (a[19] + x[3] - a[15]) * (a[19] + x[3] - a[15]);
}

void homework3_1_1() {
	char readfile[256];
	char writefile[256];

	int n = SOLNUMS;
	double x[SOLNUMS];	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	printf("\nhomework1 HYBRJ1\n");
	for (int t = 0; t < 3; t++) {
		printf("test case %d\n", t);
		sprintf(readfile, "GPS_signal_%d.txt", t);
		sprintf(writefile, "GPS_position_3-1_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}
	
		fscanf(fp_r, "%lf %lf", &C, &b);
		for (int i = 0; i < 4; i++) 
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &a[i], &a[i + 4], &a[i + 8], &a[i + 12], &a[i + 16]);
	
		printf("(x1, x2, x3) 입력 : ");
		scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
		x[3] = b;

		hybrj1_(fcn_hw3_1_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);
		fprintf(fp_w, "HYBRJ1로 구한 근 (x1, x2, x3, x4) : (%lf, %lf, %lf, %lf)\n", x[0], x[1], x[2], x[3]);
		fprintf(fp_w, "f1(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[0]) * (x[0] - a[0]) + (x[1] - a[4]) * (x[1] - a[4]) + (x[2] - a[8]) * (x[2] - a[8]) - C * C * (a[16] + x[3] - a[12]) * (a[16] + x[3] - a[12]));
		fprintf(fp_w, "f2(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[1]) * (x[0] - a[1]) + (x[1] - a[5]) * (x[1] - a[5]) + (x[2] - a[9]) * (x[2] - a[9]) - C * C * (a[17] + x[3] - a[13]) * (a[17] + x[3] - a[13]));
		fprintf(fp_w, "f3(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[2]) * (x[0] - a[2]) + (x[1] - a[6]) * (x[1] - a[6]) + (x[2] - a[10]) * (x[2] - a[10]) - C * C * (a[18] + x[3] - a[14]) * (a[18] + x[3] - a[14]));
		fprintf(fp_w, "f4(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[3]) * (x[0] - a[3]) + (x[1] - a[7]) * (x[1] - a[7]) + (x[2] - a[11]) * (x[2] - a[11]) - C * C * (a[19] + x[3] - a[15]) * (a[19] + x[3] - a[15]));

		fclose(fp_r);
		fclose(fp_w);
		printf("\n");
	}
}

void homework3_1_2() {
	char readfile[256];
	char writefile[256];

	int n = SOLNUMS;
	double x[SOLNUMS];
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	printf("\nhomework1 HYBRD1\n");
	for (int t = 0; t < 3; t++) {
		printf("test case %d\n", t);
		sprintf(readfile, "GPS_signal_%d.txt", t);
		sprintf(writefile, "GPS_position_3-2_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}

		fscanf(fp_r, "%lf %lf", &C, &b);
		for (int i = 0; i < 4; i++)
			fscanf(fp_r, "%lf %lf %lf %lf %lf", &a[i], &a[i + 4], &a[i + 8], &a[i + 12], &a[i + 16]);

		printf("(x1, x2, x3) 입력 : ");
		scanf("%lf %lf %lf", &x[0], &x[1], &x[2]);
		x[3] = b;

		hybrd1_(fcn_hw3_1_2, &n, x, fvec, &tol, &info, wa, &lwa);
		fprintf(fp_w, "HYBRD1로 구한 근 (x1, x2, x3, x4) : (%lf, %lf, %lf, %lf)\n", x[0], x[1], x[2], x[3]);
		fprintf(fp_w, "f1(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[0]) * (x[0] - a[0]) + (x[1] - a[4]) * (x[1] - a[4]) + (x[2] - a[8]) * (x[2] - a[8]) - C * C * (a[16] + x[3] - a[12]) * (a[16] + x[3] - a[12]));
		fprintf(fp_w, "f2(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[1]) * (x[0] - a[1]) + (x[1] - a[5]) * (x[1] - a[5]) + (x[2] - a[9]) * (x[2] - a[9]) - C * C * (a[17] + x[3] - a[13]) * (a[17] + x[3] - a[13]));
		fprintf(fp_w, "f3(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[2]) * (x[0] - a[2]) + (x[1] - a[6]) * (x[1] - a[6]) + (x[2] - a[10]) * (x[2] - a[10]) - C * C * (a[18] + x[3] - a[14]) * (a[18] + x[3] - a[14]));
		fprintf(fp_w, "f4(x1, x2 ,x3, x4) = %lf\n", (x[0] - a[3]) * (x[0] - a[3]) + (x[1] - a[7]) * (x[1] - a[7]) + (x[2] - a[11]) * (x[2] - a[11]) - C * C * (a[19] + x[3] - a[15]) * (a[19] + x[3] - a[15]));

		fclose(fp_r);
		fclose(fp_w);
		printf("\n");
	}
}