#include <SDL2/SDL.h>
#include <stdio.h>

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);

    if(!window) {
        printf("Failed to create window\n");
        return -1;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface) {
        printf("Failed to get the surface from the window\n");
        return -1;
    }

    int run = 1;
    while(run) {
      SDL_Event e;
      while(SDL_PollEvent(&e) > 0) {
        switch(e.type) {
            case SDL_QUIT:
                run = 0;
                break;
        }
        
        SDL_UpdateWindowSurface(window);
      }
    }

    return 0;
}

