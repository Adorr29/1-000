/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.cpp
*/

#include <iostream> // tmp
#include <cmath>
#include "World.hpp"

// TODO move in other file or in World class
static const map<string, Color> typeColor = {
    {"Wall", Color::White},
    {"Ground", Color(0, 0, 0, 100)},
    {"Anthill", Color(220, 220, 0)}
};

World::World(const size_t &size)
    : hexagonRadius(20)
{
    cell[0][0].type = "Wall";
    for (size_t i = 0; i < size; i++)
        sizeUp();
    scatterCell("Anthill", Vector2i(0, 0), 7);
    for (size_t i = rand() % 10; i < 30; i++)
        scatterCell("Wall", randCellPosition("Ground"), 10 + rand() % 30);
    for (size_t i = rand() % 5; i < 20; i++) // ?
        scatterCell("Food", randCellPosition("Ground"), 5 + rand() % 10);
    for (auto &line : cell)
        for (auto &one : line.second)
            if (one.second.type == "Food") {
                putResource(Vector2i(line.first, one.first), "Food", 20 + rand() % 40);
                one.second.type = "Ground";
            }
}

World::~World()
{
}

const Cell &World::getCell(const Vector2i &position) const
{
    //if (!cellExist(position))
    //throw Error("Cell does not exist"); // TODO
    return cell.at(position.x).at(position.y);
}

ConvexShape World::getHexagon() const
{
    ConvexShape hexagon(6);

    for (Int8 i = 0; i < 6; i++)
        hexagon.setPoint(i, Vector2f(cos(i * M_PI / 3) * (hexagonRadius - 1), sin(i * M_PI / 3) * (hexagonRadius - 1)));
    return hexagon;
}

Vector2f World::getHexagonPosition(const Vector2i &position) const
{
    return Vector2f(1.5 * hexagonRadius * position.x, (sin(M_PI / 3) * hexagonRadius) * (-position.x + 2 * position.y));
}

const Uint32 &World::getHexagonRadius() const
{
    return hexagonRadius;
}

Uint32 World::putResource(const Vector2i &position, const string &type, const Uint32 &quantity)
{
    //if (!cellExist(position))
    //throw Error("Cell does not exist"); // TODO
    //if (cell[position.x][position.y].resource[type] + quantity > quantityMax) // TODO
    cell[position.x][position.y].resource[type] += quantity;
    return quantity;
}

Uint32 World::pickResource(const Vector2i &position, const string &type, const Uint32 &quantity)
{
    //if (!cellExist(position))
    //throw Error("Cell does not exist"); // TODO
    if (cell[position.x][position.y].resource.find(type) == cell[position.x][position.y].resource.end())
        return 0;
    if (cell[position.x][position.y].resource[type] >= quantity) {
        cell[position.x][position.y].resource[type] -= quantity;
        return quantity;
    }
    else {
        const Uint32 newQuantity = cell[position.x][position.y].resource[type];

        cell[position.x][position.y].resource[type] = 0;
        return newQuantity;
    }
}

void World::putPheromone(const Vector2i &position, const size_t &id, const string &type, const Uint32 &quantity)
{
    //if (!cellExist(position))
    //throw Error("Cell does not exist"); // TODO
    for (Pheromone &one : cell[position.x][position.y].pheromone)
        if (one.ownerId == id && one.type == type) {
            one.quantity = quantity;
            return;
        }
    cell[position.x][position.y].pheromone.push_back((Pheromone){type, id, quantity});
}

void World::update()
{
    for (auto &line : cell)
        for (auto &one : line.second)
            for (auto it = one.second.pheromone.begin(); it != one.second.pheromone.end(); it++)
                if (it->quantity >= 1) // && rand() % 10 == 0 ?
                    it->quantity--;
                else
                    it = one.second.pheromone.erase(it);

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
                hexagon.setPosition(getHexagonPosition(Vector2i(line.first, one.first)));
                hexagon.setFillColor(typeColor.at(one.second.type));
                for (const Pheromone &phero : one.second.pheromone)
                    if (phero.type == "Search")
                        hexagon.setFillColor(Color(255 * (phero.quantity / 1200.0), 0, 0));
                window.draw(hexagon);
                if (one.second.resource.find("Food") != one.second.resource.end() && one.second.resource["Food"] > 0) {
                    rectange.setPosition(getHexagonPosition(Vector2i(line.first, one.first)));
                    window.draw(rectange);
                }
            }
}

bool World::cellExist(const Vector2i &position) const
{
    if (cell.find(position.x) == cell.end())
        return false;
    if (cell.at(position.x).find(position.y) == cell.at(position.x).end())
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

Vector2i World::randCellPosition(const string &type) const
{
    vector<Vector2i> allCell;

    for (auto line : cell)
        for (auto one : line.second)
            if (type.empty() || cell.at(line.first).at(one.first).type == type)
                allCell.push_back(Vector2i(line.first, one.first));
    //if (allCell.empty())
    //throw Error("No cell of type \"" + type + "\""); // TODO
    return allCell[rand() % allCell.size()];
}

void World::scatterCell(const string &type, const Vector2i &position, const size_t &size)
{
    vector<Vector2i> scatte = {position};

    if (cell[position.x][position.y].type != "Ground")
        return; // or throw ?
    cell[position.x][position.y].type = type;
    for (size_t n = 1; n < size; n++) {
        vector<Vector2i> freeCell;

        for (const Vector2i &st : scatte) // ?
            for (Int8 i = -1; i <= 1; i++)
                for (Int8 j = -1; j <= 1; j++)
                    if (i + j)
                        if (cellExist(Vector2i(st.x + i, st.y + j)) && cell[st.x + i][st.y + j].type == "Ground")
                            freeCell.push_back(Vector2i(st.x + i, st.y + j));
        if (freeCell.empty())
            break;
        scatte.push_back(freeCell[rand() % freeCell.size()]);
        cell[scatte.back().x][scatte.back().y].type = type;
    }
}
