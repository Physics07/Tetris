/// including requirements
#include <bits/stdc++.h>
#include <turboc.h>
#include "tetris.hpp"

using namespace std;

InputKey SPACE(VK_SPACE), UP(VK_UP), DOWN(VK_DOWN), LEFT(VK_LEFT), RIGHT(VK_RIGHT), Z(get_key_value('z')), X(get_key_value('x'));

int main() {
    setCursorType(NOCURSOR);

    /// for debugings
    Game game(get_key_value('z'), get_key_value('x'), VK_SPACE, VK_LEFT, VK_RIGHT, VK_DOWN);
    while(game.in_game_loop());
}