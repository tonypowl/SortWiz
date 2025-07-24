#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MIN_BARS = 5;
const int MAX_BARS = 100;

// Function declarations
void renderBars(SDL_Window* window, const std::vector<int>& array, int highlight1 = -1, int highlight2 = -1);
std::vector<int> generateRandomArray(int size);

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Sorting Visualizer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    srand(static_cast<unsigned int>(time(0)));

    int numBars = 30;
    std::vector<int> array = generateRandomArray(numBars);
    renderBars(window, array);

    SDL_Event e;
    bool quit = false;
    int hoveredBar = -1;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEMOTION: {
                    int mouseX = e.motion.x;
                    int mouseY = e.motion.y;

                    int barWidth = SCREEN_WIDTH / array.size();
                    int index = mouseX / barWidth;

                    if (index >= 0 && index < array.size()) {
                        int maxVal = *std::max_element(array.begin(), array.end());
                        int barHeight = (array[index] * SCREEN_HEIGHT) / maxVal;

                        if (mouseY >= SCREEN_HEIGHT - barHeight) {
                            hoveredBar = index;
                        } else {
                            hoveredBar = -1;
                        }
                    } else {
                        hoveredBar = -1;
                    }

                    renderBars(window, array, hoveredBar);
                    break;
                }

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_RIGHT:
                            if (numBars < MAX_BARS) {
                                numBars += 5;
                                array = generateRandomArray(numBars);
                                hoveredBar = -1;
                                renderBars(window, array);
                            }
                            break;

                        case SDLK_LEFT:
                            if (numBars > MIN_BARS) {
                                numBars -= 5;
                                array = generateRandomArray(numBars);
                                hoveredBar = -1;
                                renderBars(window, array);
                            }
                            break;

                        case SDLK_r:
                            array = generateRandomArray(numBars);
                            hoveredBar = -1;
                            renderBars(window, array);
                            break;
                    }
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// Generate random array
std::vector<int> generateRandomArray(int size) {
    std::vector<int> array(size);
    for (int& val : array) {
        val = std::rand() % SCREEN_HEIGHT;
    }
    return array;
}

// Render the bars
void renderBars(SDL_Window* window, const std::vector<int>& array, int highlight1, int highlight2) {
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    int barWidth = surface->w / array.size();
    int maxVal = *std::max_element(array.begin(), array.end());

    for (int i = 0; i < array.size(); ++i) {
        int barHeight = (array[i] * surface->h) / maxVal;
        SDL_Rect bar = {
            i * barWidth,
            surface->h - barHeight,
            barWidth,
            barHeight
        };

        Uint32 color = SDL_MapRGB(surface->format, 0, 255, 0); // green
        if (i == highlight1 || i == highlight2) {
            color = SDL_MapRGB(surface->format, 255, 0, 0); // red
        }

        SDL_FillRect(surface, &bar, color);
    }

    SDL_UpdateWindowSurface(window);
}
