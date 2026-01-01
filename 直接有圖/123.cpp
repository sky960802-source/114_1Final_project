#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_TERMS 10

typedef struct {
    float coeff;
    float exp;
} Term;

// --- 數學運算核心 ---
float evaluate(Term p[], int count, float x) {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += p[i].coeff * pow(x, p[i].exp);
    }
    return sum;
}

// --- 極簡 BMP 圖片產生器 (不需任何 library) ---
void save_bmp(const char* filename, unsigned char* img) {
    int filesize = 54 + 3 * WIDTH * HEIGHT;
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

    bmpfileheader[ 2] = (unsigned char)(filesize      );
    bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
    bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
    bmpfileheader[ 5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[ 4] = (unsigned char)(WIDTH      );
    bmpinfoheader[ 5] = (unsigned char)(WIDTH >>  8);
    bmpinfoheader[ 6] = (unsigned char)(WIDTH >> 16);
    bmpinfoheader[ 7] = (unsigned char)(WIDTH >> 24);
    bmpinfoheader[ 8] = (unsigned char)(HEIGHT      );
    bmpinfoheader[ 9] = (unsigned char)(HEIGHT >>  8);
    bmpinfoheader[10] = (unsigned char)(HEIGHT >> 16);
    bmpinfoheader[11] = (unsigned char)(HEIGHT >> 24);

    FILE *f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for(int i=0; i<HEIGHT; i++) {
        fwrite(img + (WIDTH * (HEIGHT - i - 1) * 3), 3, WIDTH, f);
    }
    fclose(f);
}

int main() {
    Term poly[MAX_TERMS], result[MAX_TERMS];
    int count = 0, choice;
    char input[50];

    printf("=== 基礎微積分運算器 (圖片輸出版) ===\n");
    while (count < MAX_TERMS) {
        printf("輸入多項式項 (如 2x^3)，完成請輸入 Done: ");
        scanf("%s", input);
        if (strcmp(input, "Done") == 0) break;
        char *ptr = strstr(input, "x^");
        if (ptr) {
            *ptr = '\0';
            poly[count].coeff = atof(input);
            poly[count].exp = atof(ptr + 2);
            count++;
        }
    }

    printf("1. 微分\n2. 積分\n請選擇: ");
    scanf("%d", &choice);

    for (int i = 0; i < count; i++) {
        if (choice == 1) { // 微分
            result[i].coeff = poly[i].coeff * poly[i].exp;
            result[i].exp = poly[i].exp - 1;
        } else { // 積分
            result[i].coeff = poly[i].coeff / (poly[i].exp + 1);
            result[i].exp = poly[i].exp + 1;
        }
    }

    // --- 繪圖邏輯：在記憶體中畫圖 ---
    unsigned char *img = (unsigned char *)calloc(WIDTH * HEIGHT * 3, 1);
    // 背景設為白色 (255, 255, 255)
    memset(img, 255, WIDTH * HEIGHT * 3);

    // 畫簡單的座標軸 (黑色)
    for(int i=0; i<WIDTH; i++) { img[(HEIGHT/2*WIDTH + i)*3] = 0; } // X軸
    for(int i=0; i<HEIGHT; i++) { img[(i*WIDTH + WIDTH/2)*3] = 0; } // Y軸

    // 畫函數曲線 (紅色)
    for (float x = -10; x <= 10; x += 0.01f) {
        float y = evaluate(result, count, x);
        // 將數學座標 (-10~10) 轉換為像素座標 (0~800)
        int px = (int)((x + 10) * WIDTH / 20);
        int py = (int)((-y + 10) * HEIGHT / 20); // Y軸反向

        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            int idx = (py * WIDTH + px) * 3;
            img[idx] = 255; // R
            img[idx+1] = 0; // G
            img[idx+2] = 0; // B
        }
    }

    save_bmp("calculus_plot.bmp", img);
    printf("成功！請查看資料夾中的 calculus_plot.bmp\n");
    
    free(img);
    return 0;
}
