#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

#define KEY_ARROW 224
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

/**
 * @brief the struct for key input and finding
*/
struct InputKey {
    int key;
    bool isSpecial;
    InputKey(int _key, bool _isSpecial): key(_key), isSpecial(_isSpecial) {}
    InputKey() {
        key = 0;
        isSpecial = 0;
    }

    // set key with user input
    void input() {
        key = getch();
        if(key == KEY_ARROW) {
            isSpecial = true;
            key = getch();
        }
        else isSpecial = false;
    }

    // compare two keys
    bool operator==(const InputKey &key_comp) const {
        if(key == key_comp.key && isSpecial == key_comp.isSpecial) return true;
        return false;
    }
    bool operator==(const char &key_char) const {
        if(isSpecial) return false;
        return key == key_char;
    }
};
// constant keys for global using
const InputKey UP = InputKey(KEY_UP, true);
const InputKey DOWN = InputKey(KEY_DOWN, true);
const InputKey RIGHT = InputKey(KEY_RIGHT, true);
const InputKey LEFT = InputKey(KEY_LEFT, true);

/**
 * @brief set the command prompt to full screen mode
 * @bug this doesn't work in some 64-bit computers
*/
void console_full_screen() {
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_WINDOWED_MODE, 0);
}