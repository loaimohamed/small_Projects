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

int Shapes[6][16] = {{3, 3, 3, 3, 1, 3, 3, 3, 1, 1, 3, 3, 1, 3, 3, 3},
    {3, 3, 1, 3, 1, 3, 1, 1},
    {3, 3, 3, 1, 1, 1, 1, 3},
    {1, 3, 1, 3, 1, 3, 1, 3},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2},
    {5, 5, 3, 3, 5, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};

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

void shapeRotate(Shape_Struct *ss) {
    const int count = sizeof(Shapes[ss->id]) / sizeof(int);
    int tempShape[count] = {};
    const int hc = sqrt(count);
    for (int i = 0; i < count; i++) {
        tempShape[i] = Shapes[ss->id][i];
        const int x = i % hc;
        const int y = floor(i / hc);
        int grid_pos = getCord(x, y, hc);

        // printf("(%d, %d)", x + 2, y + 1);

        Point og = {x, y};
        Point pivot = {1.5, 1.5};
        Point rotated = rotateA(og, pivot, ss);
        Point np = {rotated.x + pivot.x + 0.5, rotated.y + pivot.y + 0.5};

        int alt_pos = getCord(np.x, np.y, hc);

        // printf("%d, %d\n", alt_pos, ShapeTest[alt_pos]);
        // printf("%2.f, %2.f\n", rotated.x+pivot.x, rotated.y+pivot.y);
        if (i == 4 || i == 8 || i == 9 || i == 12) {
            // printf("<=");
            // printf("(%d)(%d, %d) => (%2.f) (%f, %f) = (%d #%d)\n",
            //     i, x, y,ss->angle, np.x, np.y, alt_pos, 
            // (int)np.x
            // );
            // printf("%f, %f == %f, %f\n", rotated.x, rotated.y, pivot.x, pivot.y);
        }
        // printf("(%d)>(%.1f, %.1f)(%.1f) = %d\n",i, np.x, np.y,(double)hc, alt_pos);
        // printf("%d>%d\n",i, alt_pos);
        printf("\n");
        Shapes[ss->id][(int)alt_pos] = tempShape[grid_pos];
    }
}

void setShapes(Grid *grid, Shape_Struct *ss) {
    grid->list[grid->spawnPoint] = 4;
    const int count = sizeof(Shapes[ss->id]) / sizeof(int);
    // printf("%d\n", count);
    int *sh = Shapes[ss->id];
    const int hc = sqrt(count);
    for (int i = 0; i < count; i++) {
        const int x = i % hc;
        const int y = floor(i / hc);
        Point ip = {ss->basePoint.x + x, ss->basePoint.y + y};
        int grid_Ps = getCord(ip.x, ip.y, DIM);
        int Shape_Ps = getCord(x, y, hc);

        // printf("%f, %f = %d | %d\n", ip.x, ip.y, grid_Ps, sh[Shape_Ps]);
        grid->list[grid_Ps] = sh[Shape_Ps];
        // if (i == 4 || i == 8 || i == 9 || i == 12) grid->list[grid_Ps] = 5;
    }
}

void FrameUpdate(Grid *grid, Shape_Struct *ss) {
    // shapeRotate(ss);
    setShapes(grid, ss);
    display(grid);
    const Point oldPoint = ss->basePoint;
    for (;;) {
        // if (kbhit()) {
        int key = getch();
        if (key == 0 || key == 224) key = getch();
        if (key == 3) break;

        switch (key) {
            case Direc_UP:
                // ss->angle = 90; 
                shapeRotate(ss);
                break;
            case Direc_Down:
                if (ss->basePoint.y + 4 < DIM) ss->basePoint.y++;
                break;
            case Direc_Left:
                if (ss->basePoint.x >= 0) ss->basePoint.x--;
                break;
            case Direc_Right:
                if (ss->basePoint.x + 4 < DIM) ss->basePoint.x++;
                break;
            case 114:
                ss->basePoint = oldPoint;
                break;
            default:
                printf("Not Assigned Key: %c (%d)", key, key);
                break;
        }
        ClearScreen();
        clearGrid(grid);
        setShapes(grid, ss);
        display(grid);
        // _sleep(50);
        // }
    }
}

int main() {
    Grid grid = {.length = DIM * DIM, .spawnPoint = 60};
    grid.list = (int[DIM * DIM]){};
    // Debug = 1;
    // int i = 2;

    Point center = {3.5, 3.5};

    Shape_Struct ss = {
        .basePoint = {2, 2}, .angle = 90, .pivot = center, .id = T_Shape};

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