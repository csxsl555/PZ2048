#include "include/user_logic.h"
#include "include/game_logic.h"
#include "include/utils.h"
#include <algorithm>
#include <assert.h>
#include <random>
// WARNING: DO NOT DELETE THE LINES BELOW!
// WHEN YOU PUT YOUR CODE TO ACMOJ, YOU WOULD DELETE THE LINE 3-5 AND USE LINE 8-12 INSTEAD
/*
#include "game_logic.h"
#include "game_logic.cpp"
#include "utils.h"
#include "utils.cpp"
#include "user_logic.h"
*/

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

    class Decider {
    public:
        Decider() = default;

        char Decide() {
            // 0=w, 1=s, 2=a, 3=d
            // Please find a strategy to return a char for the direction you want to move
            std::vector<char> directions = {'w', 's', 'a', 'd'};
            std::vector<int> scores(4, 0);
            std::vector<std::vector<int>> boards = board;
            for (int i = 0; i < 4; ++i) {
                board = boards;
                scores[i] = MoreStepsScore(3, i, 0);
                board = boards; // Restore the board
            }
            int max_score = *std::max_element(scores.begin(), scores.end());
            assert(max_score >= 0); // There should be at least one valid move
            for (int i = 0; i < 4; ++i) {
                if (scores[i] == max_score) {
                    return directions[i];
                }
            }
            return 'w'; // Default move if all are invalid
        }

    private:
        const int ValuedFunction[4][4] = 
        {
            {32, 16, 8, 4},
            {16, 8, 4, 2},
            {8, 4, 2, 1},
            {4, 2, 1, 1}
        };

        inline int GetNewScore()
        {
            int score = 0;
            for (int i = 0; i < row_num; ++i) {
                for (int j = 0; j < col_num; ++j) {
                    score += board[i][j] * ValuedFunction[i][j];
                    if (board[i][j] == 0)
                        score += 24; // Penalize empty tiles
                }
            }
            return score;
        }

        inline std::pair<bool, int> Moveup() {
            bool changed = false;
            int score_gained = 0;
            for (int j = 0; j < col_num; j++) {
                int last_merge_row = -1; // To track the last merged row
                for (int i = 1; i < row_num; i++) {
                    if (board[i][j] == 0)
                        continue;
                    int k = i;
                    while (k > 0 && board[k - 1][j] == 0) {
                        board[k - 1][j] = board[k][j];
                        board[k][j] = 0;
                        k--;
                        changed = true;
                    }
                    if (k > 0 && board[k - 1][j] == board[k][j] && last_merge_row != k - 1) {
                        board[k - 1][j] *= 2;
                        score_gained += board[k - 1][j];
                        board[k][j] = 0;
                        last_merge_row = k - 1;
                        changed = true;
                    }
                }
            }
            return {changed, score_gained};
        }

        inline std::pair<bool, int> Movedown() {
            bool changed = false;
            int score_gained = 0;
            for (int j = 0; j < col_num; j++) {
                int last_merge_row = row_num; // To track the last merged row
                for (int i = row_num - 2; i >= 0; i--) {
                    if (board[i][j] == 0)
                        continue;
                    int k = i;
                    while (k < row_num - 1 && board[k + 1][j] == 0) {
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        k++;
                        changed = true;
                    }
                    if (k < row_num - 1 && board[k + 1][j] == board[k][j] && last_merge_row != k + 1) {
                        board[k + 1][j] *= 2;
                        score_gained += board[k + 1][j];
                        board[k][j] = 0;
                        last_merge_row = k + 1;
                        changed = true;
                    }
                }
            }
            return {changed, score_gained};
        }

        inline std::pair<bool, int> Moveleft() {
            bool changed = false;
            int score_gained = 0;
            for (int i = 0; i < row_num; i++) {
                int last_merge_col = -1; // To track the last merged column
                for (int j = 1; j < col_num; j++) {
                    if (board[i][j] == 0)
                        continue;
                    int k = j;
                    while (k > 0 && board[i][k - 1] == 0) {
                        board[i][k - 1] = board[i][k];
                        board[i][k] = 0;
                        k--;
                        changed = true;
                    }
                    if (k > 0 && board[i][k - 1] == board[i][k] && last_merge_col != k - 1) {
                        board[i][k - 1] *= 2;
                        score_gained += board[i][k - 1];
                        board[i][k] = 0;
                        last_merge_col = k - 1;
                        changed = true;
                    }
                }
            }
            return {changed, score_gained};
        }

        inline std::pair<bool, int> Moveright() {
            bool changed = false;
            int score_gained = 0;
            for (int i = 0; i < row_num; i++) {
                int last_merge_col = col_num; // To track the last merged column
                for (int j = col_num - 2; j >= 0; j--) {
                    if (board[i][j] == 0)
                        continue;
                    int k = j;
                    while (k < col_num - 1 && board[i][k + 1] == 0) {
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        k++;
                        changed = true;
                    }
                    if (k < col_num - 1 && board[i][k + 1] == board[i][k] && last_merge_col != k + 1) {
                        board[i][k + 1] *= 2;
                        score_gained += board[i][k + 1];
                        board[i][k] = 0;
                        last_merge_col = k + 1;
                        changed = true;
                    }
                }
            }
            return {changed, score_gained};
        }

        inline int MoreStepsScore(int rest, int dir, int score_so_far) {
            if (!rest)
                return score_so_far;
            int original_score = score_so_far;
            bool changed = false;
            int score_now = 0;
            switch (dir) {
            case 0: {
                auto result = Moveup();
                changed = result.first;
                score_now = result.second;
                break;
            }
            case 1: {
                auto result = Movedown();
                changed = result.first;
                score_now = result.second;
                break;
            }
            case 2: {
                auto result = Moveleft();
                changed = result.first;
                score_now = result.second;
                break;
            }
            case 3: {
                auto result = Moveright();
                changed = result.first;
                score_now = result.second;
                break;
            }
            }
            if (!changed)
                return -1; // Invalid move
            score_now += GetNewScore();
            original_score += score_now;
            std::vector<std::vector<int>> boards = board;
            for (int i = 0; i < 4; ++i) {
                board = boards;
                int temp_score = MoreStepsScore(rest - 1, i, score_so_far + score_now);
                if (temp_score > original_score) {
                    original_score = temp_score;
                }
                board = boards; // Restore the board
            }
            return original_score;
        }
    } decider;

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
        return decider.Decide();
    }

} // namespace PZ2048