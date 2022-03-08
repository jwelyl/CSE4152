#include "my_solver.h"
#include <math.h>
#include <stdlib.h>

extern int num, n;		//	sampling 개수 + 1 (if num = 100 -> n = 99, x0 ~ x99)
extern double* arr_x;	//	pdf x 좌표
extern double* arr_y;	//	pdf y 좌표
extern double h, a, b;

double equation(double x, double U) {
	return (calc_integral(x) - U);
}

void bisection(FILE* fp_w, double U) {
	double result = 0;
	double start = 0, end = 1, mid0, mid1;	//	bisection 구간

	if (!fp_w) return;

	mid0 = (start + end) / 2;

	for (int n = 1; n <= Nmax; n++) {
		if (equation(mid0, U) * equation(start, U) < 0)
			end = mid0;
		else if (equation(mid0, U) * equation(end, U) < 0)
			start = mid0;

		mid1 = (start + end) / 2;

		if (fabs(equation(mid1, U)) < DELTA || fabs(mid1 - mid0) < EPSILON)
			break;

		mid0 = mid1;
	}

	fprintf(fp_w, "%.15lf\n", mid1);
}

void program2_2()
{
	FILE* fp_r, *fp_w;
	int d, rand_num;	//	rand_num : 생성할 난수 개수
	double U;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/
	//printf("program2_2\n");
	printf("\n실습 문제 2. pdf를 따르는 난수 개수 입력 : ");
	scanf("%d", &rand_num);
	fscanf(fp_r, "%d %lf", &num, &h);
	//printf("num : %d, h : %lf\n", num, h);
	n = num - 1;

	arr_x = new double[num];
	arr_y = new double[num];
	for (int i = 0; i < num; i++) 
		fscanf(fp_r, "%lf %lf", &arr_x[i], &arr_y[i]);
		//printf("%lf %lf\n", arr_x[i], arr_y[i]);
	
	fprintf(fp_w, "%d\n", rand_num);
	for (int i = 0; i < rand_num; i++) {
		d = rand() % RAND_MAX;
		U = (double)d / RAND_MAX;	//	(0, 1) 난수 발생
		//printf("%.15e\n", U);
		bisection(fp_w, U);
	}

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);

	delete[] arr_x;
	delete[] arr_y;
}
