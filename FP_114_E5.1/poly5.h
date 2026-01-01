#ifndef POLY5_H
#define POLY5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define IMG_WIDTH 800
#define IMG_HEIGHT 600
#define MAX_TERMS 100

typedef struct {
    double coef;
    double exp;
} Term;

typedef struct {
    Term terms[MAX_TERMS];
    int n;
} Poly;

// 初始化
static void initPoly(Poly *p) { p->n = 0; }

// 新增項
static void addTerm(Poly *p, double coef, double exp) {
    if (p->n < MAX_TERMS) {
        p->terms[p->n].coef = coef;
        p->terms[p->n].exp = exp;
        p->n++;
    }
}

// 帶入 x 求值
static double evaluate(Poly p, double x) {
    double sum = 0;
    for (int i = 0; i < p.n; i++) sum += p.terms[i].coef * pow(x, p.terms[i].exp);
    return sum;
}

// 微分
static Poly derivative(Poly p) {
    Poly d; initPoly(&d);
    for (int i = 0; i < p.n; i++) 
        if (p.terms[i].exp != 0) 
            addTerm(&d, p.terms[i].coef * p.terms[i].exp, p.terms[i].exp - 1);
    return d;
}

// 積分
static Poly integral(Poly p, double C) {
    Poly in; initPoly(&in);
    for (int i = 0; i < p.n; i++) 
        addTerm(&in, p.terms[i].coef / (p.terms[i].exp + 1), p.terms[i].exp + 1);
    if (C != 0) addTerm(&in, C, 0);
    return in;
}

// 印出多項式
static void printPoly(Poly p) {
    if (p.n == 0) { printf("0\n"); return; }
    for (int i = 0; i < p.n; i++) {
        double c = p.terms[i].coef; double e = p.terms[i].exp;
        if (i > 0 && c > 0) printf(" + "); else if (c < 0) printf(" - ");
        if (e == 0) printf("%.2f", fabs(c));
        else if (e == 1) printf("%.2fx", fabs(c));
        else printf("%.2fx^%.2f", fabs(c), e);
    }
    printf("\n");
}

// 尋找數值點 (order=1 找極值, order=2 找反曲點)
static Poly findPoints(Poly p, int order) {
    Poly res; initPoly(&res);
    Poly target = (order == 1) ? derivative(p) : derivative(derivative(p));
    
    // 掃描區間 -10 到 10
    for (double x = -10.0; x <= 10.0; x += 0.01) {
        if (fabs(evaluate(target, x)) < 0.05) { // 誤差範圍內視為 0
            // 過濾掉太靠近的點，避免重複
            if (res.n == 0 || fabs(res.terms[res.n-1].coef - x) > 0.5) {
                double val = (fabs(x) < 0.05) ? 0.0 : x; // 修正 -0.00
                addTerm(&res, val, 0);
            }
        }
    }
    return res;
}

// 繪圖核心函式
static void savePolyGraph(Poly p, const char* filename, Poly cp, Poly ip) {
    printf("\n[Debug] Generating Graph: %s\n", filename);

    // 1. 初始化背景為白色
    unsigned char *img = (unsigned char *)calloc(IMG_WIDTH * IMG_HEIGHT * 3, 1);
    if (!img) { printf("[Error] Memory fail.\n"); return; }
    memset(img, 255, IMG_WIDTH * IMG_HEIGHT * 3);

    // 2. 畫網格 (Grid)
    for (int g = -10; g <= 10; g++) {
        int gx = (int)((g + 10) * IMG_WIDTH / 20.0);
        int gy = (int)((-g + 10) * IMG_HEIGHT / 20.0);
        unsigned char c = (g == 0) ? 0 : 220; // 軸=黑, 線=灰

        if (gx >= 0 && gx < IMG_WIDTH) 
            for (int y = 0; y < IMG_HEIGHT; y++) {
                int idx = (y * IMG_WIDTH + gx) * 3;
                img[idx] = img[idx+1] = img[idx+2] = c;
            }
        if (gy >= 0 && gy < IMG_HEIGHT)
            for (int x = 0; x < IMG_WIDTH; x++) {
                int idx = (gy * IMG_WIDTH + x) * 3;
                img[idx] = img[idx+1] = img[idx+2] = c;
            }
    }

    // 3. 畫函數曲線 (紅色 Red)
    for (double x = -10.0; x <= 10.0; x += 0.001) {
        double y = evaluate(p, x);
        int px = (int)((x + 10) * IMG_WIDTH / 20.0);
        int py = (int)((-y + 10) * IMG_HEIGHT / 20.0);
        if (px >= 0 && px < IMG_WIDTH && py >= 0 && py < IMG_HEIGHT) {
            int idx = (py * IMG_WIDTH + px) * 3;
            img[idx] = 0; img[idx+1] = 0; img[idx+2] = 255; // BGR
        }
    }

    // 4. 畫臨界點 (藍色 Blue)
    for (int i = 0; i < cp.n; i++) {
        double val = cp.terms[i].coef;
        int px = (int)((val + 10) * IMG_WIDTH / 20.0);
        int py = (int)((-evaluate(p, val) + 10) * IMG_HEIGHT / 20.0);
        for(int dy=-3; dy<=3; dy++) for(int dx=-3; dx<=3; dx++) {
            int nx=px+dx, ny=py+dy;
            if(nx>=0 && nx<IMG_WIDTH && ny>=0 && ny<IMG_HEIGHT) {
                int idx = (ny*IMG_WIDTH + nx)*3;
                img[idx]=255; img[idx+1]=0; img[idx+2]=0;
            }
        }
    }

    // 5. 畫反曲點 (綠色 Green)
    for (int i = 0; i < ip.n; i++) {
        double val = ip.terms[i].coef;
        int px = (int)((val + 10) * IMG_WIDTH / 20.0);
        int py = (int)((-evaluate(p, val) + 10) * IMG_HEIGHT / 20.0);
        for(int dy=-3; dy<=3; dy++) for(int dx=-3; dx<=3; dx++) {
            int nx=px+dx, ny=py+dy;
            if(nx>=0 && nx<IMG_WIDTH && ny>=0 && ny<IMG_HEIGHT) {
                int idx = (ny*IMG_WIDTH + nx)*3;
                img[idx]=0; img[idx+1]=200; img[idx+2]=0;
            }
        }
    }

    // 6. 寫入 BMP 檔案
    FILE *f = fopen(filename, "wb");
    if(f) {
        unsigned char fH[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
        unsigned char iH[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
        int sz = 54 + 3 * IMG_WIDTH * IMG_HEIGHT;
        fH[2]=(unsigned char)sz; fH[3]=(unsigned char)(sz>>8); fH[4]=(unsigned char)(sz>>16); fH[5]=(unsigned char)(sz>>24);
        iH[4]=(unsigned char)IMG_WIDTH; iH[5]=(unsigned char)(IMG_WIDTH>>8);
        iH[8]=(unsigned char)IMG_HEIGHT; iH[9]=(unsigned char)(IMG_HEIGHT>>8);
        
        fwrite(fH, 1, 14, f);
        fwrite(iH, 1, 40, f);
        // BMP 由下往上寫入 (Bottom-up)
        for(int i = IMG_HEIGHT - 1; i >= 0; i--) {
            fwrite(&img[i * IMG_WIDTH * 3], 3, IMG_WIDTH, f);
        }
        fclose(f);
        printf("[System] Success! Image saved: %s\n", filename);
    } else {
        printf("[Error] Cannot write file. Check permissions.\n");
    }
    free(img);
}
#endif
