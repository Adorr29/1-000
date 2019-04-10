/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.cpp
*/

#include <cmath>
#include "World.hpp"

// move in other file or in World class TODO
static const map<string, Color> typeColor = {
    {"Wall", Color::White},
    {"Ground", Color(0, 0, 0, 100)},
    {"Anthill", Color(220, 220, 0)}
};

World::World()
    : hexagonRadius(20)
{
    cell[0][0].type = "Wall";
    for (size_t i = 0; i < 10; i++)
        sizeUp();
    cell[0][0].type = "Anthill";
}

World::~World()
{
}

const Cell &World::getCell(const Vector2i &pos) const
{
    //if (!cellExist(pos))
    //throw Error("Cell does not exist"); // TODO
    return cell.at(pos.x).at(pos.y);
}

ConvexShape World::getHexagon() const
{
    ConvexShape hexagon(6);

    for (Int8 i = 0; i < 6; i++)
        hexagon.setPoint(i, Vector2f(cos(i * M_PI / 3) * (hexagonRadius - 1), sin(i * M_PI / 3) * (hexagonRadius - 1)));
    return hexagon;
}

Vector2f World::getHexagonPos(const Vector2i &pos) const
{
    return Vector2f(1.5 * hexagonRadius * pos.x, (sin(M_PI / 3) * hexagonRadius) * (-pos.x + 2 * pos.y));
}

const Uint32 &World::getHexagonRadius() const
{
    return hexagonRadius;
}

void World::aff(RenderWindow &window) const
{
    ConvexShape hexagon = getHexagon();

    for (auto line : cell)
        for (auto one : line.second)
            if (typeColor.find(one.second.type) != typeColor.end()) {
                hexagon.setPosition(getHexagonPos(Vector2i(line.first, one.first)));
                hexagon.setFillColor(typeColor.at(one.second.type));
                window.draw(hexagon);
            }
}

bool World::cellExist(const Vector2i &pos) const
{
    if (cell.find(pos.x) == cell.end())
        return false;
    if (cell.at(pos.x).find(pos.y) == cell.at(pos.x).end())
        return false;
    return true;
}

void World::sizeUp()
{
    map<Int32, map<Int32, Cell>> newCell = cell;

    for (auto line : cell)
        for (auto one : line.second) {
            for (Int8 i = -1; i <= 1; i++)
                for (Int8 j = -1; j <= 1; j++)
                    if (i + j)
                        if (!cellExist(Vector2i(line.first + i, one.first + j)))
                            newCell[line.first + i][one.first + j] = {"Wall"};
            newCell[line.first][one.first].type = "Ground";
        }
    cell = newCell;
}
