#include <iostream>
#include <stdlib.h>
#include "GridSolver.h"

void printGrid(int* grid, int rows, int cols);
bool isItPossibleToDo(int rows, int cols);
void startAt(Cell cell, int rows, int cols);

int main(int argc, char** argv)
{
    int r, c, x, y;

    if (argc == 5)
    {
        r = atoi(argv[1]);
        c = atoi(argv[2]);
        x = atoi(argv[3]);
        y = atoi(argv[4]);
    }
    else
    {
        printf("Usage: %s <#rows> <#cols> <starting x> <starting y>\n", argv[0]);
        r = 8; c = 8; x = 4; y = 4;
    }

    startAt(Cell(x, y), r, c);

    return 0;
}

void startAt(Cell cell, int rows, int cols)
{
    int r = rows, c = cols;
    int* grid = (int*) calloc(r * c, sizeof(int));
    GridSolver gs(grid, r, c);

    if (gs.solveFromThisCell(cell))
        printGrid(grid, r, c);
    else printf("Could not find a solution for %d x %d, starting at (%d, %d) :(\n", rows, cols, cell.x, cell.y);

    free(grid);
    return;
}

// This will test if its possible for a given table.
bool isItPossibleToDo(int rows, int cols)
{
    int r = rows, c = cols;
    int* grid = (int*) calloc(r * c, sizeof(int));
    GridSolver gs(grid, r, c);

    for (int i = 0; i < c; i++)
        for (int j = 0; j < r; j++)
            if (gs.solveFromThisCell(Cell(i, j))) 
            {
                printf("It's possible from cell (%d, %d)!\n", i, j);
                printGrid(grid, r, c);
                free(grid);
                return true;
            }

    printf("It's not possible to find a valid path for %d x %d :/\n", rows, cols);

    free(grid);
    return false;
}

void printGrid(int* grid, int rows, int cols)
{
    for (int i = 0; i < rows * cols; i++)
    {
        if (i % cols == 0) puts("");

        if (grid[i] >= 10)
            printf("%d  ", grid[i]);
        else printf("%d   ", grid[i]);
    }

    puts("\n");

    return;
}
