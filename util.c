#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#include "utils.h"
#include "game.h"


const SDL_Color GRID_COLOR = {.r = 255, .g = 255, .b = 255};
const SDL_Color PLAYER_X_COLOR = {.r = 255, .g = 50, .b = 50};
const SDL_Color PLAYER_O_COLOR = {.r = 50, .g = 100, .b = 255};
const SDL_Color TIE_COLOR = {.r = 100, .g = 100, .b = 100};


void render_grid(SDL_Renderer *renderer, const SDL_Color *color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);
    for (int i = 1; i < N; i++)
    {
        SDL_RenderDrawLine(renderer,
                            i * CELL_WIDTH, 0,
                            i * CELL_WIDTH, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer,
                            0,            i * CELL_HEIGHT,
                            SCREEN_WIDTH, i * CELL_HEIGHT);
    }
}

void render_X(SDL_Renderer *renderer, int row, int column, const SDL_Color *color)
{

    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH*0.5 + column*CELL_WIDTH;
    const float center_y = CELL_HEIGHT*0.5 + row * CELL_HEIGHT;

    thickLineRGBA(renderer,
            center_x - half_box_side,
            center_y - half_box_side,
            center_x + half_box_side,
            center_y + half_box_side,
            10, color->r, color->g, color->b, 255);
    
    thickLineRGBA(renderer,
            center_x + half_box_side,
            center_y - half_box_side,
            center_x - half_box_side,
            center_y + half_box_side,
            10, color->r, color->g, color->b, 255);
}


void render_O(SDL_Renderer *renderer, int row, int column, const SDL_Color *color)
{
    const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
    const float center_x = CELL_WIDTH*0.5 + column*CELL_WIDTH;
    const float center_y = CELL_HEIGHT*0.5 + row * CELL_HEIGHT;

    filledCircleRGBA(renderer, center_x, center_y, half_box_side + 5,
            color->r, color->g, color->b, 255);
    filledCircleRGBA(renderer, center_x, center_y, half_box_side - 5,
            0, 0, 0, 255);
}


void render_board(SDL_Renderer *renderer, const int *board, 
        const SDL_Color *player_x_color, const SDL_Color *player_o_color)
{

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            switch (board[i*N +j]) {
                case PLAYER_X:
                    render_X(renderer, i, j, player_x_color);
                    break;

                case PLAYER_O:
                    render_O(renderer, i, j, player_o_color);
                    break;
                default:
                    {}
            }
        }
    }
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{

    render_grid(renderer, &GRID_COLOR);
    render_board(renderer, game->board, &PLAYER_X_COLOR, &PLAYER_O_COLOR);

}

void render_game_over(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color)
{
    render_grid(renderer, color);
    render_board(renderer, game->board, color, color);
}

void render_game(SDL_Renderer *renderer, const game_t *game){
    switch (game->state) {
        case RUNNING:
            render_running_state(renderer, game);
            break;
        case X_WON:
            render_game_over(renderer, game, &PLAYER_X_COLOR);
            break;
        case O_WON:
            render_game_over(renderer, game, &PLAYER_O_COLOR);
            break;
        case TIE:
            render_game_over(renderer, game, &TIE_COLOR);
            break;
        default: {}
    }
}




/*
 *
 * Games's functionality
 * 
 */



void switch_player_turn(game_t *game)
{

    if(game->player == PLAYER_X)
    {
        game->player = PLAYER_O;
    } else if (game->player == PLAYER_O){
        game->player = PLAYER_X;
    }

}

int check_player_won(game_t *game, int player)
{

    int row_count = 0;
    int column_count = 0;
    int diag1_cont = 0;
    int diag2_cont = 0;

    for (int i =0; i< N; i++)
    {
        for (int j = 0; j < N; ++j)
        {
            if(game->board[i*N +j] == player){
                row_count++;
            }
            if(game->board[j*N + i] == player)
            {
                column_count++;
            }
        }
        if(row_count >= N || column_count >= N)
        {
            return 1;
        }
        row_count = 0;
        column_count = 0;

        if(game->board[i*N + i] == player)
        {
            diag1_cont++;
        }
        if(game->board[i*N + (N - i -1)])
        {
            diag2_cont++;
        }
    }
    return diag1_cont >= N || diag2_cont >= N;
}


int count_cells(const int *board, int cell)
{
    int count = 0;
    for (int i = 0; i < N * N; ++i)
    {
        if(board[i] == cell)
        {
            count++;
        }
    }
    return count;
}


void game_over_condition(game_t *game)
{
    if (check_player_won(game, PLAYER_X))
    {
        game->state = X_WON;
    }
    else if (check_player_won(game, PLAYER_O))
    {
        game->state = O_WON;
    }
    else if (count_cells(game->board, EMPTY) == 0)
    {
        game->state = TIE;
    }
}


void player_turn(game_t *game, int row, int column)
{
    if (game->board[row * N + column] == EMPTY)
    {
        game->board[row * N + column] = game->player;
        switch_player_turn(game);
        game_over_condition(game);
    }
}

void reset_game(game_t *game)
{
    game->player = PLAYER_X;
    game->state = RUNNING;
    for (int i = 0; i < N * N; ++i)
    {
        game->board[i] = EMPTY;
    }
}

void cell_on_click(game_t *game, int row, int column)
{
    if (game->state == RUNNING) {
        player_turn(game, row, column);
    } else {
        reset_game(game);
    }
}
