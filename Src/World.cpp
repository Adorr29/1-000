/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.cpp
*/

#include "World.hpp"

World::World()
    : hexagonRadius(20)
{
    cell[0][0].type = "Wall";
    for (size_t i = 0; i < 10; i++)
        sizeUp();
}

World::~World()
{
}

void World::aff(RenderWindow &window) const
{
    ConvexShape hexagon(6);

    for (Int8 i = 0; i < 6; i++)
        hexagon.setPoint(i, Vector2f(cos(i * M_PI / 3) * (hexagonRadius - 1), sin(i * M_PI / 3) * (hexagonRadius - 1)));

    for (auto line : cell)
        for (auto one : line.second) {
            hexagon.setPosition(1.5 * hexagonRadius * line.first, (sin(M_PI / 3) * hexagonRadius) * (-line.first + 2 * one.first));
            hexagon.move(window.getSize().x / 2, window.getSize().y / 2); // ?
            if (one.second.type == "Wall")
                hexagon.setFillColor(Color::White);
            else if (one.second.type == "Ground")
                hexagon.setFillColor(Color(0, 0, 0, 100));
            window.draw(hexagon);
        }
}

void World::sizeUp()
{
    map<Int32, map<Int32, Cell>> newCell = cell;

    for (auto line : cell)
        for (auto one : line.second) {
            for (Int8 i = -1; i <= 1; i++)
                for (Int8 j = -1; j <= 1; j++)
                    if (i + j)
                        if (cell.find(line.first + i) == cell.end() || cell[line.first + i].find(one.first + j) == cell[line.first + i].end())
                            newCell[line.first + i][one.first + j] = {"Wall"};
            newCell[line.first][one.first].type = "Ground";
        }
    cell = newCell;
}
