#ifndef __GAME_H__
#define __GAME_H__

#define N 3
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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
