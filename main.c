#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "game.h"
#include "util.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO != 0))
    {
        fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(
                            "Tic Tac Toe",
                            100, 100, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN
                        );
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(renderer == NULL)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    game_t game = {
        .board = {
            EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY
        },
        .player = PLAYER_X,
        .state = RUNNING
    };


   SDL_Event e;

    while (game.state != END)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type) {
                case SDL_QUIT:
                    game.state = END;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    cell_on_click(&game, e.button.y/CELL_HEIGHT, e.button.x/CELL_WIDTH);
                    break;

                default: {}
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
