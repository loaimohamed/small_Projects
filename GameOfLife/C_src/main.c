#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DIM 30
#define TCount DIM *DIM

typedef struct {
    const int dim;
    const int count;
    int debug;
    int rand;
} SetCells;

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
        if (x != 0 && y == 0)
            printf("\n");
        if (!sc.debug) {
            sprintf(back, "\x1b[46m  \x1b[0m");
            sprintf(fore, "\x1b[47m  \x1b[0m");
        } else {
            sprintf(back, " %d", list[i]);
            sprintf(fore, " %d", list[i]);
        }

        if (list[i] == 1)
            printf("%s", fore);
        else
            printf("%s", back);
    }
}

int getPosition(int x, int y, int d) { return x + y * d; }

void getNeighbors(int current_cell, int *list, int d) {
    int nlist[TCount] = {};
    memcpy(nlist, list, sizeof(int) * d * d);
    // Current Cell x,y;
    int x = current_cell % d;
    int y = (int)current_cell / d;

    // printf("baseXY: %d,%d\n", x, y);

    /*for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + i;
            int npos = getPosition(nx, ny, d);
            printf("%d, %d = %d\n", i, j, npos);
        }
    }*/
    int value = 0;
    for (int i = x - 1; i < x + 2; i++) {
        for (int j = y - 1; j < y + 2; j++) {
            if (i == current_cell && j == current_cell)
                continue;
            if (i < 0 || j < 0 || i >= d || j >= d)
                continue;
            // printf("=> %d, %d\n", i, j);
            int ncp = getPosition(i, j, d);
            value += list[ncp];
        }
    }

    int cell = list[current_cell];
    if (cell == 1 && value <= 1) {
        nlist[current_cell] = 0;
    } else if (cell == 1 && value >= 4) {
        nlist[current_cell] = 0;
    } else if (cell == 1 && value <= 3) {
        nlist[current_cell] = 1;
    } else if (cell == 0 && value == 3) {
        nlist[current_cell] = 1;
    }
    memcpy(list, nlist, sizeof(int) * d * d);
}
void update(int *list, SetCells sc) {
    for (int i = 0; i < sc.count; i++) {
        // printf("%d\n", i);
        getNeighbors(i, list, sc.dim);
    }
}

void clearScreen(int num) {
    printf("\x1b[%dA", num);
    printf("\x1b[%dD", num * 2);
}

int main() {
    //
    const int totalCounts = DIM * DIM;
    // Debug, Random
    SetCells sc = {DIM, totalCounts, 1, 1};

    int cells[totalCounts] = {};
    fill(cells, sc);
    // cells[getPosition(10,10, dim)] = 5;
    /*display(cells, sc);
    //getNeighbors(231, cells, dim);
    printf("\nNew Set\n");
    update(cells, sc);
    display(cells, sc);
    */
    for (;;) {
        usleep(500 * 1000);
        clearScreen(sc.dim);
        update(cells, sc);
        display(cells, sc);
    }
    return 0;
}

/*
int key = getch();
    if (key == 3) break;
    // printf("Look: %d\n", key);
    /*if (key == 32) {
        clearScreen();
        update(cells, sc);
        display(cells, sc);
    }  */
// sleep(1);*/