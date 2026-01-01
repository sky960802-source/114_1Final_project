//定義結構體與函式宣告
/*#ifndef POLY_H
#define POLY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TERMS 100
#define IMG_WIDTH 800
#define IMG_HEIGHT 600

typedef struct {
    double coef;
    double exp;
} Term;

typedef struct {
    Term terms[MAX_TERMS];
    int n;
} Poly;

void initPoly(Poly *p);
void addTerm(Poly *p, double coef, double exp);
Poly derivative(Poly p);
Poly integral(Poly p, double C);
void printPoly(Poly p);

// 新增：繪圖並存成 BMP 的函式
void savePolyGraph(Poly p, const char* filename);

#endif
*/

#ifndef POLY_H
#define POLY_H

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

// 使用 static 確保函式實作可以安全地放在 .h 檔中
static void initPoly(Poly *p) {
    p->n = 0;
}

static void addTerm(Poly *p, double coef, double exp) {
    if (p->n < MAX_TERMS) {
        p->terms[p->n].coef = coef;
        p->terms[p->n].exp = exp;
        p->n++;
    }
}

static Poly derivative(Poly p) {
    Poly d;
    initPoly(&d);
    for (int i = 0; i < p.n; i++) {
        if (p.terms[i].exp != 0) {
            addTerm(&d, p.terms[i].coef * p.terms[i].exp, p.terms[i].exp - 1);
        }
    }
    return d;
}

static Poly integral(Poly p, double C) {
    Poly in;
    initPoly(&in);
    for (int i = 0; i < p.n; i++) {
        double newExp = p.terms[i].exp + 1;
        addTerm(&in, p.terms[i].coef / newExp, newExp);
    }
    if (C != 0) addTerm(&in, C, 0);
    return in;
}

static void printPoly(Poly p) {
    if (p.n == 0) { printf("0\n"); return; }
    for (int i = 0; i < p.n; i++) {
        double c = p.terms[i].coef;
        double e = p.terms[i].exp;
        
        if (i > 0 && c > 0) printf(" + ");
        else if (c < 0) printf(" - ");
        else if (i > 0 && c == 0) continue;

        double absC = fabs(c);
        if (e == 0) printf("%.6g", absC);
        else if (e == 1) (absC == 1) ? printf("x") : printf("%.6gx", absC);
        else (absC == 1) ? printf("x^%.6g", e) : printf("%.6gx^%.6g", absC, e);
    }
    printf("\n");
}

static double evaluate(Poly p, double x) {
    double sum = 0;
    for (int i = 0; i < p.n; i++) {
        sum += p.terms[i].coef * pow(x, p.terms[i].exp);
    }
    return sum;
}

void savePolyGraph(Poly p, const char* filename) {
    unsigned char *img = (unsigned char *)calloc(IMG_WIDTH * IMG_HEIGHT * 3, 1);
    memset(img, 255, IMG_WIDTH * IMG_HEIGHT * 3); // 背景白色

    // 1. 畫網格與標線 (x, y 從 -10 到 10)
    for (int g = -10; g <= 10; g++) {
        // 將數學座標轉換為像素位置
        int gx = (int)((g + 10) * IMG_WIDTH / 20);
        int gy = (int)((-g + 10) * IMG_HEIGHT / 20);

        // 決定顏色：0, 5, 10 使用深灰色 (160)，其餘使用淺灰色 (220)
        unsigned char gridColor = (g % 5 == 0) ? 160 : 220;

        // 畫垂直網格線 (X = g)
        for (int y = 0; y < IMG_HEIGHT; y++) {
            int idx = (y * IMG_WIDTH + gx) * 3;
            if (idx >= 0 && idx < IMG_WIDTH * IMG_HEIGHT * 3) {
                img[idx] = img[idx+1] = img[idx+2] = gridColor;
            }
        }
        // 畫水平網格線 (Y = g)
        for (int x = 0; x < IMG_WIDTH; x++) {
            int idx = (gy * IMG_WIDTH + x) * 3;
            if (idx >= 0 && idx < IMG_WIDTH * IMG_HEIGHT * 3) {
                img[idx] = img[idx+1] = img[idx+2] = gridColor;
            }
        }
    }

    // 2. 畫主座標軸 (黑色，X=0 和 Y=0)
    for(int i=0; i<IMG_WIDTH; i++) { 
        int idx = (IMG_HEIGHT/2 * IMG_WIDTH + i) * 3; // X軸
        img[idx] = img[idx+1] = img[idx+2] = 0; 
    }
    for(int i=0; i<IMG_HEIGHT; i++) { 
        int idx = (i * IMG_WIDTH + IMG_WIDTH/2) * 3; // Y軸
        img[idx] = img[idx+1] = img[idx+2] = 0; 
    }

    // 3. 畫函數曲線 (紅色)
    for (double x = -10; x <= 10; x += 0.005) {
        double y = evaluate(p, x);
        int px = (int)((x + 10) * IMG_WIDTH / 20);
        int py = (int)((-y + 10) * IMG_HEIGHT / 20);

        if (px >= 0 && px < IMG_WIDTH && py >= 0 && py < IMG_HEIGHT) {
            // 畫 2x2 像素的點，增加線條粗細
            for(int dx=0; dx<2; dx++) {
                for(int dy=0; dy<2; dy++) {
                    int nx = px + dx; int ny = py + dy;
                    if (nx < IMG_WIDTH && ny < IMG_HEIGHT) {
                        int idx = (ny * IMG_WIDTH + nx) * 3;
                        img[idx] = 255; img[idx+1] = 0; img[idx+2] = 0; // 紅色
                    }
                }
            }
        }
    }

    // --- BMP 檔案寫入邏輯 (與之前相同) ---
    int filesize = 54 + 3 * IMG_WIDTH * IMG_HEIGHT;
    unsigned char fileH[14] = {'B','M', filesize, filesize>>8, filesize>>16, filesize>>24, 0,0,0,0, 54,0,0,0};
    unsigned char infoH[40] = {40,0,0,0, IMG_WIDTH, IMG_WIDTH>>8, IMG_WIDTH>>16, IMG_WIDTH>>24, 
                               IMG_HEIGHT, IMG_HEIGHT>>8, IMG_HEIGHT>>16, IMG_HEIGHT>>24, 1,0, 24,0};

    FILE *f = fopen(filename, "wb");
    if(f) {
        fwrite(fileH, 1, 14, f);
        fwrite(infoH, 1, 40, f);
        for(int i=0; i<IMG_HEIGHT; i++) {
            fwrite(img + (IMG_WIDTH * (IMG_HEIGHT - i - 1) * 3), 3, IMG_WIDTH, f);
        }
        fclose(f);
        printf("\n[提示] 圖形標線已繪製，檔案存為: %s\n", filename);
    }
    free(img);
}

#endif 
