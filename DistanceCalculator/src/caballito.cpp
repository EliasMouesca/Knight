#include "caballito.h"

#define WINDOW_TITLE "DEMO"

class Cell
{
    public:
    int x, y, dist;
    Cell(int x, int y, int dist) : x(x), y(y), dist(dist) {}

};

// TODO: create this procedurally!
SDL_Color colors[] = {
    {0xff, 0xff, 0xff, 0xff},       // 0
    {0xdd, 0x00, 0x00, 0xff},       // 1
    {0x00, 0xdd, 0x00, 0xff},       // 2
    {0x00, 0x00, 0xdd, 0xff},       // 3
    {0xdd, 0xdd, 0x00, 0xff},       // 4
    {0xdd, 0x00, 0xdd, 0xff},       // 5
    {0x00, 0xdd, 0xdd, 0xff},       // 6
    {0xaa, 0xbb, 0xcc, 0xff},       // 7
    {0xcc, 0xbb, 0xaa, 0xff},       // 8
    {0xcc, 0xaa, 0xbb, 0xff}        // 9
};

bool setColor(SDL_Renderer* renderer, SDL_Color color);

Caballito::Caballito(int r, int c, int w, int h)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    printf("Initializing...\n\nr: %d, c: %d, w: %d, h: %d\n", 
            r, c, w, h);

    windowWidth = w - w % c;
    windowHeight = h - h % r;

    rows = r;
    cols = c;

    cellWidth = windowWidth / cols;
    cellHeight = windowHeight / rows;

    printf("Cell size: %d * %d\n", cellWidth, cellHeight);

    window = SDL_CreateWindow(
            WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight, 0);

    if (window != NULL)
        printf("Created window with w=%d, h=%d\n", windowWidth, windowHeight);

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer != NULL) puts("Created renderer succesfully");

    windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
    if (windowTexture != NULL) puts("Created texture succesfully\n");

    grid = (int*) calloc(rows * cols, sizeof(int));

    isRunning = true;

    return;
}

bool Caballito::setFirst(int x, int y)
{
    if (x < 0 || x >= cols) return false;
    if (y < 0 || y >= rows) return false;

    first.x = x;
    first.y = y;

    return true;
}

void Caballito::calculateGrid()
{
    // Knight movement
    int dx[] = {-2, -1, 1, 2, -2, -1, 1, 2};
    int dy[] = {-1, -2, -2, -1, 1, 2, 2, 1};

    std::queue<Cell> q;

    for (int i = 0; i < rows * cols; i++)
        grid[i] = -1;

    grid[first.y * cols + first.x] = 0;

    q.push(Cell(first.x, first.y, 0));

    while (!q.empty())
    {
        Cell currentCell = q.front();
        q.pop();

        for (int i = 0; i < 8; i++)
        {
            int newX = currentCell.x + dx[i];
            int newY = currentCell.y + dy[i];

            if (newX >= 0 && newY >= 0 && newX < cols && newY < rows && grid[newY * cols + newX] == -1)
            {
                grid[newY * cols + newX] = currentCell.dist + 1;
                q.push(Cell(newX, newY, currentCell.dist + 1));
            }
        }
    }

    return;
}    

void Caballito::renderGrid()
{
    SDL_Color white = {0xff, 0xff, 0xff, 0xff};
    SDL_SetRenderTarget(renderer, windowTexture);

    setColor(renderer, white);

    SDL_RenderClear(renderer);

    for (int i = 0; i < rows * cols; i++)
    {
        if (grid[i] >= 0 && grid[i] <= 9)
            setColor(renderer, colors[grid[i]]);
        else 
        {
            SDL_Color color = {0, 0, 0, 0};
            setColor(renderer, color);
        }

        int r = i / cols;
        int c = i % cols;

        SDL_Rect rect = {c * cellWidth, r * cellHeight, cellWidth, cellHeight};

        SDL_RenderFillRect(renderer, &rect);

        int centerX = rect.x + rect.w / 2;
        int centerY = rect.y + rect.h / 2;

        std::string text = std::to_string(grid[i]);
        renderText(text, centerX, centerY, white);


    }

    SDL_RenderPresent(renderer);
}

void Caballito::renderText(const std::string &text, int x, int y, SDL_Color color)
{
    TTF_Font *font = TTF_OpenFont("font.ttf", 24);

    if (!font)
    {
        // Handle font loading failure
        std::cerr << "Failed to load font! Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a surface from the text
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface)
    {
        // Handle surface creation failure
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    // Create a texture from the surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        // Handle texture creation failure
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    // Get the width and height of the rendered text
    int textWidth = surface->w;
    int textHeight = surface->h;

    // Set the position to render the text
    SDL_Rect renderQuad = {x - textWidth / 2, y - textHeight / 2, textWidth, textHeight};

    // Render the text
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

bool setColor(SDL_Renderer* renderer, SDL_Color color)
{
    if (renderer == NULL) return false;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    return true;
}

void Caballito::showGrid()
{
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, windowTexture, NULL, NULL);

    SDL_RenderPresent(renderer);
    
    return;
}

void Caballito::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
        if (event.type == SDL_QUIT) isRunning = false;

    return;
}

void Caballito::quit()
{
    free(grid);
    if (windowTexture != NULL) SDL_DestroyTexture(windowTexture);
    if (window != NULL) SDL_DestroyWindow(window);
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
}

