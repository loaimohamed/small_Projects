#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 11
#define TOTALEN DIM *DIM
#define SetColor(color) printf("\x1b[%dm  \x1b[0m", color);
#define mPI 3.141592653589793

static int Debug = 0;

typedef struct {
    const int length;
    const int spawnPoint;
    int *list;
} Grid;

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point basePoint;
    double angle;
    Point pivot;
    int id;
} Shape_Struct;

int Shapes[5][16] = {{3, 3, 3, 3, 1, 3, 3, 3, 1, 1, 3, 3, 1, 3, 3, 3},
    {3, 3, 1, 3, 1, 3, 1, 1},
    {3, 3, 3, 1, 1, 1, 1, 3},
    {1, 3, 1, 3, 1, 3, 1, 3},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2}};

int ShapeTest[4] = {3, 3, 1, 3};

typedef enum {
    T_Shape = 0,
    L_Shape,
    Z_Shape,
    I_Shape,
    H_Shape,
    F_Shape
} eShapes;

typedef enum { RED = 41, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE } TermColors;

typedef enum {
    Direc_UP = 72,
    Direc_Down = 80,
    Direc_Left = 75,
    Direc_Right = 77
} Directions;

int getCord(int x, int y, int s) {
    return x + y * s;
};

int getCordP(Point p, int s) {
    return getCord(p.x, p.y, s);
}

void ClearScreen() {
    printf("\x1b[0H");
}

/// @brief Display the 1D grid into Console
void display(const Grid *grid) {
    for (int i = 0; i < grid->length; i++) {
        int item = grid->list[i];

        int b = i % DIM;
        if (i != 0 && b == 0) {
            printf("\n");
        }

        if (Debug) {
            printf("%d ", item);
        } else {
            switch (item) {
                case 0:
                    SetColor(WHITE);
                    break;
                case 1:
                    SetColor(BLUE);
                    break;
                case 2:
                    SetColor(PURPLE);
                    break;
                case 3:
                    SetColor(RED);
                    break;
                case 4:
                    SetColor(GREEN);
                    break;
            }
        }
    }
    printf("\n");
}

void clearGrid(Grid *grid) {
    for (int i = 0; i < grid->length; i++) {
        grid->list[i] = 0;
    }
}

Point rotateA(Point p, Point d, Shape_Struct *ss) {
    double a = ss->angle / 180 * mPI;
    double nx = p.x - d.x;
    double ny = p.y - d.y;

    // printf("r>(%f, %f)\t", ss->angle, a);

    Point np =
        (Point){.x = nx * cos(a) - ny * sin(a), .y = nx * sin(a) + ny * cos(a)};

    return np;
}

void setShapes(Grid *grid, Shape_Struct *ss) {
    grid->list[grid->spawnPoint] = 4;
    const int count = sizeof(ShapeTest) / sizeof(int);
    int *sh = ShapeTest;
    const int hc = count / 2;
    for (int i = 0; i < count; i++) {
        const int x = i % hc;
        const int y = floor(i / hc);
        Point ip = {ss->basePoint.x + x, ss->basePoint.y + y};
        int grid_Ps = getCord(ip.x, ip.y, DIM);
        int Shape_Ps = getCord(x, y, hc);

        Point np = rotateA(ip, ss->basePoint, ss);
        Point np1 = {np.x + ss->basePoint.x, np.y + ss->basePoint.y};

        // printf("%f %f\n", np1.x, np1.y);
        int grid_Ps2 = getCord(np1.x, np1.y, DIM);

        // grid->list[grid_Ps] = sh[Shape_Ps];
        grid->list[grid_Ps2] = sh[Shape_Ps];
    }
}

void FrameUpdate(Grid *grid, Shape_Struct *ss) {
    setShapes(grid, ss);
    display(grid);
    for (;;) {
        // if (kbhit()) {
            int key = getch();
            if (key == 0 || key == 224) key = getch();
            if (key == 3) break;

            clearGrid(grid);
            ClearScreen();
            setShapes(grid, ss);
            display(grid);

            switch (key) {
                case Direc_UP:
                    setShapes(grid, ss);
                    break;
                case Direc_Down:
                    if (ss->basePoint.y < DIM) ss->basePoint.y++;
                    break;
                case Direc_Left:
                    break;
                case Direc_Right:
                    break;
            }
        // _sleep(50);
        // }
    }
}


int main() {
    Grid grid = {.length = DIM * DIM, .spawnPoint = 60};
    grid.list = (int[DIM * DIM]){};
    Debug = 1;
    // int i = 2;

    Point center = {3.5, 3.5};

    Shape_Struct ss = {
        .basePoint = {2, 2}, .angle = 0, .pivot = center, .id = F_Shape};

    FrameUpdate(&grid, &ss);

    // while (i != 0) {
    //     printf("----------------------------------\n");
    //     clearGrid(&grid);
    //     ClearScreen();
    //     setShapes(&grid, &ss);
    //     display(&grid);
    //     ss.angle = 90;
    //     i--;
    // };
}