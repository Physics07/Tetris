/// including requirements
#include <bits/stdc++.h>
#include <turboc.h>
#include "console.hpp"

using namespace std;

int main() {
    InputKey c;
    for(int i = 0; i < 10; i++) {
        c.input();
        if(c == UP) cout << "up ";
        if(c == DOWN) cout << "down ";
        if(c == RIGHT) cout << "right ";
        if(c == LEFT) cout << "left ";
        if(c == 'j') cout << "j ";
    }
}