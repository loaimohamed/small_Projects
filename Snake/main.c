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

        if (Debug) {
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
            (Point){.x = snake->head.x + (i + 1), .y = snake->head.y};
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
    for (int i = 0;i < grid->length;i++) {
        grid->list[i] = 0;
    }  
}

void snakeMovement(Snake *snake, Point point) {
    Point temp = snake->head;
    snake->head = (Point){
            .x = snake->head.x + point.x, 
            .y = snake->head.y + point.y
        };
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

void FrameUpdate(Grid *grid, Snake *snake) {
    // printf("FrameUpdate");
    for (;;) {
        int key = getch();
        if (key == 0 || key == 224) key = getch();
        if (key == 3) break;

        switch (key) {
            case Direc_UP:
                snakeMovement(snake, Direcs[0]);
                break;
            case Direc_Down:
                snakeMovement(snake, Direcs[1]);
                break;
            case Direc_Left:
                snakeMovement(snake, Direcs[2]);
                break;
            case Direc_Right:
                snakeMovement(snake, Direcs[3]);
                break;
        }
        resetGrid(grid);
        snakeOnGrid(grid, snake);
        ClearScreen();
        display(grid);
    }

    // printf("\nList: (%d, %d), (%d, %d), (%d, %d)", 
    //     snake->Tail[0].x, snake->Tail[0].y,
    //     snake->Tail[1].x, snake->Tail[1].y,
    //     snake->Tail[2].x, snake->Tail[2].y);
    // printf("\n-------------------------------------------------------\n");
}

int main() {
    int snakeLength = 3;
    // Debug = 0;
    Grid grid = {.length = DIM * DIM, .appleLocation = 25};
    grid.list = (int[DIM * DIM]){};

    Snake snake = {
        .head = {.x = DIM / 2, .y = DIM / 2},
        .length = snakeLength,
        .Tail = {},
    };

    SetSnake(&snake);

    snakeOnGrid(&grid, &snake);

    FrameUpdate(&grid, &snake);
}