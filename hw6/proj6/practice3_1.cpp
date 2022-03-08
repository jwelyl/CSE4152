#include "my_solver.h"


void practice3_1() {
    int i, ia, j, n, l[4];
    //float a[16], b[4], x[4], s[4];
    float a[16] = { 1, -2, 3, 4, 3, 1, -2, -1, -4, -1, 2, -3, 5, 3, -1, -1 };
    float b[4] = { 1, 4, 5, 2 };
    float x[4], s[4];
    printf("practice1\n\n");

    printf("미지수 4개인 연립 일차 방정식\n\n");
    printf("f1(x1, x2, x3, x4) =    x1 + 3*x2 - 4*x3 + 5*x4 - 1 = 0\n");
    printf("f2(x1, x2, x3, x4) = -2*x1 +   x2 -   x3 + 3*x4 - 4 = 0\n");
    printf("f3(x1, x2, x3, x4) =  3*x1 - 2*x2 + 2*x3 -   x4 - 5 = 0\n");
    printf("f4(x1, x2, x3, x4) =  4*x1 -   x2 - 3*x3 -   x4 - 2 = 0\n");

    printf("\n알려진 근\n\n");
    printf("x1 = -0.142857\nx2 = -4.285714\nx3 = -0.285714\nx4 =  2.571428\n");

    /********************************/
    n = 4;
    ia = 4;

    gespp_(&n, a, &ia, l, s);
    solve_(&n, a, &ia, l, b, x);
    printf("\n실제로 구한 근 (x1, x2, x3, x4) = (%f, %f, %f, %f)\n", x[0], x[1], x[2], x[3]);
    printf("f1(x1, x2, x3, x4) = %f\n", x[0] + 3 * x[1] - 4 * x[2] + 5 * x[3] - 1);
    printf("f2(x1, x2, x3, x4) = %f\n", -2 * x[0] + x[1] - x[2] + 3 * x[3] - 4);
    printf("f3(x1, x2, x3, x4) = %f\n", 3 * x[0] -2 * x[1] + 2 * x[2] - x[3] - 5);
    printf("f4(x1, x2, x3, x4) = %f\n", 4 * x[0] - x[1] - 3 * x[2] + - x[3] - 2);

    /********************************/
}
