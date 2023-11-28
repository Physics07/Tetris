#include <bits/stdc++.h>
#include <turboc.h>
#include <windows.h>
#include <conio.h>

#define VK_CHAR_A 0x41

using namespace std;

/**
 * @brief the struct for key input and finding
*/
struct InputKey {
    int key;
    InputKey(int _key): key(_key) {}
    InputKey() {
        key = 0;
    }

    /**
     * @brief returns whether the key is pressed
    */
    bool is_pressing() {
        if(GetAsyncKeyState(key)) return true;
        else return false;
    }
};

int get_key_value(char c) {
    return VK_CHAR_A + c - 'a';
}

/**
 * @brief set the command prompt to full screen mode
 * @bug this doesn't work in some 64-bit computers
*/
void console_full_screen() {
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_WINDOWED_MODE, 0);
}

/**
 * @brief draws a character or a string on the (X, Y) position
*/
void draw_char(char c, int X, int Y) {
    gotoXY(X, Y);
    printf("%c", c);
}
void draw_string(string s, int X, int Y) {
    gotoXY(X, Y);
    printf("%s", s.c_str());
}