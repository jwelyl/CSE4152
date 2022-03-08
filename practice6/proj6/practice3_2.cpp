#include "my_solver.h"

//	(a + bi)(c + di)의 실수부
double mult_r(double a, double b, double c, double d) {
	return a * c - b * d;
}

//	(a + bi)(c + di)의 허수부
double mult_i(double a, double b, double c, double d) {
	return a * d + b * c;
}

void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i, j;

	printf("\npractice2\n");
	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

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

		fscanf(fp_r, "%d", &DEGREE);
	/*	printf("D : %d\n", DEGREE);*/
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);

		/********************************/
		double* cache_r = (double*)malloc(sizeof(double) * DEGREE);
		double* cache_i = (double*)malloc(sizeof(double) * DEGREE);
		double f_r = 0;	//	f(x)에 근을 대입했을때 실수부
		double f_i = 0;	//	f(x)에 근을 대입했을때 허수부

		for (i = 0; i < NCOEF; i++) 
			fscanf(fp_r, "%lf", &poly[i]);

		rpoly_(poly, &DEGREE, zeror, zeroi, &fail);

		//printf("\n\nt = %d\n", t);
		for (i = 0; i < DEGREE; i++) {
			//printf("%d번째 근\n", i + 1);
			//printf("a + bi = %lf + %lfi\n", zeror[i], zeroi[i]);
			fprintf(fp_w, "x%d = %lf + %lfi\n", i + 1, zeror[i], zeroi[i]);
			fprintf(fp_w, "f(x%d) = ", i + 1);

			//printf("캐시 계산 전\n");
			for (j = 0; j < DEGREE; j++) {
				cache_r[j] = zeror[i];
				cache_i[j] = zeroi[i];
				//printf("cache[%d] : a + bi = %lf + %lfi\n", DEGREE - j, cache_r[j], cache_i[j]);
			}

			//printf("캐시 계산 후\n");
			for (j = 0; j < DEGREE; j++) {
				int k = DEGREE - j - 1;

				for (; k > 0; k--) {
					double tmp = cache_r[j];
					//printf("k = %d\n", k);
					cache_r[j] = mult_r(cache_r[j], cache_i[j], zeror[i], zeroi[i]);
					cache_i[j] = mult_i(tmp, cache_i[j], zeror[i], zeroi[i]);
				}

				//printf("cache[%d] : a + bi = %lf + %lfi\n", DEGREE - j, cache_r[j], cache_i[j]);
			}

			//printf("중간 f_r = %lf\n", f_r);
			//printf("중간 f_i = %lf\n", f_i);

			for (j = 0; j < DEGREE; j++) {
				//printf("계수 : %lf\n", poly[j]);
				f_r += poly[j] * cache_r[j];
				f_i += poly[j] * cache_i[j];

				//printf("중간 f_r = %lf\n", f_r);
				//printf("중간 f_i = %lf\n", f_i);
			}
			//printf("상수항 : %lf\n", poly[NCOEF - 1]);
			f_r += poly[NCOEF - 1];
			//printf("\n대입 결과 : %lf + %lfi\n", f_r, f_i);
			fprintf(fp_w, "%lf + %lfi\n\n", f_r, f_i);
		}
		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}