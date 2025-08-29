#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846
typedef struct {
    float x;
    float y;
} Point;

void frotate(Point *points, const Point *center, float theta, const int count) {
    theta = theta / 180 * PI;
    for (int i = 0; i < count; i++) {

        float nx = (points[i].x - center->x) * cos(theta) -
                      (points[i].y - center->y) * sin(theta) + center->x;
        float ny = (points[i].x - center->x) * sin(theta) +
                      (points[i].y - center->y) * cos(theta) + center->y;

        points[i].x = nx;
        points[i].y = ny;
    }
}

int main() {
    printf("Beginning\n");
    const int c = 3;
    const int tc = 3 * 3;
    Point points[tc];
    printf("Original: \n");
    for (int i = 0; i < tc; i++) {
        points[i].x =  i % c;
        points[i].y = floor(i / c);
        printf("%d: (%.1f, %.1f) ", i, points[i].x, points[i].y);
    }
    printf("\n");

    Point center = {1.0, 1.0};

    frotate(points, &center, 90.0, tc);

    printf("Rotated: \n");
    for (int i = 0; i < tc; i++) {
        printf("%d: (%.1f, %.1f) ",i, points[i].x, points[i].y);
    }

    return 0;
}