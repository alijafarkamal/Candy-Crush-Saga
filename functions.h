#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

const int row = 5;
const int col = 5;
const int cell_size = 70;
const int total_textures = 6;
const int special_textures = 2;
Vector2i suggested_move(-1, -1);

int candies[row][col] = { {0 } };
bool assigned_toBurst[row][col] = { {false} };
Font font; 

int score1 = 0;
int prevScore1 = 0;
int moves = 20;

Texture textures[total_textures]; 
Texture specialTextures[special_textures];

RenderWindow window(VideoMode(1398, 896), "Candy Crush Saga", Style::Close);

Sprite sprite;
Clock clock1;
int timer = 30;

int mid_x = 400;
int mid_y = 150;

int breakTimeout = 30;
bool breakTimeoutActive = false;

Text scoretext;
Text movestext;
Text timertext;

bool SpecialCandy = false;
bool SpecialCandy_1 = false;
Vector2i selectedtile(-1, -1);

void game_loader(int scores, int move) {
    ofstream game_loader("load.txt");
    if (game_loader.is_open()) {
        game_loader << "score " << scores << endl << "moves " << move;
        game_loader.close();
    }
    else {
        cout << "Error faced during file opening" << endl;
    }
}


void game(int& scores, int& move) {
    ifstream game_saver("load.txt");

    if (game_saver.is_open()) {
        char data[30];

        while (game_saver >> data) {
            if (strcmp(data, "score") == 0) {
                game_saver >> scores;
                prevScore1 = scores;
            }
            else if (strcmp(data, "moves") == 0) {
                game_saver >> move;
                break;
            }

        }
        if (move <= 0) {
            move = 20;
            scores = 0;
            prevScore1 = scores;
        }
        else {
            cout << "ABCD" << endl;
            for (int i = 0; i < row; i++) {
                for (int j = 0; j < row; j++) {
                    game_saver >> candies[i][j];
                    cout << candies[i][j] << " ";
                }
                cout << endl;
            }
        }
        clock1.restart();
        game_saver.close();
    }
    else {
        cout << "No file found. " << endl;
    }
}

void loadtextures() {
    int i = 0;
    while (i < total_textures) {
        Texture candytexture;
        char file_nam[60];
        sprintf_s(file_nam, "images/candies/%d.png", i + 1); 

        if (candytexture.loadFromFile(file_nam)) {
            cout << "Candy texture " << i + 1 << " succeeds" << endl;
            textures[i] = candytexture;
            ++i;
        }
        else {
            cout << "Error occurred during candy_texture loading " << i + 1 << endl;
            exit(1);
        }
    }
    int j = 0;
    while (j < special_textures) {
        Texture candytexture;
        char file_nam[60];
        sprintf_s(file_nam, "images/Special1/%d.png", j + 1); 

        if (candytexture.loadFromFile(file_nam)) {
            cout << "Special texture " << j + 1 << " loaded successfully." << endl;
            specialTextures[j] = candytexture;
            ++j;
        }
        else {
            cout << "Error occurred during special_texture loading " << j + 1 << "." << endl;
            //system("pause");
            exit(1);
        }
    }
}
void initializegrid()
{
    srand(time(0));
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            while ((j >= 2 && candies[i][j] == candies[i][j - 1] && candies[i][j - 1] == candies[i][j - 2]) ||
                (i >= 2 && candies[i][j] == candies[i - 1][j] && candies[i - 1][j] == candies[i - 2][j]) ||
                (i >= 1 && j >= 1 && candies[i][j] == candies[i - 1][j] && candies[i][j] == candies[i - 1][j - 1]) ||
                (i >= 1 && j >= 1 && candies[i][j] == candies[i][j - 1] && candies[i][j] == candies[i - 1][j - 1]) ||
                (i >= 1 && j >= 1 && candies[i][j] == candies[i - 1][j] && candies[i][j] == candies[i - 1][j + 1]) ||
                (i >= 2 && j >= 2 && candies[i][j] == candies[i - 1][j - 1] && candies[i - 1][j - 1] == candies[i - 2][j - 2]) ||
                (i >= 2 && candies[i][j] == candies[i - 1][j + 1] && candies[i - 1][j + 1] == candies[i - 2][j + 2]))
            {

                candies[i][j] = time(0) % total_textures;
            }
        }
    }
}

int valid_swap(int row1, int col1, int row2, int col2)
{
    
    if (abs(row1 - row2) > 1 || abs(col1 - col2) > 1 || abs(row1 - row2) == abs(col1 - col2)) return 0;
    swap(candies[row1][col1], candies[row2][col2]);
    int result = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {

            if (j < col - 2 && candies[i][j] == candies[i][j + 1] && candies[i][j] == candies[i][j + 2]) {
                result = 1;
                break;
            }
            //row
            if (j > 1 && candies[i][j] == candies[i][j - 1] && candies[i][j - 1] == candies[i][j - 2]) {
                result = 1;
                break;
            }//column
            if (i < row - 2 && candies[i][j] == candies[i + 1][j] && candies[i][j] == candies[i + 2][j]) {
                result = 1;
                break;
            }
            // back 3 diagonal
            if (i > 1 && candies[i][j] == candies[i - 1][j] && candies[i - 1][j] == candies[i - 2][j]) {
                result = 1;
                break;
            }
            // front 3 diagonal
            if (j < col - 2 && i < row - 2 && candies[i][j] == candies[i + 1][j + 1] && candies[i][j] == candies[i + 2][j + 2]) {
                result = 1;
                break;
            }
            // front 4 diagonal
            if (j < col - 3 && i < row - 2 && candies[i][j] == candies[i + 1][j + 1] && candies[i + 1][j + 1] == candies[i + 2][j + 2]
                && candies[i + 2][j + 2] == candies[i + 3][j + 3]) {
                result = 1;
                break;
            }
            // back 3 diagonal
            if (j >= 2 && i < row - 2 && candies[i][j] == candies[i + 1][j - 1] && candies[i][j] == candies[i + 2][j - 2]) {
                result = 1;
                break;
            }
            // back 4 diagonal
            if (j >= 3 && i < row - 3 && candies[i][j] == candies[i + 1][j - 1] && candies[i + 1][j - 1] == candies[i + 2][j - 2]
                && candies[i + 2][j - 2] == candies[i + 3][j - 3]) {
                result = 1;
                break;
            }
            //elbow
            // TOPLEFT CORNER
            if (j < col - 1 && i < row - 1 && candies[i][j] == candies[i + 1][j] && candies[i][j] == candies[i][j + 1]) {
                result = 1;
                break;
            }
            // TOPRIGHT CORNER
            if (j > 0 && i < row - 1 && candies[i][j] == candies[i + 1][j] && candies[i][j] == candies[i][j - 1]) {
                result = 1;
                break;
            }
            // BOTTOMLEFT CORNER
            if (j < col - 1 && i > 0 && candies[i][j] == candies[i - 1][j] && candies[i][j] == candies[i][j + 1]) {
                result = 1;
                break;
            }
            // BOTTOMRIGHT CORNER
            if (j > 0 && i > 0 && candies[i][j] == candies[i - 1][j] && candies[i][j] == candies[i][j - 1]) {
                result = 1;
                break;
            }
        }
    }
    
    swap(candies[row1][col1], candies[row2][col2]);

    return result;

}


int row_col_matching() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            //horizntal
            if (j < col - 2 && candies[i][j] == candies[i][j + 1] && candies[i][j + 1] == candies[i][j + 2]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i][j + 1] == false && assigned_toBurst[i][j + 2] == false) {
                if (j < col - 3 && candies[i][j] == candies[i][j + 3]
                    && assigned_toBurst[i][j + 3] == false) {
                    if (j < col - 4 && candies[i][j] == candies[i][j + 4]
                        && assigned_toBurst[i][j + 4] == false) {
                        assigned_toBurst[i][j] = assigned_toBurst[i][j + 1] = assigned_toBurst[i][j + 2] = assigned_toBurst[i][j + 3] = assigned_toBurst[i][j + 4] = true;
                        SpecialCandy_1 = true; breakTimeoutActive = true;
                    }
                    else {
                        
                        assigned_toBurst[i][j] = assigned_toBurst[i][j + 1] = assigned_toBurst[i][j + 2] = assigned_toBurst[i][j + 3] = true;
                        breakTimeoutActive = true;
                        SpecialCandy = true;
                    }
                }
                else {
                    assigned_toBurst[i][j] = assigned_toBurst[i][j + 1] = assigned_toBurst[i][j + 2] = true;
                    breakTimeoutActive = true;
                }
            }
            //vertical
            if (i < row - 2 && candies[i][j] == candies[i + 1][j] && candies[i + 1][j] == candies[i + 2][j]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i + 1][j] == false && assigned_toBurst[i + 2][j] == false) {
                if (i < row - 3 && candies[i][j] == candies[i + 3][j]
                    && assigned_toBurst[i + 3][j] == false) {
                    if (i < row - 4 && candies[i][j] == candies[i + 4][j]
                        && assigned_toBurst[i + 4][j] == false) {
                        assigned_toBurst[i][j] = assigned_toBurst[i + 1][j] = assigned_toBurst[i + 2][j] = assigned_toBurst[i + 3][j] = assigned_toBurst[i][j + 4] = true;
                        SpecialCandy_1 = true; breakTimeoutActive = true;
                    }
                    else {
                        assigned_toBurst[i][j] = assigned_toBurst[i + 1][j] = assigned_toBurst[i + 2][j] = assigned_toBurst[i + 3][j] = true;
                        breakTimeoutActive = true;
                        SpecialCandy = true;
                    }
                }
                else {
                    assigned_toBurst[i][j] = assigned_toBurst[i + 1][j] = assigned_toBurst[i + 2][j] = true;
                    breakTimeoutActive = true;
                }
            }
            //diagonal matches (from top-left to bottom-right)

            if (i < row - 2 && j < col - 2 && candies[i][j] == candies[i + 1][j + 1] && candies[i + 1][j + 1] == candies[i + 2][j + 2]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i + 1][j + 1] == false && assigned_toBurst[i + 2][j + 2] == false) {
                if (i < row - 3 && j < col - 3 && candies[i][j] == candies[i + 3][j + 3]
                    && assigned_toBurst[i + 3][j + 3] == false) {
                    if (i < row - 4 && j < col - 4 && candies[i][j] == candies[i + 4][j + 4]
                        && assigned_toBurst[i + 4][j + 4] == false) {
                        assigned_toBurst[i][j] = assigned_toBurst[i + 1][j + 1] = assigned_toBurst[i + 2][j + 2] = assigned_toBurst[i + 3][j + 3] = assigned_toBurst[i + 4][j + 4] = true;
                        SpecialCandy_1 = true; breakTimeoutActive = true;
                    }
                    else {
                        assigned_toBurst[i][j] = assigned_toBurst[i + 1][j + 1] = assigned_toBurst[i + 2][j + 2] = assigned_toBurst[i + 3][j + 3] = true;
                        breakTimeoutActive = true;
                        SpecialCandy = true;
                    }
                }
                else {
                    assigned_toBurst[i][j] = assigned_toBurst[i + 1][j + 1] = assigned_toBurst[i + 2][j + 2] = true;
                    breakTimeoutActive = true;
                }
            }

            //diagonal matches (from top-right to bottom-left)
            if (i < row - 2 && j > 1 && candies[i][j] == candies[i + 1][j - 1] && candies[i + 1][j - 1] == candies[i + 2][j - 2]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i + 1][j - 1] == false && assigned_toBurst[i + 2][j - 2] == false) {
                if (i < row - 3 && j > 2 && candies[i][j] == candies[i + 3][j - 3]
                    && assigned_toBurst[i + 3][j - 3] == false) {
                    if (i < row - 4 && j > 3 && candies[i][j] == candies[i + 4][j - 4]
                        && assigned_toBurst[i + 4][j - 4] == false) {
                        assigned_toBurst[i][j] = assigned_toBurst[i + 1][j - 1] = assigned_toBurst[i + 2][j - 2] = assigned_toBurst[i + 3][j - 3] = assigned_toBurst[i + 4][j - 4] = true;
                        SpecialCandy_1 = true; breakTimeoutActive = true;
                    }
                    else {
                        assigned_toBurst[i][j] = assigned_toBurst[i + 1][j - 1] = assigned_toBurst[i + 2][j - 2] = assigned_toBurst[i + 3][j - 3] = true;
                        breakTimeoutActive = true;
                        SpecialCandy = true;
                    }
                }
                else {
                    assigned_toBurst[i][j] = assigned_toBurst[i + 1][j - 1] = assigned_toBurst[i + 2][j - 2] = true;
                    breakTimeoutActive = true;
                }
            }
            //elbow conditions
            if (j < col - 1 && i < row - 1 && candies[i][j] == candies[i + 1][j] && candies[i + 1][j] == candies[i + 1][j + 1]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i + 1][j] == false && assigned_toBurst[i + 1][j + 1] == false) {
                assigned_toBurst[i][j] = assigned_toBurst[i + 1][j] = assigned_toBurst[i + 1][j + 1] = true;
                breakTimeoutActive = true;
            }
            if (j > 0 && i < row - 1 && candies[i][j] == candies[i + 1][j] && candies[i + 1][j] == candies[i + 1][j - 1]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i + 1][j] == false && assigned_toBurst[i + 1][j - 1] == false) {
                assigned_toBurst[i][j] = assigned_toBurst[i + 1][j] = assigned_toBurst[i + 1][j - 1] = true;
                breakTimeoutActive = true;
            }
            if (j < col - 1 && i < row - 1 && candies[i][j] == candies[i][j + 1] && candies[i][j + 1] == candies[i + 1][j + 1]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i][j + 1] == false && assigned_toBurst[i + 1][j + 1] == false) {
                assigned_toBurst[i][j] = assigned_toBurst[i][j + 1] = assigned_toBurst[i + 1][j + 1] = true;
                breakTimeoutActive = true;
            }
            if (j < col - 1 && i < row - 1 && candies[i][j] == candies[i + 1][j] && candies[i + 1][j] == candies[i][j + 1]
                && assigned_toBurst[i][j] == false && assigned_toBurst[i + 1][j] == false && assigned_toBurst[i][j + 1] == false) {
                assigned_toBurst[i][j] = assigned_toBurst[i + 1][j] = assigned_toBurst[i][j + 1] = true;
                breakTimeoutActive = true;
            }

        }
    }
    return breakTimeoutActive;
}
void scoreText_updation(Text& scoreText) {
    char scorestorage[15];
    const int s = score1;
    snprintf(scorestorage, sizeof(scorestorage), "Score: %d", s);
    scoreText.setString(scorestorage);

}

void movesText_updation(Text& movesText) {
    char movesstorage[30];
    snprintf(movesstorage, sizeof(movesstorage), "Moves: %d", moves);
    movesText.setString(movesstorage);
}

void TimerText_updation(Text& timertext) {
    
    int time_left = timer - static_cast<int>(clock1.getElapsedTime().asSeconds());
    char timerstorage[30];
    snprintf(timerstorage, sizeof(timerstorage), "Timer: %d", time_left);
    timertext.setString(timerstorage);
}

bool HINT(Vector2i& suggested_move, Vector2i& selectedtile) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            
            if (valid_swap(selectedtile.y, selectedtile.x, i, j)) {
                suggested_move.x = j;
                suggested_move.y = i;
                //when suggested move happens, it stops
                return true;
            }
        }
    }
    //No valid move is found
    return false;
}

bool isAnyMoveValid() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (i > 0) { if (valid_swap(i, j, i - 1, j)) return true; }
            if (i < 4) { if (valid_swap(i, j, i + 1, j)) return true; }
            if (j > 0) { if (valid_swap(i, j, i, j - 1)) return true; }
            if (i > 0) { if (valid_swap(i, j, i, j + 1)) return true; }
        }
    }
    return false;
}

void shuffleBoard() {
    
    int sumcandies[special_textures + total_textures] = { 0 };
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            sumcandies[candies[i][j]]++;
        }
    }
    int random_Candy;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            do {
                random_Candy = rand() % (special_textures + total_textures);
            } while (sumcandies[random_Candy] <= 0);
            candies[i][j] = random_Candy;
            sumcandies[random_Candy]--;
        }
    }
}

void sameCandies_bursting() {
    int burstcount = 0;

    //already done our swapping process. Just to mark that candies about it
    for (int i = row - 1; i >= 0; --i)
    {
        for (int j = 0; j < col; ++j) {
            if (assigned_toBurst[i][j]) {
                if (i == 0) {
                    candies[i][j] = rand() % total_textures;
                }
                else {
                    candies[i][j] = -1;
                }
                assigned_toBurst[i][j] = false;
                ++burstcount;
            }
        }
    }
    cout << "BURST COUNT " << burstcount << endl;
    for (int i = row - 1; i >= 0; --i) {
        for (int j = 0; j < col; ++j) {
            if (candies[i][j] == -1) {
                for (int k = i; k >= 0; k--) {
                    if (candies[k][j] != -1) {
                        if (SpecialCandy_1 == true) {

                            candies[i][j] = 7;
                            if (moves != 50) score1 += 10;
                            scoreText_updation(scoretext);
                            assigned_toBurst[i][j] = true;
                            breakTimeout = 30;
                            breakTimeoutActive = true;
                            SpecialCandy_1 = false;

                            //row column bursting
                            //row bursting
                            for (int k = 0; k < col; k++) {
                                assigned_toBurst[i][k] = true;
                            }
                            //column bursting
                            for (int k = 0; k < row; k++) {
                                assigned_toBurst[k][j] = true;
                            }
                            break;
                        }

                        else if (SpecialCandy == true) {
                            candies[i][j] = 6;
                            if (moves != 20) score1 += 5;
                            scoreText_updation(scoretext);
                            //bursting of 3 by 3 region
                            assigned_toBurst[i][j] = true;
                            breakTimeout = 30;
                            breakTimeoutActive = true;
                            //above row
                            if (i > 0) {
                                assigned_toBurst[i - 1][j] = true;
                                if (j > 0) {
                                    assigned_toBurst[i - 1][j - 1] = true;
                                }
                                if (j < row - 1) {
                                    assigned_toBurst[i - 1][j + 1] = true;
                                }
                            }
                            //below row
                            if (i < col - 1) {
                                assigned_toBurst[i + 1][j] = true;
                                if (j > 0) {
                                    assigned_toBurst[i + 1][j - 1] = true;
                                }
                                if (j < row - 1) {
                                    assigned_toBurst[i + 1][j + 1] = true;
                                }
                            }
                            //row in which special candy is present
                            if (j > 0) {
                                assigned_toBurst[i][j - 1] = true;
                            }
                            if (j < row - 1) {
                                assigned_toBurst[i][j + 1] = true;
                            }
                            SpecialCandy = 0;
                            break;
                        }
                        else {
                            candies[i][j] = candies[k][j];
                            candies[k][j] = -1;
                            break;
                        }
                    }
                    if (candies[i][j] == -1) {
                        candies[i][j] = rand() % total_textures;
                    }
                }
            }
        }
    }
}