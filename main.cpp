/// including requirements
#include <bits/stdc++.h>
#include <turboc.h>
#include "tetris.hpp"

using namespace std;

InputKey SPACE(VK_SPACE), UP(VK_UP), DOWN(VK_DOWN), LEFT(VK_LEFT), RIGHT(VK_RIGHT), ENTER(VK_RETURN), ESC(VK_ESCAPE);

int main() {
    setCursorType(NOCURSOR);
    system("fullscreen.exe");
    system("chcp 65001");
    system("cls");
    COLORMAKER.set_color_white();
    ifstream title_in("./img/title.txt", ios::in), main_in("./img/small_title.txt", ios::in);
    ifstream player1_in("./img/player1.txt", ios::in), player2_in("./img/player2.txt", ios::in);
    ifstream player1turn_in("./img/player1turn.txt", ios::in), player2turn_in("./img/player2turn.txt", ios::in);
    Screen titleScreen(0, {Text("press ENTER to continue", 15, 25)}, {Text(title_in, 15, 10)}, &DOWN, &UP, &ENTER, &ESC);
    Screen mainScreen(1, {Text("Play", 15, 25), Text("Quit", 15, 30)}, {Text(main_in, 15, 10)}, &DOWN, &UP, &ENTER, &ESC);
    Screen resultScreen(2, {Text("press ENTER to continue", 15, 25)}, {Text(player1_in, 15, 10), Text(player2_in, 15, 10)}, &DOWN, &UP, &ENTER, &ESC);
    Screen *currScreen = &titleScreen;
    Text turnOfPlayer[] = {Text(player1turn_in, 70, 20), Text(player2turn_in, 70, 20)};
    while(true) {
        while(true) {
            int input = currScreen->show();
            if(currScreen->ID == 0) {
                if(input == -2) exit(0);
                if(input == 0) transfer_screen_by_screen(&currScreen, &mainScreen);
            }
            else if(currScreen->ID == 1) {
                if(input == -2) transfer_screen_by_screen(&currScreen, &titleScreen);
                if(input == 0) break;
                if(input == 1) exit(0);
            }
        }
        system("cls");
        
        int winner = 0;
        Game player1(get_key_value('q'), get_key_value('w'), get_key_value('f'), get_key_value('c'), get_key_value('b'), get_key_value('v'), 30, 5);
        Game player2(get_key_value('l'), VK_OEM_1, VK_RSHIFT, VK_LEFT, VK_RIGHT, VK_DOWN, 100, 5); /// VK_OEM_1: ';' key
        player1.set_enemy(&player2);
        player2.set_enemy(&player1);
        player1.start_turn();
        while(true) {
            if(player1.isTurn) {
                turnOfPlayer[0].render();
            }
            if(player2.isTurn) {
                turnOfPlayer[1].render();
            }
            if(!player1.in_game_loop()) {
                winner = 1;
                break;
            }
            if(!player2.in_game_loop()) {
                winner = 0;
                break;
            }
        }
        system("cls");
        while(true) {
            int input = resultScreen.show(true, winner);
            if(input == -2 || input == 0) break;
        }
        system("cls");
    }
}