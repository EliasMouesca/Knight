#include "Cell.cpp"
#include <deque>
#include <vector>
#include <iostream>

class GridSolver 
{
private:
    bool solve(Cell cell);
    std::vector<Cell> findPossibleJumps(Cell cell);

    void parseQueueToGrid();

    bool isValidCell(Cell cell);

    std::deque<Cell> q;
    int* grid;

public:
    unsigned int rows, cols;
    GridSolver(int* _grid, int _rows, int _cols);
    ~GridSolver();

    bool solveFromThisCell(Cell cell);

};

