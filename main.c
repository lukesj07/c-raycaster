#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

#define HALF_FOV (PI / 4)
#define DEGREE (PI / 180)
#define PI 3.1415926536

#define TILE_SIZE 45

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

void drawMap2D(SDL_Renderer *renderer, int map[MAP_HEIGHT][MAP_WIDTH]);
void drawPlayer2D(SDL_Renderer *renderer, float pos[2], float direction);
void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void drawRays(SDL_Renderer *renderer, float pos[2], float direction);
float calculateDist(float p1[2], float p2[2]);

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

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        printf("Failed to create renderer\n");
        return -1;
    }

    struct playerState player;
    player.position[0] = SCREEN_WIDTH / 2;
    player.position[1] = SCREEN_HEIGHT / 2;
    player.direction = 0;
    player.health = 200;

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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawMap2D(renderer, map);
        drawPlayer2D(renderer, player.position, player.direction);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void drawMap2D(SDL_Renderer *renderer, int map[MAP_HEIGHT][MAP_WIDTH]) {
    int rectWidth = SCREEN_WIDTH / MAP_WIDTH;
    int rectHeight = SCREEN_HEIGHT / MAP_HEIGHT;
    SDL_Rect rect;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == 1) {
                rect.x = j * rectWidth;
                rect.y = i * rectHeight;
                rect.w = rectWidth - 1;
                rect.h = rectHeight - 1;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void drawPlayer2D(SDL_Renderer *renderer, float pos[2], float direction) {
    SDL_Rect rect;
    rect.x = (int) pos[0] - 5;
    rect.y = (int) pos[1] - 5;
    rect.w = 10;
    rect.h = 10;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    float dy = sin(direction) * 20;
    float dx = cos(direction) * 20;
    drawLine(renderer, pos[0], pos[1], pos[0] + dx, pos[1] + dy, (SDL_Color){255, 255, 0, 255});

    drawRays(renderer, pos, direction);
}

void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

float calculateDist(float pos[2], float intersection[2]) {
    return sqrt(pow(intersection[0] - pos[0], 2) + pow(intersection[1] - pos[1], 2));
}

void drawRays(SDL_Renderer *renderer, float pos[2], float direction) {
    float mid[2] = {pos[0], pos[1]}; // Middle of player position

    for (float a = direction - HALF_FOV; a <= direction + HALF_FOV; a += DEGREE) {
        float rayAngle = fmod(a + 2 * PI, 2 * PI);  // Ensure angle is within 0 to 2*PI
        float tanA = tan(rayAngle);
        float dy = sin(rayAngle);
        float dx = cos(rayAngle);

        // Calculate intersections with vertical lines
        float xs[2], incX[2];
        if (dx < 0) {
            xs[0] = floor(mid[0] / TILE_SIZE) * TILE_SIZE - 1;
            xs[1] = mid[1] + (mid[0] - xs[0]) * tanA;
            incX[0] = -TILE_SIZE;
            incX[1] = -TILE_SIZE * tanA;
        } else {
            xs[0] = floor(mid[0] / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
            xs[1] = mid[1] + (xs[0] - mid[0]) * tanA;
            incX[0] = TILE_SIZE;
            incX[1] = TILE_SIZE * tanA;
        }

        while (1) {
            int mapX = xs[0] / TILE_SIZE;
            int mapY = xs[1] / TILE_SIZE;
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                break;
            }
            if (map[mapY][mapX] == 1) {
                break;
            }
            xs[0] += incX[0];
            xs[1] += incX[1];
        }

        // Calculate intersections with horizontal lines
        float ys[2], incY[2];
        if (dy < 0) {
            ys[1] = floor(mid[1] / TILE_SIZE) * TILE_SIZE - 1;
            ys[0] = mid[0] + (mid[1] - ys[1]) / tanA;
            incY[1] = -TILE_SIZE;
            incY[0] = -TILE_SIZE / tanA;
        } else {
            ys[1] = floor(mid[1] / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
            ys[0] = mid[0] + (ys[1] - mid[1]) / tanA;
            incY[1] = TILE_SIZE;
            incY[0] = TILE_SIZE / tanA;
        }

        while (1) {
            int mapX = ys[0] / TILE_SIZE;
            int mapY = ys[1] / TILE_SIZE;
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                break;
            }
            if (map[mapY][mapX] == 1) {
                break;
            }
            ys[0] += incY[0];
            ys[1] += incY[1];
        }

        if (calculateDist(mid, xs) < calculateDist(mid, ys)) {
            drawLine(renderer, mid[0], mid[1], xs[0], xs[1], (SDL_Color){255, 0, 0, 255});
        } else {
            drawLine(renderer, mid[0], mid[1], ys[0], ys[1], (SDL_Color){255, 0, 0, 255});
        }
    }
}
