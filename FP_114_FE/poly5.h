#ifndef POLY5_H
#define POLY5_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 定義圖片大小與多項式最大項數
#define IMG_WIDTH 800
#define IMG_HEIGHT 600
#define MAX_TERMS 100

// 單一項的結構：係數 (coef) 與指數 (exp)
typedef struct 
{
    double coef;
    double exp;
} Term;

// 多項式結構
typedef struct 
{
    Term terms[MAX_TERMS];
    int n; // 目前有多少項
} Poly;

// 初始化多項式
static void initPoly(Poly *p) 
	{ p->n = 0; }

// 新增一項到多項式中
static void addTerm(Poly *p, double coef, double exp) 
{
    if (p->n < MAX_TERMS) 
	{
        p->terms[p->n].coef = coef;
        p->terms[p->n].exp = exp;
        p->n++;
    }
}

// 計算多項式在 x 的值：f(x)
static double evaluate(Poly p, double x) 
{
    double sum = 0;
    for (int i = 0; i < p.n; i++) sum += p.terms[i].coef * pow(x, p.terms[i].exp);
    return sum;
}

// 微分：回傳一個新的多項式 f'(x)
static Poly derivative(Poly p) 
{
    Poly d; initPoly(&d);
    for (int i = 0; i < p.n; i++) 
        if (p.terms[i].exp != 0) // 常數項微分為 0，忽略
            addTerm(&d, p.terms[i].coef * p.terms[i].exp, p.terms[i].exp - 1);
    return d;
}

// 積分：回傳一個新的多項式 ∫f(x)dx
static Poly integral(Poly p, double C) 
{
    Poly in; initPoly(&in);
    for (int i = 0; i < p.n; i++) 
        addTerm(&in, p.terms[i].coef / (p.terms[i].exp + 1), p.terms[i].exp + 1);
    if (C != 0) addTerm(&in, C, 0); // 加入積分常數
    return in;
}

// 在終端機印出多項式字串
static void printPoly(Poly p) 
{
    if (p.n == 0) 
	{ printf("0\n"); return; }
    for (int i = 0; i < p.n; i++) 
	{
        double c = p.terms[i].coef; double e = p.terms[i].exp;
        if (i > 0 && c > 0) printf(" + "); else if (c < 0) printf(" - ");
        if (e == 0) printf("%.2f", fabs(c)); // 常數項
        else if (e == 1) printf("%.2fx", fabs(c)); // 一次項
        else printf("%.2fx^%.2f", fabs(c), e); // 高次項
    }
    printf("\n");
}

// 尋找特殊點 (order=1 找極值, order=2 找反曲點)
static Poly findPoints(Poly p, int order) 
{
    Poly res; initPoly(&res);
    Poly target = (order == 1) ? derivative(p) : derivative(derivative(p));
    
    //如果目標函數完全是 0，直接回傳空結果
    if (target.n == 0) return res; 
    int isZero = 1;
    for(int k=0; k<target.n; k++) 
	{
        if(fabs(target.terms[k].coef) > 0.000001) isZero = 0;
    }
    if(isZero) return res;

    for (double x = -10.0; x <= 10.0; x += 0.0001) 
	{
        if (fabs(evaluate(target, x)) < 0.005) 
		{
            if (res.n == 0 || fabs(res.terms[res.n-1].coef - x) > 0.5) 
			{
                double val = (fabs(x) < 0.005) ? 0.0 : x;
                addTerm(&res, val, 0);
            }
        }
    }
    return res;
}

// 繪製圖形並存為 BMP 檔
// p: 原函數, cp: 極值點集合, ip: 反曲點集合
static void savePolyGraph(Poly p, const char* filename, Poly cp, Poly ip) 
{
    printf("\n>>>正在繪製圖形: %s\n", filename);
    // 配置 RGB 圖片記憶體 (寬 * 高 * 3 bytes)
    unsigned char *img = (unsigned char *)calloc(IMG_WIDTH * IMG_HEIGHT * 3, 1);
    if (!img) return;
    memset(img, 255, IMG_WIDTH * IMG_HEIGHT * 3); // 填滿白色 (255)

    // 1. 畫網格 (Grid)
    for (int g = -10; g <= 10; g++) 
	{
        // 座標轉換：數學座標 -> 像素座標
        int gx = (int)((g + 10) * IMG_WIDTH / 20.0);
        int gy = (int)((-g + 10) * IMG_HEIGHT / 20.0);
        
        unsigned char c;
        if (g == 0) 
		{
            c = 0;    // 黑色 (軸線 x=0, y=0)
        } 
		else if (g % 10 == 0) 
		{
            c = 100;  // 深灰 (10的倍數，如 -10, 10)
        } 
		else if (g % 5 == 0) 
		{
            c = 160;  // 中灰 (5的倍數，如 -5, 5)
        } 
		else 
		{
            c = 220;  // 淺灰 (其他網格線)
        }

        // 畫垂直線
        if (gx >= 0 && gx < IMG_WIDTH) 
            for (int y = 0; y < IMG_HEIGHT; y++) 
			{
                int idx = (y * IMG_WIDTH + gx) * 3;
                img[idx] = img[idx+1] = img[idx+2] = c;
            }
        
        // 畫水平線
        if (gy >= 0 && gy < IMG_HEIGHT)
            for (int x = 0; x < IMG_WIDTH; x++) 
			{
                int idx = (gy * IMG_WIDTH + x) * 3;
                img[idx] = img[idx+1] = img[idx+2] = c;
            }
    }
    

    // 2. 畫函數曲線 (紅色)
    for (double x = -10.0; x <= 10.0; x += 0.001) 
	{
        double y = evaluate(p, x);
        int px = (int)((x + 10) * IMG_WIDTH / 20.0);
        int py = (int)((-y + 10) * IMG_HEIGHT / 20.0);
        if (px >= 0 && px < IMG_WIDTH && py >= 0 && py < IMG_HEIGHT) 
		{
            int idx = (py * IMG_WIDTH + px) * 3;
            // BGR 順序：藍=0, 綠=0, 紅=255
            img[idx] = 0; img[idx+1] = 0; img[idx+2] = 255;
        }
    }

    // 3. 畫極值點 (藍色大點)
    for (int i = 0; i < cp.n; i++) 
	{
        double val = cp.terms[i].coef; // 取得 x 座標
        int px = (int)((val + 10) * IMG_WIDTH / 20.0);
        int py = (int)((-evaluate(p, val) + 10) * IMG_HEIGHT / 20.0);
        // 畫一個 7x7 的方塊
        for(int dy=-3; dy<=3; dy++) for(int dx=-3; dx<=3; dx++) 
		{
            int nx=px+dx, ny=py+dy;
            if(nx>=0 && nx<IMG_WIDTH && ny>=0 && ny<IMG_HEIGHT) 
			{
                int idx = (ny*IMG_WIDTH + nx)*3;
                img[idx]=255; img[idx+1]=0; img[idx+2]=0; // 藍色
            }
        }
    }

    // 4. 畫反曲點 (綠色大點)
    for (int i = 0; i < ip.n; i++) 
	{
        double val = ip.terms[i].coef;
        int px = (int)((val + 10) * IMG_WIDTH / 20.0);
        int py = (int)((-evaluate(p, val) + 10) * IMG_HEIGHT / 20.0);
        for(int dy=-3; dy<=3; dy++) for(int dx=-3; dx<=3; dx++) 
		{
            int nx=px+dx, ny=py+dy;
            if(nx>=0 && nx<IMG_WIDTH && ny>=0 && ny<IMG_HEIGHT) 
			{
                int idx = (ny*IMG_WIDTH + nx)*3;
                img[idx]=0; img[idx+1]=200; img[idx+2]=0; // 綠色
            }
        }
    }

    // 5. 寫入 BMP 檔案
    FILE *f = fopen(filename, "wb");
    if(f) {
        // BMP 檔頭設定 (固定格式)
        unsigned char fH[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
        unsigned char iH[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
		int sz = 54 + 3 * IMG_WIDTH * IMG_HEIGHT;
        fH[2]=(unsigned char)sz; fH[3]=(unsigned char)(sz>>8); // 檔案大小
        iH[4]=(unsigned char)IMG_WIDTH; iH[5]=(unsigned char)(IMG_WIDTH>>8); // 寬
        iH[8]=(unsigned char)IMG_HEIGHT; iH[9]=(unsigned char)(IMG_HEIGHT>>8); // 高
        
        fwrite(fH, 1, 14, f); fwrite(iH, 1, 40, f);
        // 寫入像素數據
        for(int i = IMG_HEIGHT - 1; i >= 0; i--) 
            fwrite(&img[i * IMG_WIDTH * 3], 3, IMG_WIDTH, f);
        fclose(f);
        printf(">>>圖片已存至: %s\n", filename);
    }
    free(img);
}
#endif
