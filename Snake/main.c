#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
} SetCells;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point head;
    Point prePoint;
    int length;
    Point *points;
} SnakeProp;

void fill(int *list, SetCells sc) {
    int max = 1;
    int min = 0;
    for (int i = 0; i < sc.count; i++) {
        if (!sc.rand)
            list[i] = 0;
        else
            list[i] = (rand() % (max - min + 1)) + min;
    }
}

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

void resetList(int *list) {
    for (size_t i = 0; i < TCount; i++) {
        list[i] = 0;
    }
}

const Point InitialPoint = {.x = 10, .y = 10};

Point movePoint(Point Og, Point Np) {
    Point newPoint = {.x = Og.x + Np.x, .y = Og.y + Np.y};

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
    // printf("%zu", se->length);
    Point temp = se->head;
    list[getPos(se->head)] = 2;
    for (size_t i = 0; i < se->length; i++) {
        // printf("\nLook: %d/H(%d, %d):(%d, %d)", i, se->head.x, se->head.y,
        //        se->points[i].x, se->points[i].y);
        Point item = se->points[i];
        list[getPos(item)] = 1;
        se->points[i] = temp;
        temp = item;
    }

    // printf("\n----------------------------------------------\n");
}

const Point moveTop = {0, -1};
const Point moveDown = {0, 1};
const Point moveLeft = {-1, 0};
const Point moveRight = {1, 0};

int main() {
    // Debug, Random
    SetCells sc = {DIM, DIM * DIM, 0, 1};
    int cells[TCount] = {};

    Point *pts = assignPoints();

    SnakeProp se = {InitialPoint, {}, DefaultSize, pts};

    snakeEntity(cells, &se);

    display(cells, sc);

    for (;;) {
        int key = getch();
        if (key == 3) break;
        if (key == 0 || key == 224) { // Handle arrow keys
            key = getch();
        }
        // printf("Key: %d\n", key);
        // printf("\x1b[2J\x1b[H");
        clearScreen();
        // TOP Down Left Right
        // 72    80  75  77
        int xswi = 0;
        switch (key) {
            case 72:
                se.head = movePoint(se.head, moveTop);
                break;
            case 80:
                se.head = movePoint(se.head, moveDown);
                break;
            case 75:
                se.head = movePoint(se.head, moveLeft);
                break;
            case 77:
                se.head = movePoint(se.head, moveRight);
                break;
        }
        // resetList(cells);
        memset(cells, 0, TCount * sizeof(int));
        // usleep(100 * 1000);
        // clearScreen();
        snakeEntity(cells, &se);
        display(cells, sc);
    }
    free(pts);
    return 0;
}