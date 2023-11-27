/// including requirements
#include <bits/stdc++.h>
#include <turboc.h>
#include "tetris.hpp"

using namespace std;

int main() {
    setCursorType(NOCURSOR);

    /// for debugings
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 6);
    TetrisBoard board;
    Block *block = new Block(dist(mt), &board);
    InputKey userInput;
    block->make_in_board();
    while(true) {
        board.show();
        userInput.input();
        if(userInput == 'z') block->rotate_counterclockwise();
        if(userInput == 'x') block->rotate_clockwise();
        if(userInput == ' ') {
            block->hard_drop();
            block = new Block(dist(mt), &board);
            block->make_in_board();
        }
        if(userInput == DOWN) block->move_block_down();
        if(userInput == LEFT) block->move_block_left();
        if(userInput == RIGHT) block->move_block_right();
    }
}