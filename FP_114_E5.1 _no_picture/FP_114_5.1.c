/*
Edition 5.1 of the  114-1 Final Project  
新增 critical numbers,critical numbers 的標示 
「、」號與 x 輸入優化

*/
//處理使用者輸入與主程式流程
#include "poly5.h"

int main() {
    Poly f, res;
    Poly cp, ip;
    initPoly(&f);
    char input[100];
    int choice;

    printf("=== 多項式微積分分析工具 ===\n");
    printf("輸入 f(x) :( x^3 )、( -3x^2 )、( x )、( 5 )。結束請打 Done\n\n");

    while (1) {
        printf("請輸入項: ");
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

    printf("\n1. 微分分析 (Derivative)\n2. 積分分析 (Integral)\n請選擇: ");
    scanf("%d", &choice);

    if (choice == 1) {
        res = derivative(f);
        cp = findPoints(f, 1);
        ip = findPoints(f, 2);
        printf("\n原始函數 f(x) = ");
        printPoly(f);
        printf("\n微分結果 f'(x) = "); 
		printPoly(res);
    } else {
        double C;
        printf("請輸入積分常數 C: ");
        scanf("%lf", &C);
        res = integral(f, C);
        cp = findPoints(res, 1); // 針對積分後的函數找極值
        ip = findPoints(res, 2); // 針對積分後的函數找反曲點
        printf("\n原始函數 f(x) = ");   // <--- 新增這行：顯示標題
        printPoly(f);
        printf("\n積分結果 ∫f(x) = "); 
		printPoly(res);
    }

    // 統一輸出分析數值
    //printf("\n\n臨界點 Critical Numbers (圖中藍色): x = ");
    if(cp.n == 0) printf(">>無臨界點 No Critical Numbers");
    for(int i=0; i<cp.n; i++) printf("\n\n臨界點 Critical Numbers (圖中藍色): x = %.2f%s"
		, cp.terms[i].coef, (i==cp.n-1?"":" , "));
    
    //printf("\n反曲點 Inflection Points (圖中綠色): x = ");
    if(ip.n == 0) printf("\n>>無反曲點 No Inflection Points");
    for(int i=0; i<ip.n; i++) 
	printf("\n反曲點 Inflection Points (圖中綠色): x = %.2f%s"
		, ip.terms[i].coef, (i==ip.n-1?"":" , "));
	printf("\n");

    savePolyGraph((choice == 1 ? f : res), "analysis.bmp", cp, ip);

    printf("\n\n------------------------------------------------\n");
    printf("分析完成！請查看同資料夾下的 'analysis_plot.bmp'。\n");
    printf("------------------------------------------------\n");
    
    system("pause");
    return 0;
}
