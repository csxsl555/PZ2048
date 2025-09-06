#include <algorithm>
#include <random>
#include "include/user_logic.h"
#include "include/utils.h"
#include "include/game_logic.h"
//WARNING: DO NOT DELETE THE LINES BELOW!
//WHEN YOU PUT YOUR CODE TO ACMOJ, YOU WOULD DELETE THE LINE 3-5 AND USE LINE 8-12 INSTEAD 
//#include "game_logic.h"
//#include "game_logic.cpp"
//#include "utils.h"
//#include "utils.cpp"
//#include "user_logic.h"

namespace PZ2048 {

// You can add global variables here.
// If your IDE suggested some identifiers like "static", "inline" or "extern", you'd better learn about them
// and decide whether, how and where to use them.
// Or if you want to use type aliases, structures, classes or something else,
// just go ahead! You are free (and encouraged) to use them to make your code clearer and more integrated.
// Just don't modify the signatures of existing functions.

// You are free to add helper functions.

// Don't try to use extern variables from game_logic.cpp. You're not supposed to communicate through this way.
// After all, the game logic on the online judger won't contain extern variables same as yours.

using std::find;

static int row_num = 0, col_num = 0;
static std::vector<std::vector<int>> board;

void ClientPrepare(int r, int c) {
    row_num = r;
    col_num = c;
    board.assign(row_num, std::vector<int>(col_num, 0));
}

void ReadBoard() {
    for (int i = 0; i < row_num; ++i) {
        for (int j = 0; j < col_num; ++j) {
            std::cin >> board[i][j];
        }
    }
}

char Decide() {
    // 0=w, 1=s, 2=a, 3=d
    // Please find a strategy to return a char for the direction you want to move
    // Here's an example for random move for four directions with equal probability
    // Notice: the program could not run in reality, please find a reasonable algorithm on your own :)
    srand(time(nullptr));
    static const char dir_map[4] = {'w', 's', 'a', 'd'};
    return dir_map[rand()%4];
}

}

