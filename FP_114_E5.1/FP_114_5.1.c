#include "poly5.h"

int main() {
    Poly f, res;
    Poly cp, ip; // 用於儲存點
    initPoly(&f);
    char input[100];
    int choice;

    printf("=== Polynomial Analysis Tool ===\n");
    printf("Examples: x^3, -3x^2, x, 5. Type 'Done' to finish.\n\n");

    // 輸入迴圈
    while (1) {
        printf("Input Term: ");
        scanf("%s", input);
        if (strcasecmp(input, "Done") == 0) break;
        
        char *ptr_pow = strstr(input, "x^");
        if (ptr_pow) {
            double c = (ptr_pow == input) ? 1.0 : (ptr_pow == input + 1 && input[0] == '-') ? -1.0 : atof(input);
            addTerm(&f, c, atof(ptr_pow + 2));
        } else if (strchr(input, 'x')) {
            double c = (strcmp(input, "x") == 0) ? 1.0 : (strcmp(input, "-x") == 0) ? -1.0 : atof(input);
            addTerm(&f, c, 1.0);
        } else {
            addTerm(&f, atof(input), 0);
        }
    }

    printf("\n1. Derivative Analysis\n2. Integral Analysis\nSelect: ");
    scanf("%d", &choice);

    if (choice == 1) {
        // 微分分析
        res = derivative(f);
        cp = findPoints(f, 1); // 找原函數的臨界點
        ip = findPoints(f, 2); // 找原函數的反曲點
        
        printf("\nResult f'(x) = "); printPoly(res);
        
        printf("Critical Points (Blue): ");
        if(cp.n == 0) printf("None");
        for(int i=0; i<cp.n; i++) printf("%.2f%s", cp.terms[i].coef, (i==cp.n-1?"":" , "));
        
        printf("\nInflection Points (Green): ");
        if(ip.n == 0) printf("None");
        for(int i=0; i<ip.n; i++) printf("%.2f%s", ip.terms[i].coef, (i==ip.n-1?"":" , "));
        
        savePolyGraph(f, "analysis_plot.bmp", cp, ip);

    } else {
        // 積分分析
        double C;
        printf("Enter Constant C: ");
        scanf("%lf", &C);
        res = integral(f, C);
        
        // 對積分後的函數做分析
        cp = findPoints(res, 1);
        ip = findPoints(res, 2);
        
        printf("\nResult F(x) = "); printPoly(res);
        savePolyGraph(res, "analysis_plot.bmp", cp, ip);
    }

    printf("\n\n------------------------------------------------\n");
    printf("Analysis Complete. Please check 'analysis_plot.bmp'.\n");
    printf("------------------------------------------------\n");
    
    system("pause");
    return 0;
}
