#include <SDL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
/*
    The SDL GUI is based of the project by catsock on github.
    URL: https://github.com/catsocks/sdl-grid
*/
struct NODE {
    int g; // Distance from start to current node
    int h; // Estimated distance to end node
    int f; // Total cost of the node

    int blocked;
    int path;
};


int startx = 1;
int starty = 1;

int endx = 49;
int endy = 49;

int main()
{
    const int grid_cell_size = 15;
    const int grid_width = 50;
    const int grid_height = 50;

    // + 1 so that the last grid lines fit in the screen.
    int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;

    // Place the grid cursor in the middle of the screen.
    SDL_Rect grid_cursor = {
        .x = (grid_width - 1) / 2 * grid_cell_size,
        .y = (grid_height - 1) / 2 * grid_cell_size,
        .w = grid_cell_size,
        .h = grid_cell_size,
    };

    // The cursor ghost is a cursor that always shows in the cell below the
    // mouse cursor.
    SDL_Rect grid_cursor_ghost = { grid_cursor.x, grid_cursor.y, grid_cell_size,
                                  grid_cell_size };
    // Grid colours
    SDL_Color grid_background = { 22, 22, 22, 255 }; // Barely Black
    SDL_Color grid_line_color = { 44, 44, 44, 255 }; // Dark grey
    SDL_Color grid_cursor_ghost_color = { 44, 44, 44, 255 };
    SDL_Color grid_cursor_color = { 255, 255, 255, 255 }; // White
    
    // A* colours
    SDL_Color grid_start_color = { 0, 69, 255, 255 }; // Orange
    SDL_Color grid_end_color = { 0, 69, 255, 255 }; // Blue
    SDL_Color grid_path_color = { 120, 0, 255, 255 }; // Orange
    SDL_Color grid_in_queue_color = { 89, 255, 0, 255 }; // Green
    SDL_Color grid_searched_color = { 255, 0, 0, 255 }; // Green
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s",
            SDL_GetError());
        return 1;
    }

    printf("SDL has been sucessfully been initialised\n");
    SDL_Window* window;
    SDL_Renderer* renderer;
    if (SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,
        &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Create window and renderer: %s", SDL_GetError());
        return 1;
    }
    printf("Window has been sucessfully been initialised\n");

    SDL_SetWindowTitle(window, "A* path finder By Jimmy Lin");

    SDL_bool quit = SDL_FALSE;
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;
                break;
            case SDL_MOUSEMOTION:
                grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                if (!mouse_active)
                    mouse_active = SDL_TRUE;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                    mouse_hover = SDL_TRUE;
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                    mouse_hover = SDL_FALSE;
                break;
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            }
        }

        // Draw grid background.
        SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g,
            grid_background.b, grid_background.a);
        SDL_RenderClear(renderer);

        // Draw grid lines.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g,
            grid_line_color.b, grid_line_color.a);

        for (int x = 0; x < 1 + grid_width * grid_cell_size;
            x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }

        for (int y = 0; y < 1 + grid_height * grid_cell_size;
            y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Draw grid ghost cursor.
        if (mouse_active && mouse_hover) {
            SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r,
                grid_cursor_ghost_color.g,
                grid_cursor_ghost_color.b,
                grid_cursor_ghost_color.a);
            SDL_RenderFillRect(renderer, &grid_cursor_ghost);
        }

        // Draw grid cursor.
        SDL_SetRenderDrawColor(renderer, grid_cursor_color.r,
            grid_cursor_color.g, grid_cursor_color.b,
            grid_cursor_color.a);
        SDL_RenderFillRect(renderer, &grid_cursor);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

