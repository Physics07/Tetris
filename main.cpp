/// including requirements
#include <bits/stdc++.h>
#include <turboc.h>
#include "tetris.hpp"

using namespace std;

InputKey SPACE(VK_SPACE), UP(VK_UP), DOWN(VK_DOWN), LEFT(VK_LEFT), RIGHT(VK_RIGHT), Z(get_key_value('z')), X(get_key_value('x'));

int main() {
    setCursorType(NOCURSOR);
    system("fullscreen.exe");
    system("chcp 65001");
    system("cls");
    COLORMAKER.set_color_white();

    /// for debugings
    Game player1(get_key_value('q'), get_key_value('w'), get_key_value('f'), get_key_value('c'), get_key_value('b'), get_key_value('v'), 30, 5);
    Game player2(get_key_value('l'), VK_OEM_1, VK_RSHIFT, VK_LEFT, VK_RIGHT, VK_DOWN, 100, 5); /// VK_OEM_1: ';' key
    player1.set_enemy(&player2);
    player2.set_enemy(&player1);
    player1.isTurn = true;
    while(player1.in_game_loop() && player2.in_game_loop());
}