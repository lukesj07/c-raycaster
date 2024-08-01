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

void drawMap2D(SDL_Renderer *renderer, int map[MAP_HEIGHT][MAP_WIDTH]);
void drawPlayer2D(SDL_Renderer *renderer, float pos[2], float direction);
void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void drawRays(SDL_Renderer *renderer, float pos[2], float direction);
int nearestDirectionalMultiple(float m, float n, int dir);
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

int nearestDirectionalMultiple(float m, float n, int dir) {
  // where y is 45  
  int nearest = (m / n) * n;
  return dir == -1 ? nearest : nearest + n; 
}

float calculateDist(float p1[2], float p2[2]) {
    return sqrt(pow((p1[0] - p2[0]), 2) + pow((p1[1] - p2[1]), 2));
}

void drawRays(SDL_Renderer *renderer, float pos[2], float direction) {
    float dy, dx;
    float mid[2] = {pos[0], pos[1]};
    float a = direction;
    
    float tanA = tan(a);
    if (fabs(tanA) < 1e-6) {
        return;
    }

    for (int i = 0; i < 1; i++) {
        dy = sin(a);
        dx = cos(a);

        // Calculate intersections with vertical lines
        float xs[2], incX[2];
        if (dx < 0) {
            xs[0] = (mid[0] / 45) * 45;
            xs[1] = mid[1] + (mid[0] - xs[0]) * tanA;
            incX[0] = -45;
        } else {
            xs[0] = ((mid[0] / 45) + 1) * 45;
            xs[1] = mid[1] + (xs[0] - mid[0]) * tanA;
            incX[0] = 45;
        }
        incX[1] = 45 * tanA;

        while (1) {
            int mapX = xs[0] / 45;
            int mapY = xs[1] / 45;
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
            ys[1] = (mid[1] / 45) * 45;
            ys[0] = mid[0] + (mid[1] - ys[1]) / tanA;
            incY[1] = -45;
        } else {
            ys[1] = ((mid[1] / 45) + 1) * 45;
            ys[0] = mid[0] + (ys[1] - mid[1]) / tanA;
            incY[1] = 45;
        }
        incY[0] = 45 / tanA;

        while (1) {
            int mapX = ys[0] / 45;
            int mapY = ys[1] / 45;
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

        // drawLine(renderer, mid[0], mid[1], mid[0] + dx * 100, mid[1] + dy * 100, (SDL_Color){255, 0, 0, 255});
    }
}
