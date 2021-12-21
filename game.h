#ifndef __GAME_H__
#define __GAME_H__

#define N 3
#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0
#define CELL_WIDTH (SCREEN_WIDTH/N)
#define CELL_HEIGHT (SCREEN_HEIGHT/N)
 
enum GAME_STATE{
    RUNNING,
    X_WON,
    O_WON,
    TIE,
    END
};

enum CELL_STATE {
    EMPTY,
    PLAYER_X,
    PLAYER_O
};

typedef struct{
    int board[N*N];
    int player;
    int state;
} game_t;

#endif    // __GAME_H__
