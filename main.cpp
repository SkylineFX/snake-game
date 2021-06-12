#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include <time.h>
#include <windows.h>
#include <string>
#include <fstream>

using namespace std;
using namespace sf;

int N = 30, M = 20;
int sprite_size = 32;
int width = sprite_size * N;
int height = sprite_size * (M + 1);
bool isAlive = true, frame_direction = false;
int score = 0, max_score = 0;

char direction[10] = "Right";
int snake_lenght = 4;
struct snake
{
    int x, y;
} s[500];

struct fruit
{
    int x, y;
} f;

void Restart()
{
    Sleep(500);
    isAlive = true;
    for (int i = 0; i < snake_lenght; i++)
    {
        s[i].x = 0; s[i].y = 0;
    }
    snake_lenght = 4;
    score = 0;

    strcpy_s(direction, "Right");
    f.x = rand() % N;
    f.y = rand() % M;
}

void GameTick(Text &score_string)
{
    for (int i = snake_lenght; i > 0; i--)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    if (strstr(direction, "Up")) s[0].y -= 1;
    if (strstr(direction, "Down")) s[0].y += 1;
    if (strstr(direction, "Right")) s[0].x += 1;
    if (strstr(direction, "Left")) s[0].x -= 1;

    for (int i = 1; i < snake_lenght; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y)
            isAlive = false;


    if (s[0].x < 0 || s[0].y < 0 || s[0].x > N - 1 || s[0].y > M - 1)
        isAlive = false;

    if (s[0].x == f.x && s[0].y == f.y)
    {
        snake_lenght++; score++;
        f.x = rand() % N;
        f.y = rand() % M;
        bool f_onSnake = true;
        while (f_onSnake == true)
        {
            bool f_found = false;
            for (int i = 0; i < snake_lenght; i++)
                if (f.x == s[i].x && f.y == s[i].y)
                {
                    f_found = true; i = snake_lenght;
                    f.x = rand() % N;
                    f.y = rand() % M;
                }
            if (f_found == false)
                f_onSnake = false;
        }
    }
    score_string.setString(std::to_string(score));
    frame_direction = false;
}

int main()
{
    RenderWindow window(VideoMode(width, height), "Snake", Style::Titlebar | Style::Close);
    window.setFramerateLimit(10);

    Texture bg_texture, snake_texture, snake_head;
    bg_texture.loadFromFile("images/snake_bg.png");
    snake_texture.loadFromFile("images/snake_body.png");
    snake_head.loadFromFile("images/snake_head.png");

    Sprite bg_sprite(bg_texture);
    Sprite snake_sprite(snake_texture);
    Sprite head_sprite(snake_head);

    Texture edge_top_texture, edge_right_texture, edge_bottom_texture, edge_left_texture;
    edge_top_texture.loadFromFile("images/Up.png"); edge_right_texture.loadFromFile("images/Right.png");
    edge_bottom_texture.loadFromFile("images/Down.png"); edge_left_texture.loadFromFile("images/Left.png");

    Sprite edge_top_sprite(edge_top_texture);
    Sprite edge_right_sprite(edge_right_texture);
    Sprite edge_bottom_sprite(edge_bottom_texture);
    Sprite edge_left_sprite(edge_left_texture);

    Texture corner_tl_texture, corner_tr_texture, corner_bl_texture, corner_br_texture;
    corner_tl_texture.loadFromFile("images/tl.png"); corner_tr_texture.loadFromFile("images/tr.png");
    corner_bl_texture.loadFromFile("images/bl.png"); corner_br_texture.loadFromFile("images/br.png");

    Sprite corner_tl_sprite(corner_tl_texture);
    Sprite corner_tr_sprite(corner_tr_texture);
    Sprite corner_bl_sprite(corner_bl_texture);
    Sprite corner_br_sprite(corner_br_texture);

    Texture fruit_texture, collision_texture;
    fruit_texture.loadFromFile("images/Fruit.png");
    collision_texture.loadFromFile("images/collision.png");
    
    Sprite fruit_sprite(fruit_texture);
    Sprite collision_sprite(collision_texture);

    Font SigmarOne;
    SigmarOne.loadFromFile("fonts/SigmarOne-Regular.ttf");

    Text score_text("Score: ", SigmarOne);
    score_text.setCharacterSize(32);
    score_text.setFillColor(Color(124, 25, 35));
    score_text.setPosition(1 * sprite_size, M * sprite_size - 5);

    Text score_string(to_string(score), SigmarOne);
    score_string.setCharacterSize(32);
    score_string.setFillColor(Color(124, 25, 35));
    score_string.setPosition(5 * sprite_size, M * sprite_size - 5);

    ifstream hs_in("highscore.txt");
    ofstream hs_out;
    hs_in >> max_score;

    Text max_score_text("Highscore: ", SigmarOne);
    max_score_text.setCharacterSize(32);
    max_score_text.setFillColor(Color(124, 25, 35));
    max_score_text.setPosition((N - 9) * sprite_size, M * sprite_size - 5);

    Text max_score_string(to_string(max_score), SigmarOne);
    max_score_string.setCharacterSize(32);
    max_score_string.setFillColor(Color(124, 25, 35));
    max_score_string.setPosition((N - 2) * sprite_size, M * sprite_size - 5);

    srand((unsigned)time(0));
    f.x = (rand() % (N - 1)) + 1;
    f.y = (rand() % (M - 1)) + 1;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
            
            if ((event.type == Event::KeyPressed) && ((event.key.code == Keyboard::R)))
            {
                hs_out.open("highscore.txt"); 
                hs_out << 0; 
                hs_out.close();
                max_score = 0;
                max_score_string.setString(std::to_string(max_score));
            }
            if ((event.type == Event::KeyPressed) && ((event.key.code == Keyboard::W) || (event.key.code == Keyboard::Up)))
            {
                if (!strstr(direction, "Down") && frame_direction == false)
                {
                    strcpy_s(direction, "Up");
                    frame_direction = true;
                }
            }
            if ((event.type == Event::KeyPressed) && ((event.key.code == Keyboard::S) || (event.key.code == Keyboard::Down)))
            {
                if (!strstr(direction, "Up") && frame_direction == false)
                {
                    strcpy_s(direction, "Down");
                    frame_direction = true;
                }
            }
            if ((event.type == Event::KeyPressed) && ((event.key.code == Keyboard::D) || (event.key.code == Keyboard::Right)))
            {
                if (!strstr(direction, "Left") && frame_direction == false)
                {
                    strcpy_s(direction, "Right");
                    frame_direction = true;
                }
            }
            if ((event.type == Event::KeyPressed) && ((event.key.code == Keyboard::A) || (event.key.code == Keyboard::Left)))
            {
                if (!strstr(direction, "Right") && frame_direction == false)
                {
                    strcpy_s(direction, "Left");
                    frame_direction = true;
                }
            }
        }

        GameTick(score_string);

        window.clear();

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
            {
                if (i == 0 && (j > 0 && j < M - 1))
                {
                    edge_left_sprite.setPosition(i * sprite_size, j * sprite_size);
                    window.draw(edge_left_sprite);
                }
                else if (i == N - 1 && (j > 0 && j < M - 1))
                {
                    edge_right_sprite.setPosition(i * sprite_size, j * sprite_size);
                    window.draw(edge_right_sprite);
                }
                else if (j == 0 && (i > 0 && i < N - 1))
                {
                    edge_top_sprite.setPosition(i * sprite_size, j * sprite_size);
                    window.draw(edge_top_sprite);
                }
                else if (j == M - 1 && (i > 0 && i < N - 1))
                {
                    edge_bottom_sprite.setPosition(i * sprite_size, j * sprite_size);
                    window.draw(edge_bottom_sprite);
                }
                else
                {
                    bg_sprite.setPosition(i * sprite_size, j * sprite_size);
                    window.draw(bg_sprite);
                }

            }
        corner_tl_sprite.setPosition(0 * sprite_size, 0 * sprite_size); window.draw(corner_tl_sprite);
        corner_tr_sprite.setPosition((N - 1) * sprite_size, 0 * sprite_size); window.draw(corner_tr_sprite);
        corner_bl_sprite.setPosition(0 * sprite_size, (M - 1) * sprite_size); window.draw(corner_bl_sprite);
        corner_br_sprite.setPosition((N - 1) * sprite_size, (M - 1) * sprite_size); window.draw(corner_br_sprite);

        head_sprite.setPosition(s[0].x * sprite_size, s[0].y * sprite_size);
        window.draw(head_sprite);
        for (int i = 1; i < snake_lenght; i++)
        {
            snake_sprite.setPosition(s[i].x * sprite_size, s[i].y * sprite_size);
            window.draw(snake_sprite);
        }
        fruit_sprite.setPosition(f.x * sprite_size, f.y * sprite_size);
        window.draw(fruit_sprite);
        
        window.draw(score_text);
        window.draw(score_string);
        window.draw(max_score_text);
        window.draw(max_score_string);
        
        if (!isAlive)
        {
            if (score > max_score)
            {
                max_score = score;
                hs_out.open("highscore.txt"); 
                hs_out << max_score; 
                hs_out.close();
            }
            max_score_string.setString(std::to_string(max_score));
            
            collision_sprite.setPosition(s[0].x * sprite_size, s[0].y * sprite_size);         
            window.draw(collision_sprite);
            window.display();

            Restart();
        }
        
        window.display();
    }
    return 0;
} 