#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

#define HALF_FOV (PI / 6)
#define DEGREE (PI / 180)
#define PI 3.1415926536

#define TILE_SIZE 45
#define MAX_DIST 1000.0
#define MOVE_SPEED 300 // pixels per seconf
#define TURN_SPEED 3.0f // radians per second

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
};

void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, SDL_Color color);
void render3DView(SDL_Renderer *renderer, float pos[2], float direction);
float calculateDist(float p1[2], float p2[2]);
int nearestMultiple(float m);
int clamp(int value, int min, int max);
int checkCollision(float x, float y);
void movePlayer(struct playerState *player, float deltaX, float deltaY);

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

    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);
    Uint32 lastTime = SDL_GetTicks();
    int run = 1;
    while(run) {
        
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0) {
            if (e.type == SDL_QUIT) {
                run = 0;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f; //conv to secs
        lastTime = currentTime;     

        float deltaX = 0;
        float deltaY = 0;

        if (keyStates[SDL_SCANCODE_W]) {
            deltaX = cos(player.direction) * MOVE_SPEED * dt;
            deltaY = sin(player.direction) * MOVE_SPEED * dt;
            movePlayer(&player, deltaX, deltaY);
        }
        if (keyStates[SDL_SCANCODE_S]) {
            deltaX = -cos(player.direction) * MOVE_SPEED * dt;
            deltaY = -sin(player.direction) * MOVE_SPEED * dt;
            movePlayer(&player, deltaX, deltaY);
        }
        if (keyStates[SDL_SCANCODE_A]) {
            player.direction -= TURN_SPEED * dt;
            if (player.direction < 0) { player.direction += 2 * PI; }
        }
        if (keyStates[SDL_SCANCODE_D]) {
            player.direction += TURN_SPEED * dt;
            if (player.direction > 2 * PI) { player.direction -= 2 * PI; }
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

int checkCollision(float x, float y) {
    int mapX = (int)(x / TILE_SIZE);
    int mapY = (int)(y / TILE_SIZE);
    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
        return 1;
    }
    return map[mapY][mapX] == 1;
}

void movePlayer(struct playerState *player, float deltaX, float deltaY) {
    float newX = player->position[0] + deltaX;
    float newY = player->position[1] + deltaY;

    if (!checkCollision(newX, player->position[1])) {
        player->position[0] = newX;
    }
    if (!checkCollision(player->position[0], newY)) {
        player->position[1] = newY;
    }
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
    float cosA = cos(rayAngle);
    if (cosA > 0) {
        result[0] = floor(pos[0] / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
        result[1] = pos[1] + (result[0] - pos[0]) * tanA;
    } 
    if (cosA < 0){
        result[0] = floor(pos[0] / TILE_SIZE) * TILE_SIZE - 1;
        result[1] = pos[1] + (result[0] - pos[0]) * tanA;
    }
}

void calculateHorizontalIntersection(float pos[2], float rayAngle, float result[2]) {
    float tanA = tan(rayAngle);
    float sinA = sin(rayAngle);
    if (sinA > 0) {
        result[1] = floor(pos[1] / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
        result[0] = pos[0] + (result[1] - pos[1]) / tanA;
    }
    if (sinA < 0){
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
            float cosA = cos(rayAngle);
            if (cosA > 0) {
                verticalIntersection[0] += TILE_SIZE;
                verticalIntersection[1] += TILE_SIZE * tan(rayAngle);
            }
            if (cosA < 0) {
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
            float sinA = sin(rayAngle);
            if (sinA > 0) {
                horizontalIntersection[1] += TILE_SIZE;
                horizontalIntersection[0] += TILE_SIZE / tan(rayAngle);
            }
            if (sinA < 0) {
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

