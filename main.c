#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

#define HALF_FOV 0.785398
#define DEGREE 0.0174532925
#define PI 3.1415926536

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct playerState {
    float position[2];
    float direction;
    int health;
};

void drawMap2D(int map[MAP_HEIGHT][MAP_WIDTH], SDL_Surface *surface);
void drawPlayer2D(float pos[2], float direction, SDL_Surface *surface);
void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);
void drawRays(float pos[2], float direction, SDL_Rect rect, SDL_Surface *surface);
int** generateWallPositions();
void freeWallPositions(int** positions);


int main() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if(!window) {
        printf("Failed to create window\n");
        return -1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    if(!surface) {
        printf("Failed to get the surface from the window\n");
        return -1;
    }

    struct playerState player;
    player.position[0] = SCREEN_WIDTH / 2;
    player.position[1] = SCREEN_HEIGHT / 2;
    player.direction = 0;
    player.health = 200;

    int** wallPositions = generateWallPositions();

    int run = 1;
    while(run) {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                run = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:
                        player.direction -= 0.1f;
                        if (player.direction < 0) {player.direction += 2*PI;}
                        break;
                    case SDLK_RIGHT:
                        player.direction += 0.1f;
                        if (player.direction > 2*PI) {player.direction -= 2*PI;}
                        break;
                    case SDLK_UP:
                        player.position[0] += cos(player.direction) * 5;
                        player.position[1] += sin(player.direction) * 5;
                        break;
                    case SDLK_DOWN:
                        player.position[0] -= cos(player.direction) * 5;
                        player.position[1] -= sin(player.direction) * 5;
                        break;
                }
            }
        }
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));    

        drawMap2D(map, surface);
        drawPlayer2D(player.position, player.direction, surface);
        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    freeWallPositions(wallPositions);
    return 0;
}


int** generateWallPositions() {
    int count = 0;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == 1) {
                count++;
            }
        }
    }

    int** positions = malloc(count * sizeof(int*));
    if (positions == NULL) {
        perror("Failed to allocate memory for positions");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == 1) {
                positions[index] = malloc(2 * sizeof(int));
                if (positions[index] == NULL) {
                    perror("Failed to allocate memory for position");
                    exit(EXIT_FAILURE);
                }
                positions[index][0] = i;
                positions[index][1] = j;
                index++;
            }
        }
    }

    return positions;
}

void freeWallPositions(int** positions) {
    free(*positions);
    free(positions);
}

void drawMap2D(int map[MAP_HEIGHT][MAP_WIDTH], SDL_Surface *surface) {
    int rectWidth = SCREEN_WIDTH / MAP_WIDTH;
    int rectHeight = SCREEN_HEIGHT / MAP_HEIGHT;
    SDL_Rect rect;
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == 1) {
                rect.x = j * rectWidth;
                rect.y = i * rectHeight;
                rect.w = rectWidth - 1;
                rect.h = rectHeight - 1;
                SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 255));
            }
        }
    }
}

void drawPlayer2D(float pos[2], float direction, SDL_Surface *surface) {
    SDL_Rect rect;
    rect.x = (int) pos[0];
    rect.y = (int) pos[1];
    rect.w = 10;
    rect.h = 10;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 0));

    float dy = sin(direction) * 20;
    float dx = cos(direction) * 20;
    int midx = pos[0]+(rect.w/2);
    int midy = pos[1]+(rect.h/2);
    drawLine(surface, midx, midy, midx+dx, midy+dy, SDL_MapRGB(surface->format, 255, 255, 0));

    drawRays(pos, direction, rect, surface);
}

void drawRays(float pos[2], float direction, SDL_Rect rect, SDL_Surface *surface) {
    float dy, dx;
    int midx = pos[0]+(rect.w/2);
    int midy = pos[1]+(rect.h/2);
    for (float a = direction-HALF_FOV; a <= direction+HALF_FOV; a += DEGREE*4) {
        dy = sin(a) * 80;
        dx = cos(a) * 80;
        drawLine(surface, midx, midy, midx+dx, midy+dy, SDL_MapRGB(surface->format, 255, 0, 0));
    }
}

void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    for (;;) {
        ((Uint32 *)surface->pixels)[y1 * surface->w + x1] = color;
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}
