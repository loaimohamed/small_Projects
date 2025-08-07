#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 20
#define TOTALEN DIM *DIM
#define SnakeSize 5
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
    Point Tail[TOTALEN];
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
/// @brief Display the 1D grid into Console
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
/// @brief Transform x and y coordinates into 1D list {x + y * size} from Two Params
int findPos(int a, int b) {
    return a + b * DIM;
}
/// @brief Transform x and y coordinates into 1D list {x + y * size} from Point Struct
int pfps(Point point) {
    return findPos(point.x, point.y);
}
/// @brief Return the sum of two Points
Point addPoint(Point I, Point II) {
    return (Point){.x = I.x + II.x, .y = I.y + II.y};
}
/// @brief simple clear screen
void ClearScreen() {
    printf("\x1b[0H");
}
/// @brief set basic snake tail coordinates
void setSnakeTail(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        snake->Tail[i] =
            (Point){.x = snake->head.x - (i + 1), .y = snake->head.y};
    }
}
/// @brief Transform Snake Coordinates to 1D Grid
void snakeOnGrid(Grid *grid, const Snake *snake) {
    grid->list[pfps(snake->head)] = 1;
    grid->list[grid->appleLocation] = 3;
    for (int i = 0; i < snake->length; i++) {
        int pos = pfps(snake->Tail[i]);
        grid->list[pos] = 2;
        // printf("%d \n", pos);
    }
}
/// @brief Reset the list. can Use memset() instead
void resetGrid(Grid *grid) {
    for (int i = 0; i < grid->length; i++) {
        grid->list[i] = 0;
    }
    // grid->list[grid->appleLocation] = 3;
}
/// @brief Handle the movement of tails point behind the Head
void snakeMovement(Snake *snake, const Point point) {
    Point temp = snake->head;

    snake->head = addPoint(snake->head, point);
    
    snake->head.x = (snake->head.x + DIM) % DIM;
    snake->head.y = (snake->head.y + DIM) % DIM;
    
    for (int i = 0; i < snake->length; i++) {
        Point current = snake->Tail[i];
        snake->Tail[i] = temp;
        temp = current;
    }
}
/// @brief Check the direction of snake return 1 if the direction is Equal 
/// @param cDirec Current Direction
/// @param pDirec Previous Direction
int isOpposite(int cDirec, int pDirec) {
    // TOP AND DOWN
    if (pDirec == 0 && cDirec == 1) return 1;
    // DOWN AND TOP
    if (pDirec == 1 && cDirec == 0) return 1;
    // LEFT AND RIGHT
    if (pDirec == 2 && cDirec == 3) return 1;
    // //RIGHT AND LEFT
    if (pDirec == 3 && cDirec == 2) return 1;
    //printf("==>>%d, %d\n", cDirec, pDirec);
    return 0;
}
/// @brief Handle Frames and Movement using Arrow Keys
void FrameUpdate(Grid *grid, Snake *snake) {
    // snake->currentDirec = 3;
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
            
            Point newPosition = addPoint(snake->head, Direcs[snake->currentDirec]);

            if (grid->list[pfps(newPosition)] == 3) {
                snake->length += 1;
                grid->appleLocation += 2;
            }

            // printf("=> %d", snake->currentDirec);
            if (newDirection != -1 && !isOpposite(newDirection, snake->currentDirec)) {
                    snake->currentDirec = newDirection;
                    snakeMovement(snake, Direcs[snake->currentDirec]);
            }
            // printf("2==>>%d, %d\n", newDirection, snake->currentDirec);
        // }


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
    //snake Length must much the snake Tail size if not might not be working Correctly
    int snakeLength = 3;
    // Debug = 0;
    Grid grid = {.length = DIM * DIM, .appleLocation = 25};
    grid.list = (int[DIM * DIM]){};

    Snake snake = {
        .head = {.x = DIM / 2, .y = DIM / 2},
        .length = snakeLength,
        .Tail = {},
        .currentDirec = 3,
    };



    setSnakeTail(&snake);

    snakeOnGrid(&grid, &snake);

    FrameUpdate(&grid, &snake);
}