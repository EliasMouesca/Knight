#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <queue>
#include <string>
#include <iostream>

class Caballito
{
    public:
        // Attr

        // Methods
        Caballito(int r, int c, int w=600, int h=600);
        bool setFirst(int x, int y);
        void calculateGrid();
        void renderGrid();
        void showGrid();
        bool running() { return isRunning; };
        void handleEvents();
        void quit();

    private:
        void renderText(const std::string &text, int x, int y, SDL_Color color);

        int windowWidth;
        int windowHeight;

        SDL_Point first = {0, 0};

        int rows;
        int cols;

        int cellWidth;
        int cellHeight;

        int* grid; // Array of all numbers, you divide by row size;

        bool isRunning = false;

        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Texture* windowTexture = NULL;

};
