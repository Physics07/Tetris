#include <bits/stdc++.h>
#include "screen.hpp"
using namespace std;

const int WMAX = 10 + 2 + 2; // board width + wall + padding
const int HMAX = 20 + 4 + 1; // board height + mino starting pos(ceiling) + floor
const int CEILING = 4;
const char *TILE[] = {" ", "■", "□", " ", "▦", "■"}; // blank, block(fixed), wall / floor, padding, shadow of the block, block(moving)
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
        {{0,0,0},{1,1,1},{1,0,0}},
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

/// @brief tetr.io combo system
/// row 1 - single, 2 - double, 3 - triple, 4 - quad
/// if more: line count + combo count
const int COMBO[][21] = {
    {0},
    {0,0,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3},
    {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6},
    {2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12},
    {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}
};
const int MAX_GARBAGE_COUNT = 8;

/**
 * @brief class of the tetris board
*/
class TetrisBoard {
public:
    int X_POS = 5, Y_POS = 1;
    int board[HMAX][WMAX];
    mt19937 random_gen;
    uniform_int_distribution<int> randomInt;

    TetrisBoard() {
        random_gen = mt19937(time(NULL));
        randomInt = uniform_int_distribution<int>(0, 9);

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
    void show(bool isTurn = true) {
        int currX = X_POS, currY = Y_POS;
        for(int i=0; i<HMAX; i++) {
            for(int j=0; j<WMAX; j++) {
                if(isTurn || (board[i][j] != 5 && board[i][j] != 4)) draw_string(TILE[board[i][j]], currX, currY);
                else {
                    COLORMAKER.set_color_red();
                    draw_string(TILE[board[i][j]], currX, currY);
                    COLORMAKER.set_color_white();
                }
                currX++;
            }
            currX = X_POS;
            currY++;
        }
    }

    int clear_line() {
        int ret = 0;
        for(int i=0; i<HMAX; i++) {
            bool isFull = true;
            for(int j=2; j<WMAX-2; j++) {
                if(board[i][j] != 1) {
                    isFull = false;
                    break;
                }
            }
            if(isFull) {
                ret++;
                for(int k=i; k>=1; k--) for(int j=2; j<WMAX-2; j++) {
                    if(board[k-1][j] != 5 && board[k-1][j] != 4) board[k][j] = board[k-1][j];
                }
            }
        }
        return ret;
    }

    /**
     * @brief defines whether the board is dead
    */
    bool is_dead() {
        for(int i=0; i<CEILING; i++) for(int j=0; j<WMAX; j++) {
            if(board[i][j] == 1) return true;
        }
        return false;
    }

    /**
     * @brief makes a garbage line in random place
    */
    void generate_garbage(int lineCount) {
        if(lineCount == 0) return;
        int emptySpace = randomInt(random_gen) + 2; // random place in the board
        for(int i=0; i<HMAX-1-lineCount; i++) {
            for(int j=2; j<WMAX-2; j++) {
                if(board[i+lineCount][j] != 5 && board[i+lineCount][j] != 4)
                    board[i][j] = board[i+lineCount][j];
            }
        }
        for(int i=HMAX-1-lineCount; i<HMAX-1; i++) {
            for(int j=2; j<WMAX-2; j++) {
                if(j != emptySpace) {
                    board[i][j] = 1;
                }
                else {
                    board[i][j] = 0;
                }
            }
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
    void make_in_board(int target = 5) {
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
                    if(tetrisBoard->board[yPos+i][xPos+j] != 0 && tetrisBoard->board[yPos+i][xPos+j] != 4) // not blank or shadow
                        return true;
                }
            }
        }
        return false;
    }

    /**
     * @brief generates the shadow in the board
     * @param target1 the TILE number of the block
     * @param target2 the TILE number of blank
     * @param shadow the TILE number of the shadow
    */
    void generate_shadow(int target1 = 5, int target2 = 0, int shadow = 4) {
        int yPos;
        make_in_board(target2);
        for(yPos = pos[1]; yPos < HMAX; yPos++) {
            if(colliding_with_board(state, pos[0], yPos)) break;
        }
        yPos--;
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                if(blockShape[state][i][j] && tetrisBoard->board[i+yPos][j+pos[0]] != 1) 
                    tetrisBoard->board[i+yPos][j+pos[0]] = shadow;
            }
        }
        make_in_board(target1);
    }

    /**
     * @brief move the block to (xPos, yPos) in the board
     * @param target1 the block number
     * @param target2 the blank number
     * @return true if moved, false if not
    */
    bool move_to_position_in_board(int xPos, int yPos, int target1 = 5, int target2 = 0) {
        if(isCreated) { // delete the previos one and the shadow
            make_in_board(target2);
            generate_shadow(target2, target2, target2);
        }
        else isCreated = true;
        if(colliding_with_board(state, xPos, yPos)) {
            make_in_board(target1);
            generate_shadow(target1, target2);
            return false;
        }
        pos[0] = xPos;
        pos[1] = yPos;
        make_in_board(target1);
        generate_shadow(target1, target2);
        return true;
    }
    
    /**
     * @brief move the block
    */
    bool move_block_down(int target1 = 5, int target2 = 0) {
        return move_to_position_in_board(pos[0], pos[1] + 1, target1, target2);
    }
    bool move_block_left(int target1 = 5, int target2 = 0) {
        return move_to_position_in_board(pos[0] - 1, pos[1], target1, target2);
    }
    bool move_block_right(int target1 = 5, int target2 = 0) {
        return move_to_position_in_board(pos[0] + 1, pos[1], target1, target2);
    }
    
    /**
     * @brief rotate the block clockwise
     * @param target1 the block number
     * @param target2 the blank number
    */
    bool rotate_clockwise(int target1 = 5, int target2 = 0) {
        if(blockNum == O_MINO) {
            state++;
            state %= 4;
            return true;
        }
        make_in_board(target2);
        generate_shadow(target2, target2, target2);
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
        generate_shadow(target1, target2);
        return canRotate;
    }

    /**
     * @brief rotate the block counterclockwise
     * @param target1 the block number
     * @param target2 the blank number
    */
    bool rotate_counterclockwise(int target1 = 5, int target2 = 0) {
        if(blockNum == O_MINO) {
            state += 3;
            state %= 4;
            return true;
        }
        make_in_board(target2);
        generate_shadow(target2, target2, target2);
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
        generate_shadow(target1, target2);
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

class Game {
public:
    InputKey rotateCCW, rotateCW, hardDrop, leftMove, rightMove, softDrop;
    TetrisBoard board;
    deque<Block*> blockList, nxtBlockList;
    Block *currBlock;
    int blockCount, gravityMaxCount, gravityCount, comboCount, currLeftTime, fisherTime;
    double gravityDelay, delayedAutoShift;
    clock_t leftTime, rightTime, prvDropTime, currTime;
    bool isTurn;
    mt19937 random_gen;
    deque<int> garbageCount;
    Game *enemyGame;

    Game() {}
    Game(int ccw, int cw, int _harddrop, int _left, int _right, int _softdrop, int xPos = 5, int yPos = 5) {
        // some constants in game
        random_gen = mt19937(time(NULL));
        gravityDelay = 300.0;
        gravityMaxCount = 2;
        delayedAutoShift = 200.0;
        currLeftTime = 300 * 1000;
        fisherTime = 5 * 1000;
        currTime = clock();

        rotateCCW.key = ccw;
        rotateCW.key = cw;
        hardDrop.key = _harddrop;
        softDrop.key = _softdrop;
        leftMove.key = _left;
        rightMove.key = _right;
        gravityCount = 0;
        comboCount = 0;
        enemyGame = nullptr;
        board.X_POS = xPos;
        board.Y_POS = yPos;
        isTurn = false;
        
        for(int i=0; i<7; i++) blockList.push_back(new Block(i, &board));
        for(int i=0; i<7; i++) nxtBlockList.push_back(new Block(i, &board));
        shuffle(blockList.begin(), blockList.end(), random_gen);
        shuffle(nxtBlockList.begin(), nxtBlockList.end(), random_gen);
        blockCount = 0;
        currBlock = blockList[0];
        blockList.pop_front();
        currBlock->make_in_board();
        currBlock->generate_shadow();
        leftTime = rightTime = prvDropTime = clock();
    }

    /**
     * @brief set enemy game to send garbages
    */
    void set_enemy(Game *_enemyGame) {
        enemyGame = _enemyGame;
    }

    /**
     * @brief get the garbage line and push it in queue
    */
    void push_garbage(int count) {
        if(count <= 0) return;
        garbageCount.push_back(count);
    }
    
    /**
     * @brief gets the board's total garbage count
    */
    int get_total_garbage_count() {
        int ret = 0;
        for(auto i: garbageCount) ret += i;
        return ret;
    }

    /**
     * @brief sends attack to enemy's board / clears the garbage of this board
    */
    void send_attack(int lineCount) {
        if(!lineCount) return;
        int garbageLineSend = (lineCount <= 4 ? COMBO[lineCount][20] : lineCount + comboCount);
        if(lineCount <= 4 && comboCount < 20) garbageLineSend = COMBO[lineCount][comboCount];
        while(!garbageCount.empty() && garbageLineSend > 0) {
            garbageCount.front() -= garbageLineSend;
            if(garbageCount.front() <= 0) { // if the garbage is cleared
                garbageLineSend = -garbageCount.front();
                garbageCount.pop_front();
            }
            else { // if the garbage is not cleared
                garbageLineSend = 0;
            }
        }
        if(garbageLineSend > 0 && enemyGame) enemyGame->push_garbage(garbageLineSend);
        comboCount++;
    }
    
    /**
     * @brief generates the garbage in this board
     * @details the max count of garbage generated in one call is MAX_GARBAGE_COUNT
    */
    void generate_garbage() {
        int generatedGarbageCount = 0;
        while(!garbageCount.empty() && generatedGarbageCount < MAX_GARBAGE_COUNT) {
            if(generatedGarbageCount + garbageCount.front() > MAX_GARBAGE_COUNT) {
                board.generate_garbage(MAX_GARBAGE_COUNT - generatedGarbageCount);
                garbageCount.front() -= MAX_GARBAGE_COUNT - generatedGarbageCount;
                generatedGarbageCount = MAX_GARBAGE_COUNT;
                break;
            }
            board.generate_garbage(garbageCount.front());
            generatedGarbageCount += garbageCount.front();
            garbageCount.pop_front();
        }
    }

    void draw_garbage_line() {
        COLORMAKER.set_color_red();
        int totalLine = get_total_garbage_count();
        totalLine = min(totalLine, 25);
        for(int yPos=board.Y_POS+HMAX-1, i=0; i<25; yPos--, i++) {
            if(i < totalLine) draw_string(TILE[1], board.X_POS + WMAX, yPos);
            else draw_string(TILE[0], board.X_POS + WMAX, yPos);
        }
        COLORMAKER.set_color_white();
    }

    void draw_time() {
        int xPos = board.X_POS + 2, yPos = board.Y_POS + HMAX + 2;
        int minute = currLeftTime / 60'000, second = currLeftTime % 60'000;
        int decimal = second % 1000;
        second /= 1000;
        decimal /= 10;
        gotoXY(xPos, yPos);
        printf("%d:%02d.%02d ", minute, second, decimal);
    }

    /**
     * @brief starts the turn of this player.
     * @details manages all the stuff in the premoving process.
    */
    void start_turn() {
        isTurn = true;
        int currLineCount = board.clear_line();
        prvDropTime = clock();
        currTime = clock();

        // sending lines and clearing/generating garbages
        if(currLineCount) {
            send_attack(currLineCount);
        }
        else {
            comboCount = 0;
        }

        gravityCount = 0;
        if(!currBlock) {
            currBlock = blockList[0];
            blockList.pop_front();
            currBlock->make_in_board();
            currBlock->generate_shadow();

            // switch the turn
            isTurn = false;
            if(enemyGame) enemyGame->start_turn();
            
            // fisher
            currLeftTime += fisherTime;
        }
    }

    /**
     * @brief generating next block
    */
    void generate_next_block() {
        int currLineCount = board.clear_line();
        
        // sending lines and clearing/generating garbages
        if(currLineCount) {
            send_attack(currLineCount);
        }
        else {
            comboCount = 0;
            generate_garbage();
        }

        gravityCount = 0;
        blockCount++;
        if(blockCount == 7) {
            blockCount = 0;
            for(int i=0; i<7; i++) blockList.push_back(nxtBlockList[i]);
            nxtBlockList.clear();
            for(int i=0; i<7; i++) nxtBlockList.push_back(new Block(i, &board));
            shuffle(nxtBlockList.begin(), nxtBlockList.end(), random_gen);
            
            // switch the turn
            isTurn = false;
            if(enemyGame) enemyGame->start_turn();
            
            // fisher
            currLeftTime += fisherTime;
        }
        currBlock = blockList[0];
        blockList.pop_front();
        currBlock->make_in_board();
        currBlock->generate_shadow();
    }

    /**
     * @brief generate the next block when the player is premoving
     * @details premoving the blocks is up to 1 bag & premoving doesn't send or get attacks.
    */
    void generate_next_block_when_premoving() {
        gravityCount = 0;
        blockCount++;
        if(blockCount == 7) {
            blockCount = 0;
            for(int i=0; i<7; i++) blockList.push_back(nxtBlockList[i]);
            nxtBlockList.clear();
            for(int i=0; i<7; i++) nxtBlockList.push_back(new Block(i, &board));
            shuffle(nxtBlockList.begin(), nxtBlockList.end(), random_gen);
            currBlock = nullptr;
            return;
        }

        currBlock = blockList[0];
        blockList.pop_front();
        currBlock->make_in_board();
        currBlock->generate_shadow();
    }

    void show_next() {
        int x = board.X_POS + WMAX + 2, y = board.Y_POS + 4;
        if(!isTurn) {
            for(int i=0; i<5; i++, y+=4) for(int j=0; j<4; j++) draw_string("    ", x, y + j);
            return;
        }
        for(int i=0; i<5; i++, y+=4) {
            if(i<(int)blockList.size()) {
                vector<vector<int>> &block = MINO[blockList[i]->blockNum][0];
                for(int j=0; j<(int)block.size(); j++) {
                    for(int k=0; k<4; k++) {
                        if(k<(int)block[0].size()) draw_string(TILE[block[j][k]], x + k, y + j);
                        else draw_string(TILE[0], x + k, y + j);
                    }
                }
            }
            else {
                vector<vector<int>> &block = MINO[nxtBlockList[i+blockCount-6]->blockNum][0];
                for(int j=0; j<(int)block.size(); j++) {
                    for(int k=0; k<4; k++) {
                        if(k<(int)block[0].size()) draw_string(TILE[block[j][k]], x + k, y + j);
                        else draw_string(TILE[0], x + k, y + j);
                    }
                }
            }
        }
    }

    /**
     * @brief loop of game
    */
    bool in_game_loop() {
        board.show(isTurn);
        draw_garbage_line();
        draw_time();
        show_next();

        if(isTurn) {
            // time control
            currLeftTime -= clock_millisecond(clock() - currTime);
            currTime = clock();
            if(currLeftTime < 0) return false;

            // gravity
            if(clock_millisecond(clock() - prvDropTime) >= gravityDelay) {
                prvDropTime = clock();
                if(!currBlock->move_block_down()) gravityCount++;
                if(gravityCount == gravityMaxCount) {
                    currBlock->fix();
                    generate_next_block();
                    return !board.is_dead();
                }
            }
        }

        if(!currBlock) return !board.is_dead();

        if(rotateCCW.is_pressed_first()) {
            currBlock->rotate_counterclockwise();
            gravityCount = 0; // for good gameplay
        }
        if(rotateCW.is_pressed_first()) {
            currBlock->rotate_clockwise();
            gravityCount = 0; // for good gameplay
        }
        if(softDrop.is_pressing()) currBlock->move_block_down();

        // DAS
        if(leftMove.is_pressed_first()) {
            currBlock->move_block_left();
            leftTime = clock();
        }
        else if(leftMove.is_pressed) {
            if(clock_millisecond(clock() - leftTime) >= delayedAutoShift) {
                currBlock->move_block_left();
            }
        }
        if(rightMove.is_pressed_first()) {
            currBlock->move_block_right();
            rightTime = clock();
        }
        else if(rightMove.is_pressed) {
            if(clock_millisecond(clock() - rightTime) >= delayedAutoShift) {
                currBlock->move_block_right();
            }
        }

        if(hardDrop.is_pressed_first()) {
            currBlock->hard_drop();
            if(isTurn) generate_next_block();
            else generate_next_block_when_premoving();
            return !board.is_dead();
        }

        return !board.is_dead();
    }
};