#include <bits/stdc++.h>
#include "console.hpp"
using namespace std;

const int WMAX = 10 + 2 + 2; // board width + wall + padding
const int HMAX = 20 + 4 + 1; // board height + mino starting pos(ceiling) + floor
const int CEILING = 4;
const char *TILE[] = {" ", "■", "□", " "}; // blank, block, wall / floor, padding
vector<vector<int>> MINO[][4] = {
    { // I mino
        {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
        {{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,0,1,0}},
        {{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
        {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}}
    },
    { // J mino
        {{1,0,0},{1,1,1},{0,0,0}},
        {{0,1,1},{0,1,0},{0,1,0}},
        {{0,0,0},{1,1,1},{0,0,1}},
        {{0,1,0},{0,1,0},{1,1,0}}
    },
    { // L mino
        {{0,0,1},{1,1,1},{0,0,0}},
        {{0,1,0},{0,1,0},{0,1,1}},
        {{0,0,0},{1,1,1},{0,0,1}},
        {{1,1,0},{0,1,0},{0,1,0}}
    },
    { // S mino
        {{0,1,1},{1,1,0},{0,0,0}},
        {{0,1,0},{0,1,1},{0,0,1}},
        {{0,0,0},{0,1,1},{1,1,0}},
        {{1,0,0},{1,1,0},{0,1,0}}
    },
    { // Z mino
        {{1,1,0},{0,1,1},{0,0,0}},
        {{0,0,1},{0,1,1},{0,1,0}},
        {{0,0,0},{1,1,0},{0,1,1}},
        {{0,1,0},{1,1,0},{1,0,0}}
    },
    { // T mino
        {{0,1,0},{1,1,1},{0,0,0}},
        {{0,1,0},{0,1,1},{0,1,0}},
        {{0,0,0},{1,1,1},{0,1,0}},
        {{0,1,0},{1,1,0},{0,1,0}}
    },
    { // O mino
        {{0,1,1,0},{0,1,1,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0}},
        {{0,1,1,0},{0,1,1,0},{0,0,0,0}}
    }
};

// state, clockwise/counterclockwise, wallkick index
/// @brief wallkick data for I mino
const int WALLKICK_I[4][2][5][2] = {
    { // state 0 wallkick
        {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}},
        {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}}
    },
    { // state 1 wallkick
        {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}},
        {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}}
    },
    { // state 2 wallkick
        {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}},
        {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}}
    },
    { // state 3 wallkick
        {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}},
        {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}}
    }
};
/// @brief wallkick data for JLSZT minos
const int WALLKICK_ELSE[4][2][5][2] = {
    { // state 0 wallkick
        {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},
        {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}
    },
    { // state 1 wallkick
        {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},
        {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}
    },
    { // state 2 wallkick
        {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},
        {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}
    },
    { // state 3 wallkick
        {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, 2}},
        {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}
    }
};
const int I_MINO = 0;
const int J_MINO = 1;
const int L_MINO = 2;
const int S_MINO = 3;
const int Z_MINO = 4;
const int T_MINO = 5;
const int O_MINO = 6;
const int STARTING_POS[2] = {5, 0};

/**
 * @brief class of the tetris board
*/
class TetrisBoard {
public:
    int X_POS = 5, Y_POS = 1;
    int board[HMAX][WMAX];

    TetrisBoard() {
        for(int i=0; i<HMAX; i++) {
            board[i][0] = board[i][WMAX-1] = 3;
            if(i == HMAX-1) {
                for(int j=1; j<WMAX-1; j++) board[i][j]=2;
                continue;
            }
            if(i < CEILING) board[i][1] = board[i][WMAX-2] = 3;
            else board[i][1] = board[i][WMAX-2] = 2;
            for(int j=2; j<WMAX-2; j++) board[i][j] = 0;
        }
    }

    /**
     * @brief drawing the board in CMD
    */
    void show() {
        int currX = X_POS, currY = Y_POS;
        for(int i=0; i<HMAX; i++) {
            for(int j=0; j<WMAX; j++) {
                draw_string(TILE[board[i][j]], currX, currY);
                currX++;
            }
            currX = X_POS;
            currY++;
        }
    }
};

/**
 * @brief class of the blocks(tetrominos) in the game
*/
class Block {
public:
    int blockNum, state, width, height, pos[2]; // pos: (x, y) coordinate of upper left corner in the block
    bool isCreated;
    TetrisBoard *tetrisBoard;
    vector<vector<int>> blockShape[4];

    Block() {
        assert(0); // just don't call this function
    }
    Block(int _blockNum, TetrisBoard *_tetrisBoard = nullptr, int xPos = STARTING_POS[0], int yPos = STARTING_POS[1]) {
        blockNum = _blockNum;
        tetrisBoard = _tetrisBoard;
        state = 0;
        isCreated = false;
        for(int i=0; i<4; i++) blockShape[i] = MINO[blockNum][i];
        width = blockShape[0][0].size();
        height = blockShape[0].size();
        pos[0] = xPos;
        pos[1] = yPos;
    }
    ~Block() {
        tetrisBoard = nullptr;
    }

    /**
     * @brief fills the tetrisBoard with target number in the block's pos
    */
    void make_in_board(int target = 1) {
        isCreated = true;
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                if(blockShape[state][i][j]) 
                    tetrisBoard->board[i+pos[1]][j+pos[0]] = target;
            }
        }
    }

    /**
     * @brief decides whether the block is colliding with the board or out of the board
     * @return true if colliding, false if not.
    */
    bool colliding_with_board(int newState, int xPos, int yPos) {
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                if(blockShape[newState][i][j]) {
                    if(yPos + i < 0 || xPos + j < 0 || yPos + i >= HMAX || xPos + j >= WMAX) 
                        return true;
                    if(tetrisBoard->board[yPos+i][xPos+j])
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * @brief move the block to (xPos, yPos) in the board
     * @param target1 the block number
     * @param target2 the blank number
     * @return true if moved, false if not
    */
    bool move_to_position_in_board(int xPos, int yPos, int target1 = 1, int target2 = 0) {
        if(isCreated) make_in_board(target2); // delete the previous one
        else isCreated = true;
        if(colliding_with_board(state, xPos, yPos)) {
            make_in_board(target1);
            return false;
        }
        pos[0] = xPos;
        pos[1] = yPos;
        make_in_board(target1);
        return true;
    }
    
    /**
     * @brief move the block
    */
    bool move_block_down(int target1 = 1, int target2 = 0) {
        return move_to_position_in_board(pos[0], pos[1] + 1, target1, target2);
    }
    bool move_block_left(int target1 = 1, int target2 = 0) {
        return move_to_position_in_board(pos[0] - 1, pos[1], target1, target2);
    }
    bool move_block_right(int target1 = 1, int target2 = 0) {
        return move_to_position_in_board(pos[0] + 1, pos[1], target1, target2);
    }
    
    /**
     * @brief rotate the block clockwise
     * @param target1 the block number
     * @param target2 the blank number
    */
    bool rotate_clockwise(int target1 = 1, int target2 = 0) {
        if(blockNum == O_MINO) {
            state++;
            state %= 4;
            return true;
        }
        make_in_board(target2);
        const int (&rotate_wallkick)[5][2] = (blockNum == I_MINO ? WALLKICK_I : WALLKICK_ELSE)[state][0];
        int nxtState = (state+1)%4, wallkickIdx;
        bool canRotate = false;
        for(wallkickIdx=0; wallkickIdx<5; wallkickIdx++) {
            if(colliding_with_board(nxtState, pos[0] + rotate_wallkick[wallkickIdx][0], pos[1] + rotate_wallkick[wallkickIdx][1])) {
                continue;
            }
            else {
                canRotate = true;
                break;
            }
        }
        if(canRotate) {
            pos[0] += rotate_wallkick[wallkickIdx][0];
            pos[1] += rotate_wallkick[wallkickIdx][1];
            state = nxtState;
        }
        make_in_board(target1);
        return canRotate;
    }

    /**
     * @brief rotate the block counterclockwise
     * @param target1 the block number
     * @param target2 the blank number
    */
    bool rotate_counterclockwise(int target1 = 1, int target2 = 0) {
        if(blockNum == O_MINO) {
            state += 3;
            state %= 4;
            return true;
        }
        make_in_board(target2);
        const int (&rotate_wallkick)[5][2] = (blockNum == I_MINO ? WALLKICK_I : WALLKICK_ELSE)[state][1];
        int nxtState = (state+3)%4, wallkickIdx;
        bool canRotate = false;
        for(wallkickIdx=0; wallkickIdx<5; wallkickIdx++) {
            if(colliding_with_board(nxtState, pos[0] + rotate_wallkick[wallkickIdx][0], pos[1] + rotate_wallkick[wallkickIdx][1])) {
                continue;
            }
            else {
                canRotate = true;
                break;
            }
        }
        if(canRotate) {
            pos[0] += rotate_wallkick[wallkickIdx][0];
            pos[1] += rotate_wallkick[wallkickIdx][1];
            state = nxtState;
        }
        make_in_board(target1);
        return canRotate;
    }

    /**
     * @brief fix on the board
    */
    void fix(int target = 1) {
        make_in_board(target);
        delete this;
    }
    
    /**
     * @brief hard drop
    */
    void hard_drop(int target1 = 1, int target2 = 0) {
        while(move_block_down(target1, target2));
        fix(target1);
    }
};