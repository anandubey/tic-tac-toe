#ifndef __UTIL_H__
#define __UTIL_H__


void render_game(SDL_Renderer *renderer, const game_t *game);

void cell_on_click(game_t *game, int row, int column);

#endif    //__UTIL_H__
