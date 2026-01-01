//定義結構體與函式宣告
/*#ifndef POLY_H
#define POLY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct { double coef; double exp; } Term;
typedef struct { Term terms[100]; int n; } Poly;

void initPoly(Poly *p);
void addTerm(Poly *p, double coef, double exp);
Poly derivative(Poly p);
Poly integral(Poly p, double C);
void printPoly(Poly p);


#endif
*/

#ifndef POLY_H
#define POLY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

#endif 
