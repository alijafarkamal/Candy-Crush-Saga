#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "functions.h"


using namespace std;
using namespace sf;

int main()
{

    loadtextures();
    initializegrid();
   // game(score1, moves);

    Texture backgroundtexture;
    if (!backgroundtexture.loadFromFile("images/background/background.png")) {
        cout << "Error occurred while loading background image." << endl;
        return 1;
    }
    Texture celltexture;
    if (!celltexture.loadFromFile("images/background/cell.png")) {
        cout << "Error occurred while loading cell image." << endl;
        return 1;
    }

    if (!font.loadFromFile("images/text/lato-black.ttf")) {
        cout << "Error occurred while loading font." << endl;
        return 1;
    }
    //MUSIC
    SoundBuffer music;
    if (!music.loadFromFile("candycrush.ogg")) {
        cout << "Music";
    }

    //background
    RectangleShape background(Vector2f(1400.0f, 900.0f));
    background.setTexture(&backgroundtexture);

    const float candysize = 60.0f;
    float candyscale = candysize / static_cast<float>(textures[0].getSize().x);

    //score
    scoretext.setFont(font);
    scoretext.setCharacterSize(24);
    scoretext.setFillColor(Color::Black);
    scoretext.setPosition(10, 650);

    //moves
    movestext.setFont(font);
    movestext.setCharacterSize(24);
    movestext.setFillColor(Color::Black);
    movestext.setPosition(10, 700);

    //timer
    timertext.setFont(font);
    timertext.setCharacterSize(24);
    timertext.setFillColor(Color::Black);
    timertext.setPosition(10, 750);

    //Hint button
    RectangleShape button(Vector2f(70.0f, 50.0f));
    button.setFillColor(Color::Red);
    button.setPosition(1230, 30);
    //loadgame button
    RectangleShape button1(Vector2f(140.0f, 50.0f));
    button1.setFillColor(Color::Blue);
    button1.setPosition(1230, 80);
    //savegame button
    RectangleShape button2(Vector2f(140.0f, 50.0f));
    button2.setFillColor(Color::Green);
    button2.setPosition(1230, 130);

    //text of hint
    Text btntext;
    btntext.setFont(font);
    btntext.setCharacterSize(24);
    btntext.setFillColor(Color::Black);
    btntext.setString("Hint");
    btntext.setPosition(button.getPosition().x + 10, button.getPosition().y + 10);
    //text of save_game 
    Text butntext;
    butntext.setFont(font);
    butntext.setCharacterSize(24);
    butntext.setFillColor(Color::Black);
    butntext.setString("Save Game");
    butntext.setPosition(button1.getPosition().x + 10, button1.getPosition().y + 12);

    //text of load_game
    Text butn2text;
    butn2text.setFont(font);
    butn2text.setCharacterSize(24);
    butn2text.setFillColor(Color::Black);
    butn2text.setString("Load Game");
    butn2text.setPosition(button2.getPosition().x + 10, button2.getPosition().y + 12);


    movesText_updation(movestext);
    scoreText_updation(scoretext);

    RectangleShape cell(Vector2f(cell_size, cell_size));
    cell.setTexture(&celltexture);
    bool hintrequested = false;
    //highlightening
    RectangleShape contrast_light(Vector2f(cell_size, cell_size));
    contrast_light.setOutlineThickness(4);
    contrast_light.setOutlineColor(Color::Black);

    Sound sound(music);
    sound.play();
    //timer
    int timer = 60;
    window.setFramerateLimit(60);
    while (window.isOpen()) {

        
        //done to avoid sudden increade in score
        if (score1 - prevScore1 < 0 || score1 - prevScore1 > 100) score1 = prevScore1;
        prevScore1 = score1;
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                //game_loader(score1, moves);
                window.close();

                cout << "Game over\n";
            }

            else if (event.type == Event::MouseButtonPressed) {
                if (!breakTimeoutActive && event.mouseButton.button == Mouse::Left) {
                    int mouse_x = event.mouseButton.x - mid_x;
                    int mouse_y = event.mouseButton.y - mid_y;
                    
                    //Load game
                    if (button1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        game_loader(score1, moves);
                    }
                    //Save game
                    else if (button2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        game(score1, moves);
                        scoreText_updation(scoretext);
                        movesText_updation(movestext);
                    }
                    //HINT
                    else if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        cout << "Button clicked!\n";
                        hintrequested = true;
                        Vector2i tile;
                        suggested_move = { -1,-1 };
                        for (int i = 0; i < row; i++) {
                            if (suggested_move.x != -1) break;
                            for (int j = 0; j < col; j++) {
                                if (suggested_move.x != -1) break;
                                tile = { i,j };
                                HINT(suggested_move, tile);
                            }
                        }
                        cout << suggested_move.x << " " << suggested_move.y;

                    }
                    else {

                        int grid_x = mouse_x / cell_size;
                        int grid_y = mouse_y / cell_size;

                        //2D array index
                        if (grid_x >= 0 && grid_x < col && grid_y >= 0 && grid_y < row) {
                            if (selectedtile.x == -1 && selectedtile.y == -1)
                                selectedtile = Vector2i(grid_x, grid_y);

                            else {

                                if (valid_swap(selectedtile.y, selectedtile.x, grid_y, grid_x)) {

                                    //after validity checking
                                    suggested_move = { -1,-1 };
                                    swap(candies[selectedtile.y][selectedtile.x], candies[grid_y][grid_x]);
                                    selectedtile = Vector2i(-1, -1);
                                    row_col_matching();
                                    sameCandies_bursting();
                                    score1++;
                                    clock1.restart();
                                    scoreText_updation(scoretext);
                                    moves -= 1;
                                    movesText_updation(movestext);

                                    if (moves == 0) {
                                        moves = 0;
                                        score1 = 0;
                                        //game_loader(score1, moves);
                                        window.close();
                                        cout << "Game over\n";
                                    }
                                }

                                else {
                                    selectedtile = Vector2i(grid_x, grid_y);
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        window.draw(background);

        row_col_matching();
        //means any bursting happens, time pauses
        if (breakTimeoutActive) {
            clock1.restart();
            breakTimeout--;
        }
        else breakTimeout = 30;
        if ((breakTimeoutActive && moves == 20) || breakTimeout == 0) {
            breakTimeoutActive = false;
            breakTimeout = 30;
            sameCandies_bursting();
        }

        if (!breakTimeoutActive) {
            if (!isAnyMoveValid()) {
                shuffleBoard();
            }
        }
        //means any valid move
        if (suggested_move.x != -1 && suggested_move.y != -1)
        {
            contrast_light.setPosition(mid_x + suggested_move.x * cell_size, mid_y + suggested_move.y * cell_size);
            window.draw(contrast_light);
        }

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j)
            {
                cell.setPosition(mid_x + j * cell_size, mid_y + i * cell_size);
                Color cellcolor = cell.getFillColor();
                cellcolor.a = 170;
                cell.setFillColor(cellcolor);
                //differentiate between special and normal candies
                if (candies[i][j] > 5) {
                    sprite.setTexture(specialTextures[candies[i][j] - 6]);
                }
                else {
                    sprite.setTexture(textures[candies[i][j]]);
                }

                sprite.setScale(candyscale, candyscale);
                sprite.setPosition(cell.getPosition().x + (cell_size - candysize) / 2, cell.getPosition().y + (cell_size - candysize) / 2);

                // highlight the selected candy
                if (assigned_toBurst[i][j] || (selectedtile.x == j && selectedtile.y == i)) {
                    contrast_light.setPosition(cell.getPosition());
                    window.draw(contrast_light);
                }

                window.draw(scoretext);
                window.draw(movestext);

            label:

                timer = 30 - static_cast<int>(clock1.getElapsedTime().asSeconds());
                if (timer <= 0) {
                    moves -= 1;
                    score1 -= 5;
                    prevScore1 = score1;
                    //game_loader(score1, moves);
                    movesText_updation(movestext);
                    scoreText_updation(scoretext);
                    TimerText_updation(timertext);
                    if (moves <= 0) {
                        moves = 0;
                        score1 = 0;
                        //game_loader(score1, moves);
                        cout << "Game over\n";
                        window.close();
                    }
                    else {
                        clock1.restart(); 
                        goto label;
                    }
                }
                else {
                    TimerText_updation(timertext);
                    window.draw(timertext);
                    window.draw(timertext);
                    window.draw(cell);
                }
                window.draw(sprite);
            }
            window.draw(button);
            window.draw(button1);
            window.draw(button2);

            window.draw(btntext);
            window.draw(butntext);
            window.draw(butn2text);
        }
        window.display();
    }
        return 0;
}
