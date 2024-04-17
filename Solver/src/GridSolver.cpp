#include "GridSolver.h"

bool solve(Cell cell);
void parseQueueToGrid();

GridSolver::GridSolver(int* _grid, int _rows, int _cols) : grid(_grid), rows(_rows), cols(_cols) { }

GridSolver::~GridSolver() { }

bool GridSolver::solveFromThisCell(Cell cell)
{
    if (solve(cell) == false) 
        return false; 

    parseQueueToGrid();

    return true;

}

bool GridSolver::solve(Cell cell)
{
    q.push_front(cell);
    //printf("Pushed (%d, %d)\n", cell.x, cell.y);

    if (q.size() >= rows * cols) return true;

    for (Cell possibleCell : findPossibleJumps(cell))
        if (solve(possibleCell) == true) return true;

    //puts("Going back one level");
    //printf("popped (%d, %d)\n", cell.x, cell.y);
    q.pop_front();
    return false;
}

std::vector<Cell> GridSolver::findPossibleJumps(Cell cell)
{
    int dx[] = {-2, -1, 1, 2, -2, -1, 1, 2};
    int dy[] = {-1, -2, -2, -1, 1, 2, 2, 1};

    std::vector<Cell> ret;

    for (int i = 0; i < 8; i++)
    {
        Cell possibleCell = Cell(cell.x + dx[i], cell.y + dy[i]);
        if (isValidCell(possibleCell)) ret.push_back(possibleCell);
    }

    return ret;
}

bool GridSolver::isValidCell(Cell cell)
{
    if (cell.x < 0 || cell.x >= cols) return false;
    if (cell.y < 0 || cell.y >= rows ) return false;

    for (Cell c : q)
        if (cell.x == c.x && cell.y == c.y) return false;

    return true;
}

void GridSolver::parseQueueToGrid()
{
    int i = 0;
    while (!q.empty())
    {
        Cell cell = q.back();
        grid[cell.x + cell.y * cols] = i++;

        //printf("%d) r:%d c:%d\n", i, cell.x, cell.y);

        q.pop_back();
    }

    return;
}

