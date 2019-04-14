/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.cpp
*/

#include <iostream> // tmp
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
    scatterCell("Anthill", Vector2i(0, 0), 3);
    for (size_t i = rand() % 3; i < 4; i++)
        scatterCell("Wall", randCellPos("Ground"), 2 + rand() % 3);
    for (size_t i = rand() % 5; i < 8; i++) // ?
        scatterCell("Food", randCellPos("Ground"), 1 + rand() % 2);
    for (auto &line : cell)
        for (auto &one : line.second)
            if (one.second.type == "Food") {
                putResource(Vector2i(line.first, one.first), "Food", 1 + rand() % 3);
                one.second.type = "Ground";
            }
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

Uint32 World::putResource(const Vector2i &pos, const string &type, const Uint32 &quantity)
{
    //if (!cellExist(pos))
    //throw Error("Cell does not exist"); // TODO
    //if (cell[pos.x][pos.y].resource[type] + quantity > quantityMax) // TODO
    cell[pos.x][pos.y].resource[type] += quantity;
    return quantity;
}

Uint32 World::pickResource(const Vector2i &pos, const string &type, const Uint32 &quantity)
{
    //if (!cellExist(pos))
    //throw Error("Cell does not exist"); // TODO
    if (cell[pos.x][pos.y].resource.find(type) == cell[pos.x][pos.y].resource.end())
        return 0;
    if (cell[pos.x][pos.y].resource[type] >= quantity) {
        cell[pos.x][pos.y].resource[type] -= quantity;
        return quantity;
    }
    else {
        const Uint32 newQuantity = cell[pos.x][pos.y].resource[type];

        cell[pos.x][pos.y].resource[type] = 0;
        return newQuantity;
    }
}

void World::aff(RenderWindow &window) const
{
    ConvexShape hexagon = getHexagon();
    const float tmp = sqrt(pow(-0.5 * hexagonRadius, 2) + pow(sin(M_PI / 3) * hexagonRadius, 2)) / 2;
    const float size = sqrt(pow(hexagonRadius, 2) - pow(tmp, 2)) * 2;
    Texture texture; // !!!
    RectangleShape rectange(Vector2f(size, size)); // !!!

    texture.loadFromFile("Resources/Texture/Food.png"); // tmp
    rectange.setOrigin(rectange.getSize() / (float)2);
    rectange.setTexture(&texture);
    for (auto line : cell)
        for (auto one : line.second)
            if (typeColor.find(one.second.type) != typeColor.end()) {
                hexagon.setPosition(getHexagonPos(Vector2i(line.first, one.first)));
                hexagon.setFillColor(typeColor.at(one.second.type));
                window.draw(hexagon);
                if (one.second.resource.find("Food") != one.second.resource.end() && one.second.resource["Food"] > 0) {
                    rectange.setPosition(getHexagonPos(Vector2i(line.first, one.first)));
                    window.draw(rectange);
                }
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
                            newCell[line.first + i][one.first + j].type = "Wall";
            newCell[line.first][one.first].type = "Ground";
        }
    cell = newCell;
}

Vector2i World::randCellPos(const string &type) const
{
    vector<Vector2i> allCell;

    for (auto line : cell)
        for (auto one : line.second)
            if (type.empty() || cell.at(line.first).at(one.first).type == type)
                allCell.push_back(Vector2i(line.first, one.first));
    //if (!allCell.size())
    //throw Error("No cell of type \"" + type + "\""); // TODO
    return allCell[rand() % allCell.size()];
}

void World::scatterCell(const string &type, const Vector2i &pos, const size_t &size)
{
    vector<Vector2i> scatte = {pos};

    if (cell[pos.x][pos.y].type != "Ground")
        return;
    cell[pos.x][pos.y].type = type;
    for (size_t n = 1; n < size; n++) {
        vector<Vector2i> freeCell;

        for (const Vector2i &st : scatte) // ?
            for (Int8 i = -1; i <= 1; i++)
                for (Int8 j = -1; j <= 1; j++)
                    if (i + j)
                        if (cellExist(Vector2i(st.x + i, st.y + j)) && cell[st.x + i][st.y + j].type == "Ground")
                            freeCell.push_back(Vector2i(st.x + i, st.y + j));
        if (!freeCell.size())
            break;
        scatte.push_back(freeCell[rand() % freeCell.size()]);
        cell[scatte.back().x][scatte.back().y].type = type;
    }
}
