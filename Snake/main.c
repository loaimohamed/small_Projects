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
#define getPos(...) getCount(__VA_ARGS__, getPos_val, getPos_struct) (__VA_ARGS__)

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

void display(int *list, SetCells sc) {
    char back[23];
    char fore[23];
    for (int i = 0; i < sc.count; i++) {
        int x = i;
        int y = i % sc.dim;
        if (x != 0 && y == 0) printf("\n");
        if (!sc.debug) {
            sprintf(back, "\x1b[46m  \x1b[0m");
            sprintf(fore, "\x1b[47m  \x1b[0m");
        } else {
            sprintf(back, "%d ", list[i]);
            sprintf(fore, "%d ", list[i]);
        }

        if (list[i] == 1)
            printf("%s", fore);
        else
            printf("%s", back);
    }

    printf("\n");
}

int getPosition(int x, int y) {
    return x + y * DIM;
}

void clearScreen() {
    printf("\x1b[%dA", DIM);
    printf("\x1b[%dD", DIM * 2);
}

void resetList(int *list) {
    for (size_t i = 0; i < TCount; i++) {
        list[i] = 0;
    }
}

void snakeEntity(int *list, SnakeProp *se) {
    se->points[0] = se->head;
    Point temp = se->head;
    for (int i = 0; i < se->length; i++) {
        Point current = se->points[i];
        list[getPos(current)] = 1;
        se->points[i] = temp;
        temp = current;
        // printf("%d: (%d, %d), (%d, %d)\n", i, current.x, current.y, temp.x, temp.y); 
        //printf("\n%d => Pre: (%d, %d) | Item: (%d, %d) | Temp: (%d,
        // %d)", i,
        //        se.prePoint.x, se.prePoint.y, item.x, item.y, temp.x, temp.y);
    }
}

const Point moveTop = {0, -1};
const Point moveDown = {0, 1};
const Point moveLeft = {-1, 0};
const Point moveRight = {1, 0};

Point movePoint(Point Og, Point Np) {
    return (Point){.x = Og.x + Np.x, .y = Og.y + Np.y};
}

const Point InitialPoint = {.x = 10, .y = 10};

Point *assignPoints() {
    int allocCount = DefaultSize * sizeof(Point);
    // printf("%p", allocCount);
    Point *mal = (Point *)malloc(allocCount);
    if (mal == NULL) {
        printf("Failed Allocation");
        return 0;
    }

    for (int i = 0; i < DefaultSize; i++) {
        mal[i].x = InitialPoint.x + i;
        mal[i].y = InitialPoint.y;
    }

    return mal;
}

int main() {
    // Debug, Random
    SetCells sc = {DIM, DIM * DIM, 1, 1};
    int cells[TCount] = {};

    Point *pts = assignPoints();


    SnakeProp se = {InitialPoint, {}, DefaultSize, pts};

    snakeEntity(cells, &se);

    display(cells, sc);


    for (;;) {
        int key = getch();
        if (key == 3) break;
        // printf("Key: %d\n", key);
        // TOP Down Left Right
        // 72    80  75  77
        se.prePoint = se.head;
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
        resetList(cells);
        // usleep(50 * 1000);
        clearScreen();
        snakeEntity(cells, &se);
        display(cells, sc);
    }
    free(pts);
    return 0;
}