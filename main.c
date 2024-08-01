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
int nearestDirectionalMultiple(float m, float n, int dir);
float calculateDist(int p1[2], int p2[2]);

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
    return 0;
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

int nearestDirectionalMultiple(float m, float n, int dir) {
  // where y is 45  
  int nearest = (m / n) * n;
  return dir == -1 ? nearest : nearest + n; 
}

float calculateDist(int p1[2], int p2[2]) {
    return sqrt(pow((p1[0] - p2[0]), 2) + pow((p1[1] - p2[1]), 2));
}

void drawRays(float pos[2], float direction, SDL_Rect rect, SDL_Surface *surface) {
    
    float dy, dx;
    int mid[2];
    mid[0] = pos[0]+(rect.w/2);
    mid[1] = pos[1]+(rect.h/2);

    for (float a = direction-HALF_FOV; a <= direction+HALF_FOV; a += DEGREE) {
        dy = sin(a);
        dx = cos(a);

        int xs[2]; // x, y pair denoting nearest intersection with a vertical line and used to track vert line search
        int incX[2];
        if (dx < 0) {
            xs[0] = nearestDirectionalMultiple(mid[0], 45, -1);
            xs[1] = abs(xs[0] - mid[0])*tan(a);
            incX[0] = -45;
        }
        if (dx > 0) {
            xs[0] = nearestDirectionalMultiple(mid[0], 45, 1);
            xs[1] = abs(xs[0] - mid[0])*tan(a);
            incX[0] = 45;
        }
        incX[1] = 45*tan(a);
        
        while (1) {
            int mapX = xs[0]/45;
            int mapY = xs[1]/45;
            if (cos(a) < 0) {mapX--;}
            if (sin(a) > 0) {mapY--;}
            
            if (map[mapY][mapX] == 1) {break;}
            xs[0] += incX[0];
            xs[1] += incX[1];
        }



        int ys[2]; // x, y pair denoting nearest intersection with a horizontal line and used to track horiz line search
        int incY[2];
        if (dy < 0) {
            ys[1] = nearestDirectionalMultiple(mid[1], 45, -1);
            ys[0] = abs(ys[1] - mid[1])/tan(a);
            incY[1] = -45;
        }
        if (dy > 0) {
            ys[1] = nearestDirectionalMultiple(mid[1], 45, 1);
            ys[0] = abs(ys[1] - mid[1])/tan(a);
            incY[1] = 45;
        }
        incY[0] = 45/tan(a);

        while (1) {
            int mapX = ys[0]/45;
            int mapY = ys[1]/45;
            if (cos(a) < 0) {mapX--;}
            if (sin(a) > 0) {mapY--;}
            
            if (map[mapY][mapX] == 1) {break;}
            ys[0] += incY[0];
            ys[1] += incY[1];
        }

        if (calculateDist(mid, xs) < calculateDist(mid, ys)) {
            drawLine(surface, mid[0], mid[1], xs[0], xs[1], SDL_MapRGB(surface->format, 255, 0, 0));
        } else {
            drawLine(surface, mid[0], mid[1], ys[0], ys[1], SDL_MapRGB(surface->format, 255, 0, 0));
        }

        // drawLine(surface, midx, midy, midx+dx, midy+dy, SDL_MapRGB(surface->format, 255, 0, 0));
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
