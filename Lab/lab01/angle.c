#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int main() {
    printf("Angle (deg)\tSin(angle)\n");
    printf("---------------------------\n");

    for (int angle = 0; angle <= 360; angle += 15) {
        double radians = angle * PI / 180.0;  // 角度转弧度
        double sin_val = sin(radians);        // 使用 math.h 中的 sin 函数
        printf("%3d\t\t%f\n", angle, sin_val);
    }

    return 0;
}
