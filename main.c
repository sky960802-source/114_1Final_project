//處理使用者輸入與主程式流程
#include "poly.h"

int main() {
    Poly f;
    initPoly(&f);
    char input[100];
    int choice;

    printf("請分別輸入多項式的每一個項：\n輸入 Done 來結束輸入：\n\n");

    while (1) {
        printf(">>\n");
        scanf("%s", input);

        if (strcasecmp(input, "Done") == 0) break;//比較 Done 與輸入字串，若一樣則 break 

        char *ptr = strstr(input, "x^");//在 input 中尋找 x^ 字串 
        if (ptr != NULL) {
            *ptr = '\0'; 
            double c = atof(input);//將 input 轉換為 float 形式 
            double e = atof(ptr + 2);
            addTerm(&f, c, e);
        } else {
            // 處理純常數項 (如 5)
            double c = atof(input);
            addTerm(&f, c, 0);
        }
    }

    printf("\n你要微分還是積分？\n");
    printf("微分輸入 1\n積分輸入 2\n\n>> ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nf(x) = ");
        printPoly(f);
        Poly df = derivative(f);
        printf("f'(x) = ");
        printPoly(df);
    } else if (choice == 2) {
        double C;
        printf("請輸入積分常數 C: ");
        scanf("%lf", &C);
        printf("\nf(x) = ");
        printPoly(f);
        Poly F = integral(f, C);
        printf("∫f(x)dx = ");
        printPoly(F);
    }

    return 0;
}
