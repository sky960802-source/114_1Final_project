/*
sixth edition of the 114_1 final project
新增格線 
*/

//處理使用者輸入與主程式流程
#include "poly.h"

int main() {
    Poly f, res;
    initPoly(&f);
    char input[100];
    int choice;

    printf("請分別輸入多項式的每一個項 (如 1.3x^5)：\n輸入 Done 結束：\n");
    while (1) {
        printf(">> ");
        scanf("%s", input);
        if (strcasecmp(input, "Done") == 0) break;
        
        char *ptr = strstr(input, "x^");
        if (ptr) {
            *ptr = '\0';
            addTerm(&f, atof(input), atof(ptr + 2));
        } else {
            addTerm(&f, atof(input), 0);
        }
    }

    printf("\n1. 微分\n2. 積分\n>> ");
    scanf("%d", &choice);

    if (choice == 1) {
        res = derivative(f);
        printf("微分結果: ");
        printPoly(res);
        savePolyGraph(res, "derivative_plot.bmp");
    } else {
        double C;
        printf("輸入積分常數 C: ");
        scanf("%lf", &C);
        res = integral(f, C);
        printf("積分結果: ");
        printPoly(res);
        savePolyGraph(res, "integral_plot.bmp");
    }

    return 0;
}
