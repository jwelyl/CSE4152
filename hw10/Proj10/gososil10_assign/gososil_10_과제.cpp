#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 100000
double *hw1_x, hw1_E, hw1_var1, hw1_var2;
double hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];
//float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];

/* hw1 */
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2_naive();
void hw2_safe();
double hw2_verify(double x);

/* hw3 */

void main(void)
{
	srand((unsigned)time(NULL));

	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(1);
	hw1_calc_e();

	//
	printf("hw1_calc_e = %.15f\n", hw1_E);
	//

	CHECK_TIME_START;
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);


	CHECK_TIME_START;
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");
	
	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%lf %lf %lf", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	hw2_safe();
	printf("naive result    : %.15lf, %.15lf\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	printf("advanced result : %.15lf, %.15lf\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15lf, %.15lf\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15lf, %.15lf\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");

	/* hw3 */
}

void init_hw1(int fixed)
{
	double *ptr;
	hw1_x = (double *)malloc(sizeof(double)*HW1_N);

	//
	//FILE* fp = fopen("random_data.txt", "w");
	//

	if (fixed)
	{
		double tmp = HW1_N;
		for (int i = 0; i < HW1_N; i++)
		{
			if( i & 1) tmp -= 0.0001;
			hw1_x[i] = tmp;
			//
			//fprintf(fp, "%lf\n", hw1_x[i]);
			//
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		ptr = hw1_x;
		for (int i = 0; i < HW1_N; i++) {
			*ptr++ = ((double)rand() / (double)RAND_MAX) * 2;
			//
			//fprintf(fp, "%lf\n", hw1_x[i]);
			//
		}
	}

	//fclose(fp);
}

void hw1_calc_e()
{
	hw1_E = 0;
	for (int i = 0; i < HW1_N; i++) { 
		hw1_E += hw1_x[i];
	}
	hw1_E = hw1_E / HW1_N;
}
void hw1_calc_var1()
{
	hw1_var1 = 0;
	for (int i = 0; i < HW1_N; i++) {
		//hw1_var1 += ((hw1_x[i] - hw1_E) * (hw1_x[i] - hw1_E));
		hw1_var1 += pow(hw1_x[i] - hw1_E, 2);
	}
	hw1_var1 = hw1_var1 / (HW1_N - 1);
}
void hw1_calc_var2()
{
	hw1_var2 = 0;
	for (int i = 0; i < HW1_N; i++) {
		//hw1_var2 += (hw1_x[i] * hw1_x[i]);
		hw1_var2 += pow(hw1_x[i], 2);
	}
	hw1_var2 = hw1_var2 * HW1_N;
	//hw1_var2 = hw1_var2 - (HW1_N * HW1_N * hw1_E * hw1_E);
	hw1_var2 = hw1_var2 - pow((HW1_N * hw1_E), 2);
	hw1_var2 = hw1_var2 / (HW1_N * (HW1_N - 1));
}

/* hw2 */
void hw2_naive()
{
	hw2_naive_ans[0] = (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	hw2_naive_ans[1] = (-hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
}
void hw2_safe()
{
	hw2_pre_ans[0] = (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	hw2_pre_ans[1] = (2 * hw2_c) / (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c));
}
double hw2_verify(double x)
{
	return hw2_a * x * x + hw2_b*x + hw2_c;
}
