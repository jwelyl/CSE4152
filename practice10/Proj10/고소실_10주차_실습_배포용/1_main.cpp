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


#define MATDIM 1024
//#define MATDIM	16
double *pMatA, *pMatB, *pMatC;
void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize);
void PrintMatrix(const char* str, double* pDestMatrix, int MatSize);

void init_MatMat(void);

void main(void)
{
	init_MatMat();

	CHECK_TIME_START;
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_1 : %f ms\n", compute_time);
	//PrintMatrix("1 결과", pMatC, MATDIM);

	CHECK_TIME_START;
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_2 = %f ms\n", compute_time);
	//PrintMatrix("2 결과", pMatC, MATDIM);


	CHECK_TIME_START;
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_3 = %f ms\n", compute_time);
	//PrintMatrix("3 결과", pMatC, MATDIM);


	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_4 = %f ms\n", compute_time);
}

void MultiplySquareMatrices_1(double *pDestMatrix, double *pLeftMatrix ,double *pRightMatrix, int MatSize)
{
	int i, j, k;

	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize; k++) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightMatrix[k * MatSize + j];
			}
		}
	}
}

void MultiplySquareMatrices_2(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k;
	double* pRightTranspose = (double*)malloc(sizeof(double) * MatSize * MatSize);	//	원래 행렬을 유지해야 하므로 전치행렬 따로 선언;

	//	전치행렬에 원래 행렬 복사
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pRightTranspose[i * MatSize + j] = pRightMatrix[i * MatSize + j];
		}
	}

	//	복사한 행렬을 전치행렬로 변환
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			if (i < j) {
				double tmp = pRightTranspose[i * MatSize + j];
				pRightTranspose[i * MatSize + j] = pRightTranspose[j * MatSize + i];
				pRightTranspose[j * MatSize + i] = tmp;
			}
		}
	}

	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize; k++) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightTranspose[j * MatSize + k];
			}
		}
	}

	free(pRightTranspose);
}

//	m = 8
void MultiplySquareMatrices_3(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k;
	int m = 8;
	double* pRightTranspose = (double*)malloc(sizeof(double) * MatSize * MatSize);	//	원래 행렬을 유지해야 하므로 전치행렬 따로 선언;

	//	전치행렬에 원래 행렬 복사
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j += m) {
			pRightTranspose[i * MatSize + j] = pRightMatrix[i * MatSize + j];
			pRightTranspose[i * MatSize + j + 1] = pRightMatrix[i * MatSize + j + 1];
			pRightTranspose[i * MatSize + j + 2] = pRightMatrix[i * MatSize + j + 2];
			pRightTranspose[i * MatSize + j + 3] = pRightMatrix[i * MatSize + j + 3];
			pRightTranspose[i * MatSize + j + 4] = pRightMatrix[i * MatSize + j + 4];
			pRightTranspose[i * MatSize + j + 5] = pRightMatrix[i * MatSize + j + 5];
			pRightTranspose[i * MatSize + j + 6] = pRightMatrix[i * MatSize + j + 6];
			pRightTranspose[i * MatSize + j + 7] = pRightMatrix[i * MatSize + j + 7];
		}
	}

	//	복사한 행렬을 전치행렬로 변환
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			if (i < j) {
				double tmp = pRightTranspose[i * MatSize + j];
				pRightTranspose[i * MatSize + j] = pRightTranspose[j * MatSize + i];
				pRightTranspose[j * MatSize + i] = tmp;
			}
		}
	}

	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize; k += m) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightTranspose[j * MatSize + k];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 1] * pRightTranspose[j * MatSize + k + 1];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 2] * pRightTranspose[j * MatSize + k + 2];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 3] * pRightTranspose[j * MatSize + k + 3];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 4] * pRightTranspose[j * MatSize + k + 4];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 5] * pRightTranspose[j * MatSize + k + 5];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 6] * pRightTranspose[j * MatSize + k + 6];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 7] * pRightTranspose[j * MatSize + k + 7];
			}
		}
	}

	free(pRightTranspose);
}

//	m = 16
void MultiplySquareMatrices_4(double *pDestMatrix, double *pLeftMatrix, double *pRightMatrix, int MatSize)
{
	int i, j, k;
	int m = 16;
	double* pRightTranspose = (double*)malloc(sizeof(double) * MatSize * MatSize);	//	원래 행렬을 유지해야 하므로 전치행렬 따로 선언;

	//	전치행렬에 원래 행렬 복사
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j += m) {
			pRightTranspose[i * MatSize + j] = pRightMatrix[i * MatSize + j];
			pRightTranspose[i * MatSize + j + 1] = pRightMatrix[i * MatSize + j + 1];
			pRightTranspose[i * MatSize + j + 2] = pRightMatrix[i * MatSize + j + 2];
			pRightTranspose[i * MatSize + j + 3] = pRightMatrix[i * MatSize + j + 3];
			pRightTranspose[i * MatSize + j + 4] = pRightMatrix[i * MatSize + j + 4];
			pRightTranspose[i * MatSize + j + 5] = pRightMatrix[i * MatSize + j + 5];
			pRightTranspose[i * MatSize + j + 6] = pRightMatrix[i * MatSize + j + 6];
			pRightTranspose[i * MatSize + j + 7] = pRightMatrix[i * MatSize + j + 7];
			pRightTranspose[i * MatSize + j + 8] = pRightMatrix[i * MatSize + j + 8];
			pRightTranspose[i * MatSize + j + 9] = pRightMatrix[i * MatSize + j + 9];
			pRightTranspose[i * MatSize + j + 10] = pRightMatrix[i * MatSize + j + 10];
			pRightTranspose[i * MatSize + j + 11] = pRightMatrix[i * MatSize + j + 11];
			pRightTranspose[i * MatSize + j + 12] = pRightMatrix[i * MatSize + j + 12];
			pRightTranspose[i * MatSize + j + 13] = pRightMatrix[i * MatSize + j + 13];
			pRightTranspose[i * MatSize + j + 14] = pRightMatrix[i * MatSize + j + 14];
			pRightTranspose[i * MatSize + j + 15] = pRightMatrix[i * MatSize + j + 15];
		}
	}

	//	복사한 행렬을 전치행렬로 변환
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			if (i < j) {
				double tmp = pRightTranspose[i * MatSize + j];
				pRightTranspose[i * MatSize + j] = pRightTranspose[j * MatSize + i];
				pRightTranspose[j * MatSize + i] = tmp;
			}
		}
	}

	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize; k += m) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightTranspose[j * MatSize + k];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 1] * pRightTranspose[j * MatSize + k + 1];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 2] * pRightTranspose[j * MatSize + k + 2];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 3] * pRightTranspose[j * MatSize + k + 3];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 4] * pRightTranspose[j * MatSize + k + 4];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 5] * pRightTranspose[j * MatSize + k + 5];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 6] * pRightTranspose[j * MatSize + k + 6];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 7] * pRightTranspose[j * MatSize + k + 7];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 8] * pRightTranspose[j * MatSize + k + 8];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 9] * pRightTranspose[j * MatSize + k + 9];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 10] * pRightTranspose[j * MatSize + k + 10];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 11] * pRightTranspose[j * MatSize + k + 11];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 12] * pRightTranspose[j * MatSize + k + 12];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 13] * pRightTranspose[j * MatSize + k + 13];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 14] * pRightTranspose[j * MatSize + k + 14];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 15] * pRightTranspose[j * MatSize + k + 15];
			}
		}
	}

	free(pRightTranspose);
}

void init_MatMat(void)
{
	double *ptr;
	pMatA = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatB = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	pMatC = (double *)malloc(sizeof(double)*MATDIM*MATDIM);
	srand((unsigned)time(NULL));
	ptr = pMatA;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
	ptr = pMatB;
	for (int i = 0; i < MATDIM*MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
}
