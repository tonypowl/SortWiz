#include <SDL2/SDL.h>
#include <iostream>

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create SDL Window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Sorting Visualizer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create SDL Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Event loop
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        // Clear the screen with black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw something (red background rectangle as placeholder)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = {100, 100, 600, 400};
        SDL_RenderFillRect(renderer, &rect);

        // Present the renderer
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
