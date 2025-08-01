#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 20
#define TOTALEN DIM *DIM
#define SetColor(esc, color) printf("%c[%dm  %c[0m", esc, color, esc);

static int Debug = 1;

typedef struct {
    const int length;
    int appleLocation;
    int *list;

} Grid;
typedef struct {
    int x;
    int y;
} Point;
typedef struct {
    Point head;
    int length;
    Point Tail[3];
    int currentDirec;
} Snake;
typedef enum { RED = 41, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE } TermColors;
// Direction: TOP Down Left Right 72  80  75  77
typedef enum {
    Direc_UP = 72,
    Direc_Down = 80,
    Direc_Left = 75,
    Direc_Right = 77
} Directions;

const Point Direcs[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

void display(const Grid *grid) {
    const int esc = 0x1b;

    for (int i = 0; i < grid->length; i++) {
        int item = grid->list[i];

        int b = i % DIM;
        if (i != 0 && b == 0) {
            printf("\n");
        }

        if (!Debug) {
            printf("%d ", item);
        } else {
            switch (item) {
                case 0:
                    SetColor(esc, WHITE);
                    break;
                case 1:
                    SetColor(esc, BLUE);
                    break;
                case 2:
                    SetColor(esc, PURPLE);
                    break;
                case 3:
                    SetColor(esc, RED);
                    break;
                case 4:
                    SetColor(esc, GREEN);
                    break;
            }
        }
    }
    printf("\n");
}

int findPos(int a, int b) {
    return a + b * DIM;
}
int pointFindPos(Point point) {
    return findPos(point.x, point.y);
}

void ClearScreen() {
    printf("\x1b[0H");
}

void SetSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        snake->Tail[i] =
            (Point){.x = snake->head.x - (i + 1), .y = snake->head.y};
    }
}

void snakeOnGrid(Grid *grid, const Snake *snake) {
    grid->list[pointFindPos(snake->head)] = 1;
    for (int i = 0; i < snake->length; i++) {
        int pos = pointFindPos(snake->Tail[i]);
        grid->list[pos] = 2;
        // printf("%d \n", pos);
    }
}

void resetGrid(Grid *grid) {
    for (int i = 0; i < grid->length; i++) {
        grid->list[i] = 0;
    }
    // grid->list[grid->appleLocation] = 3;
}

void snakeMovement(Snake *snake, Point point) {
    Point temp = snake->head;

    snake->head =
        (Point){.x = (snake->head.x + point.x), .y = (snake->head.y + point.y)};

    snake->head.x = (snake->head.x + DIM) % DIM;
    snake->head.y = (snake->head.y + DIM) % DIM;

    for (int i = 0; i < snake->length; i++) {
        Point current = snake->Tail[i];
        snake->Tail[i] = temp;
        temp = current;
    }
    // printf("\nList: (%d, %d), (%d, %d), (%d, %d)",
    // snake->Tail[0].x, snake->Tail[0].y,
    // snake->Tail[1].x, snake->Tail[1].y,
    // snake->Tail[2].x, snake->Tail[2].y);
    // printf("Head: (%d, %d)\n", snake->head.x, snake->head.y);
}

int isOpposite(int cDirec, int pDirec) {
    // TOP AND DOWN
    if (pDirec == 0 && cDirec == 1) return 1;
    // DOWN AND TOP
    if (pDirec == 1 && cDirec == 0) return 1;
    // LEFT AND RIGHT
    if (pDirec == 2 && cDirec == 3) return 1;
    // //RIGHT AND LEFT
    if (pDirec == 3 && cDirec == 2) return 1;
    return 0;
}
void FrameUpdate(Grid *grid, Snake *snake) {
    snake->currentDirec = 3;
    // printf("FrameUpdate");
    for (;;) {
        // if (kbhit()) {
            int key = getch();
            if (key == 0 || key == 224) key = getch();
            if (key == 3) break;

            // Current Direction
            int newDirection = -1;
            switch (key) {
                case Direc_UP:
                    newDirection = 0;
                    break;
                case Direc_Down:
                    newDirection = 1;
                    break;
                case Direc_Left:
                    newDirection = 2;
                    break;
                case Direc_Right:
                    newDirection = 3;
                    break;
            }

            // int x = isOpposite(cDirec, pDirec);
            if (newDirection != -1 &&
                !isOpposite(newDirection, snake->currentDirec)) {
                snake->currentDirec = newDirection;
            }
        // }

        snakeMovement(snake, Direcs[snake->currentDirec]);

        resetGrid(grid);
        snakeOnGrid(grid, snake);
        ClearScreen();
        display(grid);
        // _sleep(50);
    }
    // printf("\nList: (%d, %d), (%d, %d), (%d, %d)",
    //     snake->Tail[0].x, snake->Tail[0].y,
    //     snake->Tail[1].x, snake->Tail[1].y,
    //     snake->Tail[2].x, snake->Tail[2].y);
    // printf("\n-------------------------------------------------------\n");
}

int main() {
    int snakeLength = 5;
    Debug = 0;
    Grid grid = {.length = DIM * DIM, .appleLocation = 25};
    grid.list = (int[DIM * DIM]){};

    Snake snake = {
        .head = {.x = DIM / 2, .y = DIM / 2},
        .length = snakeLength,
        .Tail = {},
        .currentDirec = 3,
    };

    SetSnake(&snake);

    snakeOnGrid(&grid, &snake);

    FrameUpdate(&grid, &snake);
}