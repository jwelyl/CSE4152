#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1

#define	IN
#define OUT
#define INOUT
#define EPSILON    0.01

#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

void create_device_timer()
{
    CUDA_CALL(cudaEventCreate(&cuda_timer_start));
    CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
    CUDA_CALL(cudaEventDestroy(cuda_timer_start));
    CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
    cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
    TIMER_T ms;
    cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
    cudaEventSynchronize(cuda_timer_stop);

    cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
    return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

#define N_EQUATIONS (1 << 26)												// 방정식 개수
//#define N_EQUATIONS (1 << 6)

#define BLOCK_SIZE (1 << 6)													// CUDA 커널 thread block 사이즈

//#define BLOCK_SIZE (1 << 4)

#define BLOCK_WIDTH (1 << 3)
//#define BLOCK_WIDTH (1 << 2)

#define BLOCK_HEIGHT (BLOCK_SIZE / BLOCK_WIDTH)

TIMER_T compute_time = 0;
TIMER_T device_time = 0;

int N;
float* h_A;             //  2차항 계수
float* h_B;             //  1차항 계수
float* h_C;             //  상수항
float* h_X0;            //  두 실근 중 작은 근 x0 (CPU 계산값)
float* h_X1;            //  두 실근 중 큰 근 x1 (")
float* h_FX0;           //  ax0^2 + bx0 + c의 값 (")
float* h_FX1;           //  ax1^2 + bx1 + c의 값 (")
float* d_X0_GPU;        //  두 실근 중 작은 근 x0 (GPU 계산값)
float* d_X1_GPU;        //  두 실근 중 큰 근 x1 (")
float* d_FX0_GPU;       //  ax0^2 + bx0 + c의 값 (")
float* d_FX1_GPU;       //  ax1^2 + bx1 + c의 값 (")


cudaError_t find_roots_GPU(IN float* A_CPU, IN float* B_CPU, IN float* C_CPU,
    OUT float* X0_GPU, OUT float* X1_GPU, OUT float* FX0_GPU, OUT float* FX1_GPU, int n);

__global__ void find_roots_Kernel(IN float* d_A, IN float* d_B, IN float* d_C,
    OUT float* d_X0, OUT float* d_X1, OUT float* d_FX0, OUT float* d_FX1, int n) {
    /*Todo*/
    //printf("GPU Kernel code!!\n");

    int row = blockDim.y * blockIdx.y + threadIdx.y;
    int col = blockDim.x * blockIdx.x + threadIdx.x;
    int id = gridDim.x * blockDim.x * row + col;

    float a, b, c, d, x0, x1, tmp;

    a = d_A[id];    b = d_B[id];    c = d_C[id];
    d = sqrtf(b * b - 4.0f * a * c);
    tmp = 1.0f / (2.0 * a);
    d_X0[id] = x0 = (-b - d) * tmp;
    d_X1[id] = x1 = (-b + d) * tmp;
    d_FX0[id] = (a * x0 + b) * x0 + c;
    d_FX1[id] = (a * x1 + b) * x1 + c;
}

void find_roots_CPU(IN float* A_CPU, IN float* B_CPU, IN float* C_CPU, 
    OUT float* X0_CPU, OUT float* X1_CPU, OUT float* FX0_CPU, OUT float* FX1_CPU, int n) {
    int i;

    float a, b, c, d, x0, x1, tmp;

    //printf("CPU roots\n");
    for (i = 0; i < n; i++) {
        a = A_CPU[i];   b = B_CPU[i];   c = C_CPU[i];
        d = sqrtf(b * b - 4.0f * a * c);
        tmp = 1.0f / (2.0 * a);
        X0_CPU[i] = x0 = (-b - d) * tmp;
        X1_CPU[i] = x1 = (-b + d) * tmp;

        //printf("i = %d, x0 = %f, x1 = %f\n", i, X0_CPU[i], X1_CPU[i]);

        FX0_CPU[i] = (a * x0 + b) * x0 + c;
        FX1_CPU[i] = (a * x1 + b) * x1 + c;
    }
}

void read_bin_file() {
    printf("***Binary File Read Start!!\n");
    FILE* fp_a = fopen("A.bin", "rb");
    FILE* fp_b = fopen("B.bin", "rb");
    FILE* fp_c = fopen("C.bin", "rb");
    int tmp;
    fread(&N, sizeof(int), 1, fp_a);
    tmp = N;
    fread(&N, sizeof(int), 1, fp_b);
    if (tmp != N) goto FILE_ERROR;
    fread(&N, sizeof(int), 1, fp_c);
    if (tmp != N) goto FILE_ERROR;

    h_A = (float*)malloc(N * sizeof(float));
    h_B = (float*)malloc(N * sizeof(float));
    h_C = (float*)malloc(N * sizeof(float));
    h_X0 = (float*)malloc(N * sizeof(float));
    h_X1 = (float*)malloc(N * sizeof(float)); 
    h_FX0 = (float*)malloc(N * sizeof(float));
    h_FX1 = (float*)malloc(N * sizeof(float));
    d_X0_GPU = (float*)malloc(N * sizeof(float));
    d_X1_GPU = (float*)malloc(N * sizeof(float));
    d_FX0_GPU = (float*)malloc(N * sizeof(float));
    d_FX1_GPU = (float*)malloc(N * sizeof(float));

    fread(h_A, sizeof(float), N, fp_a);
    fread(h_B, sizeof(float), N, fp_b);
    fread(h_C, sizeof(float), N, fp_c);

    //for (int i = 0; i < N; i++) {
    //    printf("equation %d : %fx^2 + %fx + %f = 0\n", i, h_A[i], h_B[i], h_C[i]);
    //}

    fclose(fp_a);
    fclose(fp_b);
    fclose(fp_c);
    printf("***Binary File Read End!!\n\n");
    return;

FILE_ERROR:
    fprintf(stderr, "init_bin_file Error!\n");
    exit(-1);
}

void write_bin_file() {
    printf("\n***Binary File Write Start!!\n");
    FILE* fp_x0 = fopen("X0.bin", "wb");
    FILE* fp_x1 = fopen("X1.bin", "wb");
    FILE* fp_fx0 = fopen("FX0.bin", "wb");
    FILE* fp_fx1 = fopen("FX1.bin", "wb");
    
    fwrite(&N, sizeof(int), 1, fp_x0);
    fwrite(&N, sizeof(int), 1, fp_x1);
    fwrite(&N, sizeof(int), 1, fp_fx0);
    fwrite(&N, sizeof(int), 1, fp_fx1);
   
    fwrite(d_X0_GPU, sizeof(float), N, fp_x0);
    fwrite(d_X1_GPU, sizeof(float), N, fp_x1);
    fwrite(d_FX0_GPU, sizeof(float), N, fp_fx0);
    fwrite(d_FX1_GPU, sizeof(float), N, fp_fx1);

    fclose(fp_x0);
    fclose(fp_x1);
    fclose(fp_fx0);
    fclose(fp_fx1);

    printf("***Binary File Write End!!\n\n");
    return;
}

void init_bin_file(IN int n) {
    printf("***Binary File Create Start!!\n");
    srand((unsigned)time(NULL));
    FILE* fp_a = fopen("A.bin", "wb");
    FILE* fp_b = fopen("B.bin", "wb");
    FILE* fp_c = fopen("C.bin", "wb");
    fwrite(&n, sizeof(int), 1, fp_a);
    fwrite(&n, sizeof(int), 1, fp_b);
    fwrite(&n, sizeof(int), 1, fp_c);

    int i;

    for (i = 0; i < n; i++) {
        float input_a, input_b, input_c;

        do {
            //printf("do_while! %d\n", i);
            input_a = 200.0f * ((float)rand() / RAND_MAX) - 100.0f;
           // printf("input_a = %f\n", input_a);
            input_b = 200.0f * ((float)rand() / RAND_MAX) - 100.0f;
           // printf("input_b = %f\n", input_b);
            input_c = 200.0f * ((float)rand() / RAND_MAX) - 100.0f;
           // printf("input_c = %f\n", input_c);
        } while ((input_b * input_b - 4.0f * input_a * input_c) <= 0.0f);
        
        //printf("%d번째 방정식 : %fx^2 + %fx + %f = 0\n", i, input_a, input_b, input_c);

        fwrite(&input_a, sizeof(float), 1, fp_a);
        fwrite(&input_b, sizeof(float), 1, fp_b);
        fwrite(&input_c, sizeof(float), 1, fp_c);
    }

    fclose(fp_a);
    fclose(fp_b);
    fclose(fp_c);
    printf("***Binary File Create End!!\n\n");
}

int main()
{
    int i;
    init_bin_file(N_EQUATIONS);
    read_bin_file();

    CHECK_TIME_START;
    find_roots_CPU(h_A, h_B, h_C, h_X0, h_X1, h_FX0, h_FX1, N);
    CHECK_TIME_END(compute_time);
    find_roots_GPU(h_A, h_B, h_C, d_X0_GPU, d_X1_GPU, d_FX0_GPU, d_FX1_GPU, N);
    /*
    printf("\nCPU 결과\n");
    for (i = 0; i < N; i++) {
        printf("%d번째 방정식 CPU 근 x0 = %f, x1 = %f\n", i, h_X0[i], h_X1[i]);
        printf("%d번째 방정식 CPU 대입값 f(x0) = %f, f(x1) = %f\n\n", i, h_FX0[i], h_FX1[i]);
    }
    printf("\nGPU 결과\n");
    for (i = 0; i < N; i++) {
        printf("%d번째 방정식 GPU 근 x0 = %f, x1 = %f\n", i, d_X0_GPU[i], d_X1_GPU[i]);
        printf("%d번째 방정식 GPU 대입값 f(x0) = %f, f(x1) = %f\n\n", i, d_FX0_GPU[i], d_FX1_GPU[i]);
    }
    printf("\n");
    */
    for (i = 0; i < N; i++) {
        if (fabs(h_X0[i] - d_X0_GPU[i]) > EPSILON) {
            printf("In i = %d\n", i);
            printf("x0 = %f : CPU, \t x0 = %f : GPU\n", h_X0[i], d_X0_GPU[i]);
            break;
        }
        if (fabs(h_X1[i] - d_X1_GPU[i]) > EPSILON) {
            printf("In i = %d\n", i);
            printf("x1 = %f : CPU, \t x1 = %f : GPU\n", h_X1[i], d_X1_GPU[i]);
            break;
        }
        /*
        if (fabs(h_FX0[i] - d_FX0_GPU[i]) > EPSILON) {
            printf("In i = %d\n", i);
            printf("FX0 = %f : CPU, \t FX0 = %f : GPU\n", h_FX0[i], d_FX0_GPU[i]);
            break;
        }
        if (fabs(h_FX1[i] - d_FX1_GPU[i]) > EPSILON) {
            printf("In i = %d\n", i);
            printf("FX1 = %f : CPU, \t FX1 = %f : GPU\n", h_FX1[i], d_FX1_GPU[i]);
            break;
        }
        */
    }
    if (i == N)
        printf("***Kernel execution Success!!\n\n");

    printf("***CPU compute time : %.3f ms\n", compute_time);
    printf("***GPU compute time : %.3f ms\n", device_time);

    write_bin_file();

    //printf("\n***Binary File Write Start!!\n");
    //printf("***Binary File Write End!!\n");

    free(h_A);
    free(h_B);
    free(h_C);
    free(h_X0);
    free(h_X1);
    free(h_FX0);
    free(h_FX1);
    free(d_X0_GPU);
    free(d_X1_GPU);
    free(d_FX0_GPU);
    free(d_FX1_GPU);

    return 0;
}

cudaError_t find_roots_GPU(IN float* A_CPU, IN float* B_CPU, IN float* C_CPU,
    OUT float* X0_GPU, OUT float* X1_GPU, OUT float* FX0_GPU, OUT float* FX1_GPU, int n) {
    cudaError_t cudaStatus;
    /*Todo*/
    //printf("Sum_n_elements_GPU!\n");

    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    cudaDeviceProp deviceProp;
    CUDA_CALL(cudaGetDeviceProperties(&deviceProp, 0));

    float* d_A, * d_B, * d_C;
    float* d_X0, * d_X1, * d_FX0, * d_FX1;

    size_t size = n * sizeof(int);

    CUDA_CALL(cudaMalloc(&d_A, size));
    CUDA_CALL(cudaMemcpy(d_A, A_CPU, size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMalloc(&d_B, size));
    CUDA_CALL(cudaMemcpy(d_B, B_CPU, size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMalloc(&d_C, size));
    CUDA_CALL(cudaMemcpy(d_C, C_CPU, size, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMalloc(&d_X0, size));
    CUDA_CALL(cudaMalloc(&d_X1, size));
    CUDA_CALL(cudaMalloc(&d_FX0, size));
    CUDA_CALL(cudaMalloc(&d_FX1, size));

    dim3 dimBlock(BLOCK_WIDTH, BLOCK_HEIGHT);
    dim3 dimGrid((int)sqrt(n) / dimBlock.x, (int)sqrt(n) / dimBlock.y);

    CHECK_TIME_INIT_GPU();
    CHECK_TIME_START_GPU();
    find_roots_Kernel << < dimGrid, dimBlock >> > (d_A, d_B, d_C, d_X0, d_X1, d_FX0, d_FX1, n);
    CHECK_TIME_END_GPU(device_time);
    CHECK_TIME_DEST_GPU();

    CUDA_CALL(cudaDeviceSynchronize());
    CUDA_CALL(cudaMemcpy(X0_GPU, d_X0, size, cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(X1_GPU, d_X1, size, cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(FX0_GPU, d_FX0, size, cudaMemcpyDeviceToHost));
    CUDA_CALL(cudaMemcpy(FX1_GPU, d_FX1, size, cudaMemcpyDeviceToHost));


Error:
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
    cudaFree(d_X0);
    cudaFree(d_X1);
    cudaFree(d_FX0);
    cudaFree(d_FX1);

    return cudaStatus;
}
