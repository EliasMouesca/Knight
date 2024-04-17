#include <stdio.h>
#include <SDL2/SDL.h>
#include "caballito.h"

int main(int argc, char** argv)
{
    int rows, cols, x, y;

    if (argc == 5)
    {
        rows = atoi(argv[1]);
        cols = atoi(argv[2]);
        x = atoi(argv[3]);
        y = atoi(argv[4]);
    }
    else
    {
        printf("Usage: %s <#rows> <#cols> <start x> <start y>\n\n", argv[0]);
        rows = 8; cols = 8; x = 0; y = 0;
    }

    Caballito caballito(rows, cols);
    caballito.setFirst(x, y);
    caballito.calculateGrid();
    caballito.renderGrid();

    while (caballito.running())
    {
        caballito.showGrid();
        caballito.handleEvents();
        SDL_Delay(100);
    }

    caballito.quit();

    return 0;
}


