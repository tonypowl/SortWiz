// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Function declaration: draws bars on the screen
// highlight1 and highlight2 are optional indices to highlight in red
void renderBars(SDL_Window* window, const std::vector<int>& array, int highlight1 = -1, int highlight2 = -1);



int main(int argc, char* args[])
{
    // Pointer to the SDL window
    SDL_Window* window = nullptr;

    // Pointer to the window surface (the pixel buffer we draw on)
    SDL_Surface* surface = nullptr;

    // Initialize SDL (video subsystem only)
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create the SDL window
    window = SDL_CreateWindow("Sorting Visualizer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    // Handle window creation failure
    if (!window) {
        printf("Window creation failed! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Get the surface from the window for pixel rendering
    surface = SDL_GetWindowSurface(window);

    // Seed random number generator
    srand(time(0));

    // Create an array of random integers (values between 0 and SCREEN_HEIGHT)
    std::vector<int> array(30);
    for (int& val : array)
        val = std::rand() % SCREEN_HEIGHT;

    // Render the initial bar chart
    renderBars(window, array);

    SDL_Event e; 
    bool quit = false;
    int hoveredBar = -1; 

    while(!quit){
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT){
                quit = true; 
            } else if (e.type == SDL_MOUSEMOTION){
                int mouseX = e.motion.x; 
                int mouseY = e.motion.y; 

                int barWidth = SCREEN_WIDTH / array.size(); 
                int index = mouseX/barWidth;

                if(index>=0 && index<array.size()){
                    int maxVal = *std::max_element(array.begin(), array.end());
                    int barHeight = (array[index] * SCREEN_HEIGHT)/maxVal; 

                    if (mouseY >= SCREEN_HEIGHT- barHeight){
                        hoveredBar = index; 
                    } else { 
                        hoveredBar = -1;
                    }
                } else {
                        hoveredBar = -1; 
                }

                renderBars(window, array, hoveredBar); 
            }
        }
}

    // Cleanup and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

// Function to render the bar array on the screen
void renderBars(SDL_Window* window, const std::vector<int>& array, int highlight1, int highlight2)
{
    // Get the surface associated with the window
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    // Fill the surface black before drawing new bars (clears the screen)
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Calculate width of each bar
    int barWidth = surface->w / array.size();

    // Find max value in array for normalizing height
    int maxVal = *std::max_element(array.begin(), array.end());

    // Draw each bar
    for (int i = 0; i < array.size(); ++i) {
        // Calculate height of bar (scaled to screen height)
        int barHeight = (array[i] * surface->h) / maxVal;

        // Create rectangle to represent the bar
        SDL_Rect bar = {
            i * barWidth,               // x position
            surface->h - barHeight,     // y position (bottom-aligned)
            barWidth,                   // width
            barHeight                   // height
        };

        // Determine bar color
        Uint32 color;
        if (i == highlight1 || i == highlight2) {
            color = SDL_MapRGB(surface->format, 255, 0, 0); // red if highlighted
        } else {
            color = SDL_MapRGB(surface->format, 0, 255, 0); // green otherwise
        }

        // Fill the rectangle with the chosen color
        SDL_FillRect(surface, &bar, color);
    }

    // Update the window with the new surface content
    SDL_UpdateWindowSurface(window);
}
