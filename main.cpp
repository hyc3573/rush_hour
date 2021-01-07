#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <Windows.h>

#define SWIDTH 800
#define SHEIGHT 800

#define BWIDTH 6
#define BHEIGHT 6

const float GWIDTH = SWIDTH / BWIDTH;
const float GHEIGHT = SHEIGHT / BHEIGHT;

typedef int Square;

typedef enum Color  {Red, Green, Blue, Yellow, Orange, Purple, Cyan, Pink, Blank} Color;
sf::Color cTable[] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow, 
sf::Color(0xF28A2EFF), sf::Color(0x25064cFF), sf::Color(0x00FFFF), sf::Color(0xf23869ff), sf::Color::Black };
sf::Color border = sf::Color(0x404040ff);

class Board : public std::vector<std::vector<Color>>
{
public:
    Board()
    {
        for (int X = 0;X < BWIDTH;X++)
        {
            this->push_back(vector<Color>(BHEIGHT, Blank));
        }
    }
    bool isIllegal(int X, int Y)
    {
        return X < 0 || X > BWIDTH - 1 || Y < 0 || Y > BHEIGHT - 1;
    }
};

class Car
{
private:
    Board& board;
    std::vector<std::pair<int, int>> positions; // last element must have biggest coordinates.
    Color color;
    int Xdir;
    int Ydir;
public:
    Car(Board& board, int X, int Y, int length, int Xdir, int Ydir, Color color) : board(board), color(color), Xdir(Xdir), Ydir(Ydir)
    {
        for (int i = 0;i < length;i++)
        {
            positions.push_back(std::make_pair(X + i * Xdir, Y + i * Ydir));
            board[X + i * Xdir][Y + i * Ydir] = color;
        }
    }
    bool move(int dir)
    {
        if (dir == 1)
        {
            for (auto it = positions.rbegin();it < positions.rend();it++)
            {
                if (board.isIllegal(it->first + Xdir * dir, it->second + Ydir * dir) || board[it->first + Xdir * dir][it->second + Ydir * dir] != Blank)
                {
                    return false;
                }
                board[it->first + Xdir * dir][it->second + Ydir * dir] = color;
                board[it->first][it->second] = Blank;
                (*it) = std::make_pair(it->first + Xdir * dir, it->second + Ydir * dir);
            }
        }
        else if (dir == -1)
        {
            for (auto it = positions.begin();it < positions.end();it++)
            {
                if (board.isIllegal(it->first + Xdir * dir, it->second + Ydir * dir) || board[it->first + Xdir * dir][it->second + Ydir * dir] != Blank)
                {
                    return false;
                }
                board[it->first + Xdir * dir][it->second + Ydir * dir] = color;
                board[it->first][it->second] = Blank;
                (*it) = std::make_pair(it->first + Xdir * dir, it->second + Ydir * dir);
            }
        }
    }
    sf::FloatRect getGlobalBounds()
    {
        return sf::FloatRect(positions[0].first * GWIDTH, positions[0].second * GHEIGHT,
            Xdir ? positions.size() * GWIDTH : GWIDTH, Ydir ? positions.size() * GHEIGHT : GHEIGHT);
    }
};

int main()
{
    unsigned int t = GetTickCount64();
    srand(t);

    sf::RenderWindow window(sf::VideoMode(SWIDTH, SHEIGHT), L"크고♂아름다운♂게이ㅁ");

    sf::Event event;

    sf::Clock clock;

    Board board;
    std::vector<Car> car;
    /* MAP DEFINITION (FIRST CAR MUST BE RED CAR)*/
    car.push_back(Car(board, 1, 2, 2, 1, 0, Red));
    car.push_back(Car(board, 0, 0, 2, 1, 0, Green));
    car.push_back(Car(board, 0, 1, 3, 0, 1, Purple));
    car.push_back(Car(board, 0, 4, 2, 0, 1, Orange));
    car.push_back(Car(board, 3, 1, 3, 0, 1, Blue));
    car.push_back(Car(board, 5, 0, 3, 0, 1, Yellow));
    car.push_back(Car(board, 4, 4, 2, 1, 0, Cyan));
    car.push_back(Car(board, 2, 5, 3, 1, 0, Pink));
    /* MAP DEFINITION*/

    sf::RectangleShape rect(sf::Vector2f(SWIDTH / BWIDTH, SHEIGHT / BHEIGHT));
    rect.setOutlineColor(border);
    rect.setOutlineThickness(5);

    sf::Vector2f mousepos;

    int focusCar = 0;

    while (window.isOpen()) 
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    car[focusCar].move(-1);
                    break;
                case sf::Keyboard::Right:
                    car[focusCar].move(1);
                    break;
                }
                break;
            case sf::Event::MouseMoved:
                mousepos = (sf::Vector2f)sf::Mouse::getPosition(window);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (int i = 0;i < car.size();i++)
                    {
                        if (car[i].getGlobalBounds().contains(mousepos))
                        {
                            focusCar = i;
                            break;
                        }
                    }
                }
            }
        }

        window.clear();

        for (int X = 0; X < BWIDTH; X++)
        {
            for (int Y = 0; Y < BHEIGHT; Y++)
            {
                rect.setPosition(X * (SWIDTH / BWIDTH), Y * (SHEIGHT / BHEIGHT));
                rect.setFillColor(cTable[board[X][Y]]);
                window.draw(rect);
            }
        }

        window.display();
    }
    return 0;
}