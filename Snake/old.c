// NOT WORKING CORRECTLY ==>> REWRITING THE WHOLE GAME

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 20
#define TCount DIM *DIM
#define DefaultSize 7

#define getPos_struct(a) getPosition((a).x, (a).y)
#define getPos_val(a, b) getPosition(a, b)
#define getCount(_1, _2, COUNT, ...) COUNT
#define getPos(...)                                                            \
    getCount(__VA_ARGS__, getPos_val, getPos_struct)(__VA_ARGS__)

typedef struct {
    const int dim;
    const int count;
    int debug;
    int rand;
    int applePlace;
} SetCells;

typedef struct {
    int x;
    int y;
} Point;

typedef enum {
    // TOP Down Left Right
    // 72    80  75  77
    D_UP = 72,
    D_DOWN = 80,
    D_LEFT = 75,
    D_RIGHT = 77,
    D_NONE = 0,
} Direction;

typedef struct {
    Point head;
    int length;
    Point *points;
    /// Current Direction
    int CR_Direction;
} SnakeProp;

enum Color { RED = 41, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE };

void display(int *list, SetCells sc) {
    int esc = 0x1B;
    for (int i = 0; i < sc.count; i++) {
        int x = i;
        int y = i % sc.dim;
        if (x != 0 && y == 0) printf("\n");
        if (sc.debug) {
            printf("%d ", list[i]);
        } else {
            switch (list[i]) {
                case 0:
                    printf("%c[%dm  %c[0m", esc, WHITE, esc);
                    break;
                case 1:
                    printf("%c[%dm  %c[0m", esc, PURPLE, esc);
                    break;
                case 2:
                    printf("%c[%dm  %c[0m", esc, BLUE, esc);
                    break;
                case 3: 
                    printf("%c[%dm  %c[0m", esc, RED, esc);
                    break;
                        
            }
        }
    }

    printf("\n");
}

int getPosition(int x, int y) {
    return x + y * DIM;
}

void clearScreen() {
    printf("\x1b[%dA", DIM);
    printf("\x1b[%dD", DIM * 2);
    printf("\x1b[0H");
}

void resetList(int *list, SetCells sc) {
    memset(list, 0, TCount * sizeof(int));
    list[sc.applePlace] = 3;
}

const Point InitialPoint = {.x = 10, .y = 10};

Point movePoint(const Point *Og, const Point *Np) {
    Point newPoint = {.x = Og->x + Np->x, .y = Og->y + Np->y};

    if (newPoint.x > DIM - 1) {
        newPoint.x = 0;
    }
    if (newPoint.x < 0) {
        newPoint.x = DIM - 1;
    }
    if (newPoint.y > DIM - 1) {
        newPoint.y = 0;
    }
    if (newPoint.y < 0) {
        newPoint.y = DIM - 1;
    }

    // printf("\n(%d, %d)->(%d, %d)\n", newPoint.x, newPoint.y, 0, 0);

    return newPoint;
}

Point *assignPoints() {
    int allocCount = DefaultSize * sizeof(Point);
    // printf("%p", allocCount);
    Point *mal = (Point *)malloc(allocCount);
    if (mal == NULL) {
        printf("Failed Allocation");
        return 0;
    }
    int c = 64;
    for (int i = 0; i < DefaultSize; i++) {
        // mal[i].c = c;
        // mal[i].n = i;
        mal[i].x = InitialPoint.x + i;
        mal[i].y = InitialPoint.y;
        if (c <= 90)
            c++;
        else
            c = 64;
    }

    return mal;
}

void snakeEntity(int *list, SnakeProp *se) {
    Point temp = se->head;
    list[getPos(se->head)] = 2;
    for (size_t i = 0; i < se->length; i++) {
        Point item = se->points[i];
        list[getPos(item)] = 1;
        se->points[i] = temp;
        temp = item;
    }

    // printf("\n----------------------------------------------\n");
}

const Point Movements[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

int AppleSet(int *list) {
    int min = 0;
    int max = TCount;
    int rnd = (rand() % (max - min + 1)) + min;
    return rnd;
}
//this is working correctly but
//TODO: try cecking if the next cell is zero if true then move if false then don't move
int isOpposite(int current, int newDir) {
    if (current == 0 && newDir == 1) return 1;
    if (current == 1 && newDir == 0) return 1;
    if (current == 2 && newDir == 3) return 1;
    if (current == 3 && newDir == 2) return 1;
    return 0;
}

int main() {
    // Debug, Random
    SetCells sc = {DIM, DIM * DIM, 0, 1};
    int cells[TCount] = {};

    Point *pts = assignPoints();

    SnakeProp se = {InitialPoint, DefaultSize, pts, 0};

    snakeEntity(cells, &se);

    display(cells, sc);

    sc.applePlace = AppleSet(cells);

    for (;;) {
        int key = getch();
        if (key == 3) break;
        if (key == 0 || key == 224) { // Handle arrow keys
            key = getch();
        }
        // printf("Key: %d\n", key);
        // printf("\x1b[2J\x1b[H");
        clearScreen();
        // x, y blocker
        int xb = 0;
        int yb = 0;
        int canMove = 0;
        int newDirection = 4;

        switch (key) {
            case D_UP: // Top arrow
                newDirection = 0;
                break;
            case D_DOWN: // Down arrow
                newDirection = 1;
                break;
            case D_LEFT: // Left arrow
                newDirection = 2;
                break;
            case D_RIGHT: // Right arrow
                newDirection = 3;
                break;
        }

        if (newDirection <= 3 && !isOpposite(se.CR_Direction, newDirection)) {
            se.head = movePoint(&se.head, &Movements[newDirection]);
            se.CR_Direction = newDirection;

            clearScreen();
            resetList(cells, sc);
            snakeEntity(cells, &se);
            display(cells, sc);
        }
        printf("C: %d\t N: %d", se.CR_Direction, newDirection);
        // printf("\nLook: %d\t", newDirection);
        // printf("MoveLock: {x:%d, y:%d}", xb, yb);
    }

    free(pts);
    return 0;
}