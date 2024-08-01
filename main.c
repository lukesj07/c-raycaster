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
#define MAX_DIST 1000.0

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
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

void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void render3DView(SDL_Renderer *renderer, float pos[2], float direction);
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
        render3DView(renderer, player.position, player.direction);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

//3D and helper functions
void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

float calculateDist(float pos[2], float intersection[2]) {
    return sqrt(pow(intersection[0] - pos[0], 2) + pow(intersection[1] - pos[1], 2));
}

void calculateVerticalIntersection(float pos[2], float rayAngle, float result[2]) {
    float tanA = tan(rayAngle);
    if (cos(rayAngle) > 0) {
        result[0] = floor(pos[0] / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
        result[1] = pos[1] + (result[0] - pos[0]) * tanA;
    } else {
        result[0] = floor(pos[0] / TILE_SIZE) * TILE_SIZE - 1;
        result[1] = pos[1] + (result[0] - pos[0]) * tanA;
    }
}

void calculateHorizontalIntersection(float pos[2], float rayAngle, float result[2]) {
    float tanA = tan(rayAngle);
    if (sin(rayAngle) > 0) {
        result[1] = floor(pos[1] / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
        result[0] = pos[0] + (result[1] - pos[1]) / tanA;
    } else {
        result[1] = floor(pos[1] / TILE_SIZE) * TILE_SIZE - 1;
        result[0] = pos[0] + (result[1] - pos[1]) / tanA;
    }
}

void render3DView(SDL_Renderer *renderer, float pos[2], float direction) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = direction - HALF_FOV + (x * (HALF_FOV * 2) / SCREEN_WIDTH);
        rayAngle = fmod(rayAngle + 2 * PI, 2 * PI);

        float verticalIntersection[2], horizontalIntersection[2];

        // Calculate vertical intersection
        calculateVerticalIntersection(pos, rayAngle, verticalIntersection);
        while (1) {
            int mapX = verticalIntersection[0] / TILE_SIZE;
            int mapY = verticalIntersection[1] / TILE_SIZE;
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                break;
            }
            if (map[mapY][mapX] == 1) {
                break;
            }
            if (cos(rayAngle) > 0) {
                verticalIntersection[0] += TILE_SIZE;
                verticalIntersection[1] += TILE_SIZE * tan(rayAngle);
            } else {
                verticalIntersection[0] -= TILE_SIZE;
                verticalIntersection[1] -= TILE_SIZE * tan(rayAngle);
            }
        }

        // Calculate horizontal intersection
        calculateHorizontalIntersection(pos, rayAngle, horizontalIntersection);
        while (1) {
            int mapX = horizontalIntersection[0] / TILE_SIZE;
            int mapY = horizontalIntersection[1] / TILE_SIZE;
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
                break;
            }
            if (map[mapY][mapX] == 1) {
                break;
            }
            if (sin(rayAngle) > 0) {
                horizontalIntersection[1] += TILE_SIZE;
                horizontalIntersection[0] += TILE_SIZE / tan(rayAngle);
            } else {
                horizontalIntersection[1] -= TILE_SIZE;
                horizontalIntersection[0] -= TILE_SIZE / tan(rayAngle);
            }
        }

        float dist;
        int redVal;
        if (calculateDist(pos, verticalIntersection) < calculateDist(pos, horizontalIntersection)) {
            dist = calculateDist(pos, verticalIntersection);
            redVal = 255;
        } else {
            dist = calculateDist(pos, horizontalIntersection);
            redVal = 150;
        }

        // fisheye fix
        dist *= cos(rayAngle - direction);

        float lineHeight = (TILE_SIZE * SCREEN_HEIGHT) / dist;
        float drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        float drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        SDL_SetRenderDrawColor(renderer, redVal, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}
