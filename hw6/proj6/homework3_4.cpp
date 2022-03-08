#include "my_solver.h"

void homework3_4() {
    int i, ia, j, n;

    int* l;
    float* a;
    float* b;
    float* x, *s, *result;
    float error = 0;

    FILE* fp_r = fopen("linear_system_3-4.txt", "r");
    FILE* fp_w = fopen("solution_3-4.txt", "w");

    printf("\nhomework4\n");
    if (fp_r == NULL) {
        printf("%s file open error...\n", "linear_system_3-4.txt");
        return;
    }
    if (fp_w == NULL) {
        printf("%s file open error...\n", "solution_3-4.txt");
        return;
    }

    fscanf(fp_r, "%d", &n);
    ia = n;
    
    l = (int*)malloc(sizeof(int) * n);
    a = (float*)malloc(sizeof(float) * (n * n));
    b = (float*)malloc(sizeof(float) * n);
    x = (float*)malloc(sizeof(float) * n);
    s = (float*)malloc(sizeof(float) * n);
    result = (float*)malloc(sizeof(float) * n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) 
            fscanf(fp_r, "%f", &a[i + n * j]);
    }
    
    for (int i = 0; i < n; i++)
        fscanf(fp_r, "%f", &b[i]);

    /*for (int i = 0; i < n * n; i++)
        printf("a[%d] = %lf\n", i, a[i]);

    for (int i = 0; i < n; i++)
        printf("b[%d] = %lf\n", i, b[i]);*/

    gespp_(&n, a, &ia, l, s);
    solve_(&n, a, &ia, l, b, x);
    
    //  출력 부분
    fprintf(fp_w, "%d\n", n);
    for (int i = 0; i < n; i++)
        fprintf(fp_w, "%.6f\n", x[i]);

    float tmp1 = 0, tmp2 = 0;

    //  ||b||
    for (int i = 0; i < n; i++)
        tmp2 += (b[i] * b[i]);
    tmp2 = sqrt(tmp2);

    for (int i = 0; i < n; i++) {
        result[i] = 0;
        //printf("result[%d] = %.6f\n", i, result[i]);

        for (int j = 0; j < n; j++) {
            //printf("(i, j) = (%d, %d)\n", i + n * j, j);
            result[i] += (a[i + n * j] * x[j]);
        }
        //printf("result[%d] = %.6f\n", i, result[i]);
        result[i] -= b[i];
        //printf("result[%d] = %.6f\n", i, result[i]);
    }

    //  ||Ax^ - b||
    for (int i = 0; i < n; i++)
        tmp1 += (result[i] * result[i]);
    tmp1 = sqrt(tmp1);

    error = tmp1 / tmp2;

    fprintf(fp_w, "%.6f\n", error);

    free(l);
    free(a);
    free(b);
    free(x);
    free(s);
    free(result);

    fclose(fp_r);
    fclose(fp_w);
}
