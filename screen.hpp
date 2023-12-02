#include <bits/stdc++.h>
#include "console.hpp"

using namespace std;

class Text {
public:
    string content[20];
    int pos[2], line;
    Text() {
        pos[0] = pos[1] = 0;
    }
    Text(string _s, int xPos = 0, int yPos = 0) {
        content[0] = _s + "  ";
        line = 1;
        pos[0] = xPos;
        pos[1] = yPos;
    }
    Text(ifstream &fp, int xPos = 0, int yPos = 0) {
        for(line=0; getline(fp, content[line]); line++);
        pos[0] = xPos;
        pos[1] = yPos;
    }

    void render() {
        for(int i=0; i<line; i++) {
            draw_string(content[i], pos[0], pos[1] + i);
        }
    }

    void select() {
        content[0][content[0].size() - 1] = '<';
    }

    void deselect() {
        content[0][content[0].size() - 1] = ' ';
    }
};

class Screen {
public:
    vector<Text> button, text;
    int cursor, ID;
    InputKey *down, *up, *enter, *esc;
    Screen() {}
    Screen(int _id, vector<Text> _button, vector<Text> _text, InputKey *_down, InputKey *_up, InputKey *_enter, InputKey *_esc) {
        for(Text i: _button) button.push_back(i);
        for(Text i: _text) text.push_back(i);
        cursor = 0;
        button[0].select();

        ID = _id;
        
        down = _down;
        up = _up;
        enter = _enter;
        esc = _esc;
    }

    int show(bool renderByIdx = false, int idx = 0) {
        for(Text &i: button) i.render();
        if(!renderByIdx) for(Text &i: text) i.render();
        else text[idx].render();
        
        if(down->is_pressed_first()) {
            button[cursor].deselect();
            cursor++;
            cursor %= button.size();
            button[cursor].select();
        }

        if(up->is_pressed_first()) {
            button[cursor].deselect();
            cursor += button.size() - 1;
            cursor %= button.size();
            button[cursor].select();
        }

        if(esc->is_pressed_first()) return -2;

        if(enter->is_pressed_first()) {
            return cursor;
        }

        return -1;
    }
};

void transfer_screen_by_screen(Screen **currScreen, Screen *nextScreen) {
    system("cls");
    *currScreen = nextScreen;
}