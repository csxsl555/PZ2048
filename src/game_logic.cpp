#include "include/game_logic.h"
// WARNING: DO NOT DELETE THE THREE LINES BELOW!
// WHEN YOU PUT YOUR CODE TO ACMOJ, YOU WOULD DELETE THE LINE 1 AND USE LINE 4-6 INSTEAD
// #include "game_logic.h"
// #include "utils.h"
// #include "utils.cpp"

#include <iomanip>
#include <iostream>
#include <vector>

namespace PZ2048 {

    // You can add global variables here.
    // If your IDE suggested some identifiers like "static", "inline" or "extern", you'd better learn about them
    // and decide whether, how and where to use them.
    // Or if you want to use type aliases, structures, classes or something else,
    // just go ahead! You are free (and encouraged) to use them to make your code clearer and more integrated.
    // Or... can you?
    // Just don't modify the signatures of existing functions.

    // Don't try to use extern variables from user_logic.cpp. You're not supposed to communicate through this way.
    // After all, the game logic on the online judger won't contain extern variables same as yours.
    // You are free to add helper functions.

    class Game2048 {
    public:
        Game2048() = default;

        inline void Start(int row_num, int col_num, int target, uint game_seed) {
            rows = row_num, cols = col_num;
            _target = target;
            seed = game_seed;
            score = 0;
            steps = 0;
            can_undo = false;
            srand(seed);
            board.clear();
            board.resize(rows, std::vector<int>(cols, 0));
            prev_state = {{}, 0, 0};
            TryGenerateTile();
        }

        inline std::pair<int, int> EndStatus() { return {steps, score}; }

        inline int GetRows() { return rows; }

        inline int GetCols() { return cols; }

        inline bool Moveup() {
            PreviousState tmp = {board, score, steps};
            bool changed = false;
            for (int j = 0; j < cols; j++) {
                int last_merge_row = -1; // To track the last merged row
                for (int i = 1; i < rows; i++) {
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
                        score += board[k - 1][j];
                        board[k][j] = 0;
                        last_merge_row = k - 1;
                        changed = true;
                    }
                }
            }
            if (changed) {
                prev_state = tmp;
                can_undo = true;
                TryGenerateTile();
                steps++;
            }
            return changed;
        }

        inline bool Movedown() {
            PreviousState tmp = {board, score, steps};
            bool changed = false;
            for (int j = 0; j < cols; j++) {
                int last_merge_row = rows; // To track the last merged row
                for (int i = rows - 2; i >= 0; i--) {
                    if (board[i][j] == 0)
                        continue;
                    int k = i;
                    while (k < rows - 1 && board[k + 1][j] == 0) {
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        k++;
                        changed = true;
                    }
                    if (k < rows - 1 && board[k + 1][j] == board[k][j] && last_merge_row != k + 1) {
                        board[k + 1][j] *= 2;
                        score += board[k + 1][j];
                        board[k][j] = 0;
                        last_merge_row = k + 1;
                        changed = true;
                    }
                }
            }
            if (changed) {
                prev_state = tmp;
                can_undo = true;
                TryGenerateTile();
                steps++;
            }
            return changed;
        }

        inline bool Moveleft() {
            PreviousState tmp = {board, score, steps};
            bool changed = false;
            for (int i = 0; i < rows; i++) {
                int last_merge_col = -1; // To track the last merged column
                for (int j = 1; j < cols; j++) {
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
                        score += board[i][k - 1];
                        board[i][k] = 0;
                        last_merge_col = k - 1;
                        changed = true;
                    }
                }
            }
            if (changed) {
                prev_state = tmp;
                can_undo = true;
                TryGenerateTile();
                steps++;
            }
            return changed;
        }

        inline bool Moveright() {
            PreviousState tmp = {board, score, steps};
            bool changed = false;
            for (int i = 0; i < rows; i++) {
                int last_merge_col = cols; // To track the last merged column
                for (int j = cols - 2; j >= 0; j--) {
                    if (board[i][j] == 0)
                        continue;
                    int k = j;
                    while (k < cols - 1 && board[i][k + 1] == 0) {
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        k++;
                        changed = true;
                    }
                    if (k < cols - 1 && board[i][k + 1] == board[i][k] && last_merge_col != k + 1) {
                        board[i][k + 1] *= 2;
                        score += board[i][k + 1];
                        board[i][k] = 0;
                        last_merge_col = k + 1;
                        changed = true;
                    }
                }
            }
            if (changed) {
                prev_state = tmp;
                can_undo = true;
                TryGenerateTile();
                steps++;
            }
            return changed;
        }

        inline bool TryRun(char dir) {
            if (dir != 'w' && dir != 's' && dir != 'a' && dir != 'd')
                return false;
            // implement the logic of moving tiles here
            // update score and steps accordingly
            switch (dir) {
            case 'w':
                // move up logic
                if (!Moveup())
                    return false;
                break;
            case 's':
                // move down logic
                if (!Movedown())
                    return false;
                break;
            case 'a':
                // move left logic
                if (!Moveleft())
                    return false;
                break;
            case 'd':
                // move right logic
                if (!Moveright())
                    return false;
                break;
            }
            can_undo = true;
            return true; // return true if the move changes the board, otherwise false
        }

        inline bool Undo() {
            // implement the logic of undoing the last move here
            // update score and steps accordingly
            if (!can_undo)
                return false; // return true if undo is successful, otherwise false
            board = prev_state._board;
            score = prev_state.score;
            steps = prev_state.steps;
            can_undo = false;
            prev_state = {{}, 0, 0};
            return true;
        }

        inline void SetTile(int row_index, int col_index, int value) {
            board[row_index][col_index] = value;
        }

        inline int GetTile(int row_index, int col_index) {
            return board[row_index][col_index];
        }

        inline int Score() { return score; }

        inline int Steps() { return steps; }

        inline int GetTarget() { return _target; }

        inline bool HasReachedTarget() {
            int maxx = 0;
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    maxx = std::max(maxx, board[i][j]);
            return maxx >= _target;
        }

        inline bool Stuck() {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (board[i][j] == 0)
                        return false;
                    if (i > 0 && board[i][j] == board[i - 1][j])
                        return false;
                    if (i < rows - 1 && board[i][j] == board[i + 1][j])
                        return false;
                    if (j > 0 && board[i][j] == board[i][j - 1])
                        return false;
                    if (j < cols - 1 && board[i][j] == board[i][j + 1])
                        return false;
                }
            }
            return true;
        }

        inline void PrintBoard() {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    std::cout << std::left << std::setw(4) << board[i][j];
                }
                std::cout << std::endl;
            }
        }

    private:
        int rows, cols;
        int _target;
        uint seed;
        int score;
        int steps;
        std::vector<std::vector<int>> board;
        struct PreviousState {
            std::vector<std::vector<int>> _board;
            int score;
            int steps;
        } prev_state;
        bool can_undo;
    } game;

    void Start(int row_num, int col_num, int target, uint game_seed) {
        game.Start(row_num, col_num, target, game_seed);
    }

    std::pair<int, int> EndGame() { return game.EndStatus(); }

    int GetRows() { return game.GetRows(); }

    int GetCols() { return game.GetCols(); }

    bool TryRun(char dir) { return game.TryRun(dir); }

    bool Undo() { return game.Undo(); }

    void SetTile(int row_index, int col_index, int value) {
        /** implement here. **/
        /** Remember to check the validity of the input. **/
        /** If the input is invalid, do nothing. **/
        /** The given row index and column index is 0-based. **/
        if (row_index < 0 || row_index >= GetRows() || col_index < 0 || col_index >= GetCols() || value < 0)
            return;
        game.SetTile(row_index, col_index, value);
    }

    int GetTile(int row_index, int col_index) { return game.GetTile(row_index, col_index); }

    int Score() { return game.Score(); }

    int Steps() { return game.Steps(); }

    int GetTarget() { return game.GetTarget(); }

    bool HasReachedTarget() { return game.HasReachedTarget(); }

    bool Stuck() { return game.Stuck(); }

    void PrintBoard() { game.PrintBoard(); }

} // namespace PZ2048
