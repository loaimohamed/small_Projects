#include <stdio.h>
#include <math.h>
#include <stdint.h>

// #define getName(var) printf
#define PRINT_POINT(point, ...) printf("Point %s: (%.2f, %.2f) \n", #point, point.x, point.y);
#define mPI 3.141592653589793
#define count 4
#define TOTALC count * count

typedef struct {
    double x;
    double y;
} Point;

Point addPoint(Point a, Point b) {
    return (Point) {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
}

void showPoint(Point p) {
    printf("(%.2f, %.2f)\n", p.x, p.y);
}

int combinePoint(Point a) {
    return a.x + a.y * (double)count;
}

Point addPoint_Z(Point* a, Point *b);
int combinePoint_Z(Point *p, int t);

Point rotate(Point p, Point d) {
    double a = 270.0 / 180.0 * mPI;
    double nx = d.x - p.x;
    double ny = d.y - p.y;

    // printf("r>(%f, %f)\t", ss->angle, a);

    Point np =
        (Point){.x = nx * cos(a) - ny * sin(a), .y = nx * sin(a) + ny * cos(a)};

    return np;
}

int main() {
    printf("Welcome\n");
    Point list[TOTALC] = {};

    for (int i=0; i< TOTALC; i++) {
        list[i] = (Point) {
            .x = i % count,
            .y = floor(i/count)
        };
        // printf("(%d)=", combinePoint(list[i]));
        // PRINT_POINT(list[i]);
    }

    Point pivot = {1.5,1.5};
    int a0 = 270;
    double a = a0 / 180.0 * mPI;

    for (int i=0; i< TOTALC; i++) {
        Point c= list[i];
        Point r = rotate(c, pivot);
        Point np = addPoint(r, pivot);
        Point np1 = addPoint_Z(&r, &pivot);
        int n = combinePoint(np);
        int n1 = combinePoint_Z(&np, 4);
        // printf("\n#(%d)> ", i);
        // // PRINT_POINT(r);
        // printf(" %.2f, %.2f", np.x, np.y);
        // printf(" (%d): ", n);
        // // PRINT_POINT(np);

        printf("\n #(%d): ", i);
        printf("C_Num: {%.2f, %.2f}\t", np.x, np.y);
        printf("C_Total: %d \t", n);
        printf("Z_Num: {%.2f, %.2f}\t", np1.x, np1.y);
        printf("Z_Total: %d \t", n1);
    }

}