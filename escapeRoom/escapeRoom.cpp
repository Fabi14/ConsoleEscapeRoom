#include <chrono>
#include <iostream>
#include <ranges>
#include <windows.h>
#include <thread>
#include <print>

namespace
{
    constexpr int width{ 20 };
    constexpr int hight{ 20 };

    struct Position
    {
        int x{ 0 };
        int y{ 0 };
    };

    void initRandomGenerator()
    {
        std::srand(static_cast<unsigned int>(std::time(0)));
    }

    Position getRandomPosition()
    {
        const int x{ 1 + std::rand() / ((RAND_MAX + 1) / (width - 1)) };
        const int y{ 1 + std::rand() / ((RAND_MAX + 1) / (hight - 1)) };
        return { x,y };
    }

    Position getCenterPosition()
    {
        const int x{ width / 2 };
        const int y{ hight / 2 };
        return { x,y };
    }

    bool operator==(Position left, Position right)
    {
        return left.x == right.x && left.y == right.y;
    }

    bool isKeyDown(int nVirtKey)
    {
        constexpr auto keyDown{ static_cast<SHORT>(0x8000) };
        return GetKeyState(nVirtKey) & keyDown;
    }
}


int main()
{
    //Init
    using namespace std::chrono_literals;
    using namespace std::views;
    initRandomGenerator();
    Position player{ getCenterPosition() };
    const Position key{ getRandomPosition() };
    const Position door{ getRandomPosition() };
    bool keyFound{ false };

    //Game Loop
    while (true)
    {
        //Draw Game
        system("cls");
        for (auto [y, x] : cartesian_product(iota(0, hight + 1), iota(0, width + 1)))
        {
            if (y == 0 || y == hight)
            {
                std::print("-");
            }
            else if (x == 0 || x == width)
            {
                std::print("|");
            }
            else if (player == Position{ x,y })
            {
                if (keyFound)
                {
                    std::print("0");
                }
                else
                {
                    std::print("O");
                }

            }
            else if (!keyFound && key == Position{ x,y })
            {
                std::print("K");
            }
            else if (door == Position{ x,y })
            {
                std::print("D");
            }
            else
            {
                std::print(" ");
            }

            if (x == width)
            {
                std::println("");
            }
        }

        // Game Logic
        if (player == key)
        {
            keyFound = true;
        }
        if (keyFound && player == door)
        {
            break;
        }

        //User Input
        if (isKeyDown(VK_LEFT))
        {
            player.x = player.x >= 2 ? player.x - 1 : 1;
        }
        else if (isKeyDown(VK_RIGHT))
        {
            player.x = player.x <= width-2 ? player.x + 1 : width-1;
        }
        else if (isKeyDown(VK_UP))
        {
            player.y = player.y >= 2 ? player.y - 1 : 1;
        }
        else if (isKeyDown(VK_DOWN))
        {
            player.y = player.y <= width - 2 ? player.y + 1 : width - 1;
        }

        //Game Clock (Speed of the Game)
        std::this_thread::sleep_for(100ms);
    }

    std::cout << "Gewonnen!";
    std::this_thread::sleep_for(2000ms);
}
