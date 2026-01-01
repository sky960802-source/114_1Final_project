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
