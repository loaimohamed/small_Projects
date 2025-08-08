#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 20
#define TOTALEN DIM *DIM
#define SetColor(color) printf("\x1b[%dm  \x1b[0m", color);
#define mPI 3.14159

static int Debug = 0;

typedef struct {
    const int length;
    const int spawnPoint;
    int *list;
} Grid;

typedef struct {
    int x;
    int y;
} Point;

const int Shapes[5][8] = {{3, 3, 1, 3, 1, 1, 1, 3},
                          {3, 3, 1, 3, 1, 3, 1, 1},
                          {3, 3, 3, 1, 1, 1, 1, 3},
                          {1, 3, 1, 3, 1, 3, 1, 3}};

typedef enum { T_Shape = 0, L_Shape, Z_Shape, I_Shape } eShapes;

typedef enum { RED = 41, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE } TermColors;

int getCord(int x, int y, int s) {
    return x + y * s;
};
int getCordP(Point p, int s) {
    return getCord(p.x, p.y, s);
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

Point rotateAround(Point origin, Point dest, double angle) {
    return (Point){.x = (dest.x - origin.x) * cos(angle) - (dest.y - origin.y) * sin(angle) + origin.x,
                   .y = (dest.x - origin.x) * sin(angle) + (dest.y - origin.y) * cos(angle) + origin.y};
}

void setShapes(Grid *grid) {


    const int x = grid->spawnPoint % DIM;
    const int y = floor(grid->spawnPoint / DIM);
    const int s = 3;
    double a = 45.0;
    a = a / 180.0 * mPI;

    Point np = rotateAround((Point){x, y}, (Point){x + s, y}, a);

    grid->list[getCordP(np, DIM)] = 2;
    // printf("Look: %d\n", getCordP(np, DIM));

    for (int i = 0; i < 4; i++) {
        // int s = getCord(nx, ny, DIM) + (0 + i * DIM);
        // int sh = T_Shape;
        // grid->list[s] = Shapes[sh][i * 2];
        // grid->list[s-1] = Shapes[sh][i * 2+1];

        int s = getCordP(np, DIM) + (0 + i * DIM);
        int sh = T_Shape;
        // grid->list[s] = Shapes[sh][i * 2];
        // grid->list[s-1] = Shapes[sh][i * 2+1];

        printf("%d %d\n", Shapes[sh][i * 2], Shapes[sh][i * 2+1]);

        // printf("Look: %d\n", s);
    }
    grid->list[grid->spawnPoint] = 4;
}

int main() {
    Grid grid = {.length = DIM * DIM, .spawnPoint = 210};
    grid.list = (int[DIM * DIM]){};
    // Debug = 1;

    setShapes(&grid);

    display(&grid);
}