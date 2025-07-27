#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MIN_BARS = 5;
const int MAX_BARS = 100;

// Function declarations
void renderBars(SDL_Window* window, const std::vector<int>& array, int highlight1 = -1, int highlight2 = -1);
std::vector<int> generateRandomArray(int size);
void showInstructions();
void showInstructionsOverlay(SDL_Window* instructWindow);

int main(int argc, char* args[]) {
    SDL_Window* window = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Show instructions first
    showInstructions();

    window = SDL_CreateWindow("Sorting Visualizer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
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
    bool showingInstructions = false;
    SDL_Window* instructWindow = nullptr;

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

                        case SDLK_i:
                            if (!showingInstructions) {
                                showingInstructions = true;
                                instructWindow = SDL_CreateWindow("Instructions",
                                                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                                  400, 250,
                                                                  SDL_WINDOW_SHOWN);
                                if (instructWindow) {
                                    showInstructionsOverlay(instructWindow);
                                }
                            }
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    switch (e.key.keysym.sym) {
                        case SDLK_i:
                            if (showingInstructions && instructWindow) {
                                SDL_DestroyWindow(instructWindow);
                                instructWindow = nullptr;
                                showingInstructions = false;
                            }
                            break;
                    }
                    break;
            }
        }
    }

    // Clean up instruction window if still open
    if (instructWindow) {
        SDL_DestroyWindow(instructWindow);
    }

    SDL_DestroyWindow(window);
    TTF_Quit();
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

// Show instructions in a small window
void showInstructions() {
    SDL_Window* instructWindow = SDL_CreateWindow("Instructions",
                                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                  400, 250,
                                                  SDL_WINDOW_SHOWN);
    if (!instructWindow) {
        printf("Instruction window creation failed! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(instructWindow);
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Arial.ttf", 18);
    
    // Fallback fonts for different systems
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    }
    if (!font) {
        font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 18);
    }
    
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyWindow(instructWindow);
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255}; // White text
    SDL_Color backgroundColor = {0, 0, 0, 255}; // Black background

    // Clear background
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Instructions text
    std::vector<std::string> instructions = {
        "Sorting Visualizer Controls:",
        "",
        "Right Arrow: Increase array size (+5)",
        "Left Arrow: Decrease array size (-5)",
        "R key: Randomize array",
        "",
        "Press any key to continue..."
    };

    int yOffset = 20;
    for (const std::string& text : instructions) {
        if (!text.empty()) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
            if (textSurface) {
                SDL_Rect textRect = {20, yOffset, textSurface->w, textSurface->h};
                SDL_BlitSurface(textSurface, NULL, surface, &textRect);
                SDL_FreeSurface(textSurface);
            }
        }
        yOffset += 25;
    }

    SDL_UpdateWindowSurface(instructWindow);

    // Wait for user input
    SDL_Event e;
    bool waiting = true;
    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN) {
                waiting = false;
            }
        }
        SDL_Delay(10);
    }

    TTF_CloseFont(font);
    SDL_DestroyWindow(instructWindow);
}

// Show instructions overlay (called when 'i' is pressed)
void showInstructionsOverlay(SDL_Window* instructWindow) {
    if (!instructWindow) return;
    
    SDL_Surface* surface = SDL_GetWindowSurface(instructWindow);
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Arial.ttf", 18);
    
    // Fallback fonts for different systems
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    }
    if (!font) {
        font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 18);
    }
    
    if (!font) {
        return;
    }

    SDL_Color textColor = {255, 255, 255, 255}; // White text

    // Clear background
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Instructions text
    std::vector<std::string> instructions = {
        "Sorting Visualizer Controls:",
        "",
        "Right Arrow: Increase array size (+5)",
        "Left Arrow: Decrease array size (-5)",
        "R key: Randomize array",
        "I key: Show/hide instructions",
        "",
        "Release 'I' key to close this window"
    };

    int yOffset = 20;
    for (const std::string& text : instructions) {
        if (!text.empty()) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
            if (textSurface) {
                SDL_Rect textRect = {20, yOffset, textSurface->w, textSurface->h};
                SDL_BlitSurface(textSurface, NULL, surface, &textRect);
                SDL_FreeSurface(textSurface);
            }
        }
        yOffset += 25;
    }

    SDL_UpdateWindowSurface(instructWindow);
    TTF_CloseFont(font);
}
