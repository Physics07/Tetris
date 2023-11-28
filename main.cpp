/// including requirements
#include <bits/stdc++.h>
#include <turboc.h>
#include "tetris.hpp"

using namespace std;

InputKey SPACE(VK_SPACE), UP(VK_UP), DOWN(VK_DOWN), LEFT(VK_LEFT), RIGHT(VK_RIGHT), Z(get_key_value('z')), X(get_key_value('x'));

int main() {
    setCursorType(NOCURSOR);

    /// for debugings
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 6);
    TetrisBoard board;
    Block *block = new Block(dist(mt), &board);
    block->make_in_board();
    while(true) {
        board.show();
        if(Z.is_pressing()) block->rotate_counterclockwise();
        if(X.is_pressing()) block->rotate_clockwise();
        if(SPACE.is_pressing()) {
            block->hard_drop();
            block = new Block(dist(mt), &board);
            block->make_in_board();
        }
        if(DOWN.is_pressing()) block->move_block_down();
        if(LEFT.is_pressing()) block->move_block_left();
        if(RIGHT.is_pressing()) block->move_block_right();
    }
}