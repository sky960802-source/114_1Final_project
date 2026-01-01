//實作微積分的數學邏輯
#include "poly.h"

// 初始化
void initPoly(Poly *p) {
    p->n = 0;
}

// 加入一項
void addTerm(Poly *p, double coef, double exp) {
    if (p->n < MAX_TERMS) {
        p->terms[p->n].coef = coef;
        p->terms[p->n].exp = exp;
        p->n++;
    }
}

// 微分邏輯
Poly derivative(Poly p) {
    Poly d;
    initPoly(&d);
    for (int i = 0; i < p.n; i++) {
        if (p.terms[i].exp != 0) { // 常數項微分為 0，所以跳過
            addTerm(&d, p.terms[i].coef * p.terms[i].exp, p.terms[i].exp - 1);
        }
    }
    return d;
}

// 積分邏輯
Poly integral(Poly p, double C) {
    Poly in;
    initPoly(&in);
    for (int i = 0; i < p.n; i++) {
        double newExp = p.terms[i].exp + 1;
        addTerm(&in, p.terms[i].coef / newExp, newExp);
    }
    if (C != 0) addTerm(&in, C, 0); // 加上積分常數
    return in;
}

// 印出結果
void printPoly(Poly p) {
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
    memset(img, 255, IMG_WIDTH * IMG_HEIGHT * 3); // 白色背景

    // 畫座標軸 (黑色)
    for(int i=0; i<IMG_WIDTH; i++) { 
        int idx = (IMG_HEIGHT/2 * IMG_WIDTH + i) * 3;
        img[idx] = img[idx+1] = img[idx+2] = 0; 
    }
    for(int i=0; i<IMG_HEIGHT; i++) { 
        int idx = (i * IMG_WIDTH + IMG_WIDTH/2) * 3;
        img[idx] = img[idx+1] = img[idx+2] = 0; 
    }

    // 畫曲線 (紅色)
    for (double x = -10; x <= 10; x += 0.01) {
        double y = evaluate(p, x);
        int px = (int)((x + 10) * IMG_WIDTH / 20);
        int py = (int)((-y + 10) * IMG_HEIGHT / 20);

        if (px >= 0 && px < IMG_WIDTH && py >= 0 && py < IMG_HEIGHT) {
            int idx = (py * IMG_WIDTH + px) * 3;
            img[idx] = 255; img[idx+1] = 0; img[idx+2] = 0;
        }
    }

    // BMP Header 製作與寫入
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
        printf("圖形已存至: %s\n", filename);
    }
    free(img);
}
