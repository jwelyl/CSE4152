#include "my_solver.h"

int num, n;		//	sampling 개수 + 1 (if num = 100 -> n = 99, x0 ~ x99)
double* arr_x;	//	pdf x 좌표
double* arr_y;	//	pdf y 좌표
double h, a, b;	//	정규화 전 h, a, b 	
double k;		//	y 정규화 전 적분 값 구간 [0, 1] 적분값

double calc_integral(double x) {
	int m = -1;		//	x 직전에 sampling된 좌표의 번호
	double ret = 0;

	//	x 직전에 sampling된 좌표 찾기
	for (int i = 0; i < num; i++) {
		if (arr_x[i] > x) {
			m = i - 1;
			break;
		}
	}

	if (m == -1) {	//	x 직전에 sampling된 좌표가 없을 경우, x가 1 이상인 경우
		//	구간 [0, 1] 적분(사다리꼴 적분)
		ret = (h / 2) * (arr_y[0] + arr_y[n]);
		for (int i = 1; i < n; i++)
			ret += (h * arr_y[i]);
	}
	else {
		//	구간 [0, xm] 적분(사다리꼴 적분)
		ret = (h / 2) * (arr_y[0] + arr_y[m]);
		for (int i = 1; i < m; i++)
			ret += (h * arr_y[i]);

		ret += (arr_y[m] + (((arr_y[m + 1] - arr_y[m]) / (arr_x[m + 1] - arr_x[m])) * ((x - arr_x[m]) / 2))) * (x - arr_x[m]);
	}

	return ret;
}

void program2_1()
{
	FILE* fp_r, *fp_w;
	__int64 start, freq, end;
	float resultTime = 0;
	
	fp_r = fopen("sampling_table.txt", "r");
	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	/***************************************************/
	fscanf(fp_r, "%d %lf", &num, &h);
	n = num - 1;	//	n : sampling 개수

	arr_x = new double[num];
	arr_y = new double[num];

	for (int i = 0; i < num; i++) {
		fscanf(fp_r, "%lf %lf", &arr_x[i], &arr_y[i]);

		if (i == 0)
			a = arr_x[i];
		else if (i == num - 1)
			b = arr_x[i];

	}

	//	x 정규화
	for (int i = 0; i < num; i++) 
		arr_x[i] = (arr_x[i] - a) / (b - a);

	a = 0;
	b = 1;
	h = (b - a) / n;

	//	정규화 전 적분값 계산
	k = (h / 2) * (arr_y[0] + arr_y[n]);
	for (int i = 1; i < n; i++)
		k += (h * arr_y[i]);

	//	y값 정규화, pdf_table.txt 작성
	fprintf(fp_w, "%d %lf\n", num, h);
	for (int i = 0; i < num; i++) {
		arr_y[i] = arr_y[i] / k;
		fprintf(fp_w, "%.15lf %.15lf\n", arr_x[i], arr_y[i]);
	}

	printf("*** Integrating the pdf from 0.0 to 1.0 = %lf\n", calc_integral(1.0));
	printf("*** Integrating the pdf from 0.0 to 0.25 = %lf\n", calc_integral(0.25));
	printf("*** Integrating the pdf from 0.25 to 0.5 = %lf\n", calc_integral(0.5) - calc_integral(0.25));
	printf("*** Integrating the pdf from 0.5 to 0.75 = %lf\n", calc_integral(0.75) - calc_integral(0.5));
	printf("*** Integrating the pdf from 0.75 to 1.0 = %lf\n", calc_integral(1.0) - calc_integral(0.75));

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
	
	delete[] arr_x;
	delete[] arr_y;
}
