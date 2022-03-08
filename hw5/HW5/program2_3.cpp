#include "my_solver.h"
#include <math.h>

extern int num, n;		//	sampling 개수 + 1 (if num = 100 -> n = 99, x0 ~ x99)
extern double* arr_x;	//	pdf x 좌표	//	Xi
extern double* arr_y;	//	pdf y 좌표	//	Pxi
extern double h, a, b;
double* rand_u;
double* result_x;	//	생성한 난수
int* cnt;			//	각 구간에 존재하는 난수 개수

//	지수 분포 Px(X)
double exponential_dist(double x, double lambda) {
	return lambda * exp(-lambda * x);
}

//	지수 분포 integral Px(x) = Fx(X)
double exponential_integral(double x, double lambda) {
	return exp(-lambda * x) * (-1) + 1;
}

//	이론적인 지수 분포 평균
double exponential_mean(double lambda) {
	return (double)1 / lambda;
}

//	이론적인 지수 분포 분산
double exponential_variance(double lambda) {
	return (double)1 / pow(lambda, 2);
}

//	방정식
double equation(double x, double lambda, double U) {
	return exponential_integral(x, lambda) - U;
}

//	Newton-Raphson Method
void Newton_Raphson(FILE* fp, double lambda, double U) {
	double x0 = 0, x1;

	for (int n = 1; n <= Nmax; n++) {
		x1 = x0;
		x1 -= equation(x0, lambda, U) / exponential_dist(x0, lambda);
		//printf("next x : %lf\n", x1);

		if (fabs(equation(x1, lambda, U)) < DELTA || fabs(x1 - x0) < EPSILON)
			break;

		x0 = x1;
	}
	fprintf(fp, "%.15lf\n", x1);
	//fprintf(fp, "입력 난수 : %lf, 생성 난수 : %lf\n", U, x1);
}

//	선형 보간
//	임의의 확률 밀도 함수 sampling 결과가 주어졌을 때, F'(x) = px(x)를 모사하여 반환함
double pdf(double x) {
	int m = -1;
	double s, ret;

	//printf("num = %d\n", num);
	if (x <= 0) ret = arr_x[0];
	else if (x >= 1) {
		printf("something was wrong!\n");
		ret = arr_x[num - 1];
	}
	else {
		for (int i = 1; i < num; i++) {
			if (arr_x[i] > x) {
				m = i - 1;
				break;
			}
		}

		if (m == -1) {
			printf("something was wrong!!!\n");
			return -1;
		}

		s = (x - arr_x[m]) / (arr_x[m + 1] - arr_x[m]);
		ret = (1 - s) * arr_y[m] + s * arr_y[m + 1];
		
		if (ret < 0) {
			printf("something was wrong!!!!!\n");
		}
	}

	return ret;
}

//	FILE OUTPUT -> xn return
//	HW bisection
double bisection(double U) {
	double start = 0, end = 1, mid0, mid1;	//	bisection 구간
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

	return mid1;
}

//	HW secant
double secant(double U) {
	//double x0 = 0, x1 = 1, temp;
	double x0, x1, temp, start = 0, end = 1, mid0;
	mid0 = (start + end) / 2;
	
	for (int i = 0; i < 10; i++) {
		if (equation(mid0, U) * equation(start, U) < 0)
			end = mid0;
		else if (equation(mid0, U) * equation(end, U) < 0)
			start = mid0;

		mid0 = (start + end) / 2;
	}

	x0 = start, x1 = end;
	//printf("secant 방법 초기값 x0 : %lf, x1 : %lf\n", x0, x1);

	for (int n = 2; n <= Nmax; n++) {
		temp = x1 - equation(x1, U) * (x1 - x0) / (equation(x1, U) - equation(x0, U));

		if (fabs(equation(temp, U)) < DELTA || fabs(temp - x1) < EPSILON)
			break;

		x0 = x1;
		x1 = temp;
	}

	return temp;
}

//	HW Newton Raphson
double newton_raphson(double U) {
	double x0, x1, ret, start = 0, end = 1, mid0;
	mid0 = (start + end) / 2;

	for (int i = 0; i < 10; i++) {
		if (equation(mid0, U) * equation(start, U) < 0)
			end = mid0;
		else if (equation(mid0, U) * equation(end, U) < 0)
			start = mid0;

		mid0 = (start + end) / 2;
	}

	x0 = mid0;
	for (int i = 1; i <= Nmax; i++) {
		double a;
		x1 = x0;
		x1 -= (equation(x0, U) / pdf(x0));
		a = pdf(x0);
		
		if (x1 > 1) {
			printf("x1 > 1(%lf)\n", x1);
			printf("p(x) : %lf\n", a);
		}
		if (fabs(equation(x1, U)) < DELTA || fabs(x1 - x0) < EPSILON)
			break;
		
		x0 = x1;
	}
	ret = x1;

	return ret;
}

// HOMEWORK
void homework2_1() {
	FILE* fp_w;
	int rand_num;	//	생성할 난수 개수
	double lambda;

	fp_w = fopen("hw2_1result.txt", "w");
	printf("\nHW2_1. 지수 분포를 따르는 난수 개수 입력 : ");
	scanf("%d", &rand_num);
	rand_u = new double[rand_num];

	//	입력받은 개수만큼 난수 U 생성
	for (int i = 0; i < rand_num; i++) {
		rand_u[i] = (double)(rand() % RAND_MAX);
		rand_u[i] = rand_u[i] / RAND_MAX;
	
	//	printf("난수 %d : %lf\n", i, rand_u[i]);
	}

	//	서로 다른 lambda를 입력받아 inversion 방법을 이용하여 지수 분포를 따르는 난수 생성
	for (int t = 0; t < 3; t++) {
		printf("lambda%d 입력 : ", t + 1);
		scanf("%lf", &lambda);
		fprintf(fp_w, "case %d. lambda = %lf\n", t + 1, lambda);
		
		for (int i = 0; i < rand_num; i++)
			Newton_Raphson(fp_w, lambda, rand_u[i]);
		fprintf(fp_w, "생성한 난수 개수 : %d\n", rand_num);
		fprintf(fp_w, "이론적인 평균 : %.15lf\n", exponential_mean(lambda));
		fprintf(fp_w, "이론적인 분산 : %.15lf\n", exponential_variance(lambda));
		fprintf(fp_w, "\n");
	}

	fclose(fp_w);
	delete[] rand_u;
}

// HOMEWORK
//	Bisection
void program2_2_a()
{
	__int64 start, freq, end;
	float resultTime = 0;
	FILE* fp_r, * fp_w;
	int d, rand_num;
	double U;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("HW2_2_a_result.txt", "w");

	printf("\nHW2_2_a. pdf를 따르는 난수 개수 입력(bisection) : ");
	scanf("%d", &rand_num);
	fscanf(fp_r, "%d %lf", &num, &h);
	n = num - 1;

	arr_x = new double[num];
	arr_y = new double[num];
	result_x = new double[rand_num];
	for (int i = 0; i < num; i++)
		fscanf(fp_r, "%lf %lf", &arr_x[i], &arr_y[i]);
	CHECK_TIME_START;
	// something to do...
	for (int i = 0; i < rand_num; i++) {
		d = rand() % RAND_MAX;
		U = (double)d / RAND_MAX;
		result_x[i] = bisection(U);
	}
	CHECK_TIME_END(resultTime);

	fprintf(fp_w, "%d\n", rand_num);
	for (int i = 0; i < rand_num; i++)
		fprintf(fp_w, "%.15lf\n", result_x[i]);

	printf("The program2_2_a run time is %f(ms)..\n", resultTime * 1000.0);

	delete[] arr_x;
	delete[] arr_y;
	delete[] result_x;
	fclose(fp_r);
	fclose(fp_w);
}

// HOMEWORK
//	Secant
void program2_2_b()
{
	__int64 start, freq, end;
	float resultTime = 0;
	FILE* fp_r, * fp_w;
	int d, rand_num;
	double U;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("HW2_2_b_result.txt", "w");

	printf("\nHW2_2_b. pdf를 따르는 난수 개수 입력(secant) : ");
	scanf("%d", &rand_num);
	fscanf(fp_r, "%d %lf", &num, &h);
	n = num - 1;

	arr_x = new double[num];
	arr_y = new double[num];
	result_x = new double[rand_num];
	for (int i = 0; i < num; i++)
		fscanf(fp_r, "%lf %lf", &arr_x[i], &arr_y[i]);
	CHECK_TIME_START;
	// something to do...
	for (int i = 0; i < rand_num; i++) {
		d = rand() % RAND_MAX;
		U = (double)d / RAND_MAX;
		result_x[i] = secant(U);
	}

	CHECK_TIME_END(resultTime);

	fprintf(fp_w, "%d\n", rand_num);
	for (int i = 0; i < rand_num; i++)
		fprintf(fp_w, "%.15lf\n", result_x[i]);

	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);
}

// HOMEWORK
//	Newton-Raphson
void program2_2_c()
{
	__int64 start, freq, end;
	float resultTime = 0;
	FILE* fp_r, * fp_w;
	int d, rand_num;
	double U;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("HW2_2_c_result.txt", "w");

	printf("\nHW2_2_c. pdf를 따르는 난수 개수 입력(newton-raphson) : ");
	scanf("%d", &rand_num);
	fscanf(fp_r, "%d %lf", &num, &h);
	n = num - 1;

	arr_x = new double[num];
	arr_y = new double[num];
	result_x = new double[rand_num];
	for (int i = 0; i < num; i++)
		fscanf(fp_r, "%lf %lf", &arr_x[i], &arr_y[i]);
	CHECK_TIME_START;
	// something to do...
	for (int i = 0; i < rand_num; i++) {
		d = rand() % RAND_MAX;
		U = (double)d / RAND_MAX;
		result_x[i] = newton_raphson(U);
		//fprintf(fp_w, "U = %.15lf\n, ", U);
	}

	CHECK_TIME_END(resultTime);

	fprintf(fp_w, "%d\n", rand_num);
	for (int i = 0; i < rand_num; i++)
		fprintf(fp_w, "%.15lf\n", result_x[i]);

	printf("The program2_2_c run time is %f(ms)..\n", resultTime * 1000.0);
}

// HOMEWORK
void program2_3() {
	FILE* fp_r = fopen("random_event_table.txt", "r");
	FILE* fp_w = fopen("histogram.txt", "w");
	int rand_num;

	if (!fp_r || !fp_w) return;
	
	printf("\nHW2_3. histogram.txt 생성\n");
	cnt = new int[n];
	for (int i = 0; i < n; i++)
		cnt[i] = 0;

	fscanf(fp_r, "%d", &rand_num);

	for (int i = 0; i < rand_num; i++) {
		double d;
		int idx;

		fscanf(fp_r, "%lf", &d);
		idx = (int)(d / h);

		//printf("%.15lf, %d\n", d, idx);
		cnt[idx] += 1;
	}

	for (int i = 0; i < n; i++)
		fprintf(fp_w, "구간 [%.15lf ~ %.15lf]에 생성된 난수 개수 : %d\n", h * i, h * (i + 1), cnt[i]);

	fclose(fp_r);
	fclose(fp_w);
	delete[] cnt;
}

// HOMEWORK (Secant)
void program2_3_b() {
	FILE* fp_r = fopen("HW2_2_b_result.txt", "r");
	FILE* fp_w = fopen("histogram_b.txt", "w");
	int rand_num;

	if (!fp_r || !fp_w) return;

	printf("\nHW2_3. histogram_b.txt 생성\n");
	cnt = new int[n];
	for (int i = 0; i < n; i++)
		cnt[i] = 0;

	fscanf(fp_r, "%d", &rand_num);

	for (int i = 0; i < rand_num; i++) {
		double d;
		int idx;

		fscanf(fp_r, "%lf", &d);
		idx = (int)(d / h);

		//printf("%.15lf, %d\n", d, idx);
		if (0 <= idx && idx < n)
			cnt[idx] += 1;
	}

	for (int i = 0; i < n; i++)
		fprintf(fp_w, "구간 [%.15lf ~ %.15lf]에 생성된 난수 개수 : %d\n", h * i, h * (i + 1), cnt[i]);

	fclose(fp_r);
	fclose(fp_w);
	delete[] cnt;
}

// HOMEWORK (Newton-Raphson)
void program2_3_c() {
	FILE* fp_r = fopen("HW2_2_c_result.txt", "r");
	FILE* fp_w = fopen("histogram_c.txt", "w");
	int rand_num;

	if (!fp_r || !fp_w) return;

	printf("\nHW2_3. histogram_c.txt 생성\n");
	cnt = new int[n];
	for (int i = 0; i < n; i++)
		cnt[i] = 0;

	fscanf(fp_r, "%d", &rand_num);

	for (int i = 0; i < rand_num; i++) {
		double d;
		int idx;

		fscanf(fp_r, "%lf", &d);
		idx = (int)(d / h);

		//printf("%.15lf, %d\n", d, idx);
		if(0 <= idx && idx < n)
			cnt[idx] += 1;
	}

	for (int i = 0; i < n; i++)
		fprintf(fp_w, "구간 [%.15lf ~ %.15lf]에 생성된 난수 개수 : %d\n", h * i, h * (i + 1), cnt[i]);

	fclose(fp_r);
	fclose(fp_w);
	delete[] cnt;
}