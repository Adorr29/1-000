/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** Ant.cpp
*/

#include <iostream> // tmp
#include <cmath>
#include "Ant.hpp"

// TODO move in other file or in Ant class
static const array<Vector2i, 6> angleWay = { // ?
    Vector2i(0, -1),
    Vector2i(1, 0),
    Vector2i(1, 1),
    Vector2i(0, 1),
    Vector2i(-1, 0),
    Vector2i(-1, -1)
};

size_t Ant::antNum = 1; // 1 or 0 ?

Ant::Ant(World &_world, const Vector2i &_position)
    : world(_world), id(antNum++), hp(10), hpMax(10), resourceCapacity(10), position(_position), angle(0)
{
    if (!texture.loadFromFile("Resources/Texture/Ant.png"))
        cerr << "texture load failed" << endl; // TODO throw Error
}

Ant::~Ant()
{
}

const World &Ant::getWorld() const
{
    return world;
}

const size_t &Ant::getId() const
{
    return id;
}

const Uint32 &Ant::getHp() const
{
    return hp;
}

const Uint32 &Ant::getHpMax() const
{
    return hpMax;
}

Uint32 Ant::getResource(const string &type) const
{
    if (type.empty()) { // ? or getResourceTotal
        Uint32 total = 0;

        for (auto one : resource)
            total += one.second;
        return total;
    }
    const auto it = resource.find(type);

    if (it == resource.end())
        return 0;
    return it->second;
}

const Vector2i &Ant::getPosition() const
{
    return position;
}

const Int8 &Ant::getAngle() const
{
    return angle;
}

void Ant::rotateLeft()
{
    angle--;
    while (angle < 0)
        angle += 6;
}

void Ant::rotateRight()
{
    angle++;
    while (angle >= 6)
        angle -= 6;
}

bool Ant::moveForward()
{
    Vector2i newPosition(position);

    if (angle == 0)
        newPosition.y--;
    else if (angle == 1)
        newPosition.x++;
    else if (angle == 2) {
        newPosition.x++;
        newPosition.y++;
    }
    else if (angle == 3)
        newPosition.y++;
    else if (angle == 4)
        newPosition.x--;
    else if (angle == 5) {
        newPosition.x--;
        newPosition.y--;
    }
    if (world.getCell(newPosition).type == "Wall")
        return false;
    position = newPosition;
    return true;
}

Uint32 Ant::putResource(const string &type, const Uint32 &quantity)
{
    Uint32 put;
    Uint32 newQuantity;
    auto it = resource.find(type);

    if (it == resource.end())
        return 0;
    if (quantity > it->second)
        newQuantity = it->second;
    else
        newQuantity = quantity;
    put = world.putResource(position, type, newQuantity);
    it->second -= put;
    return put;
}

Uint32 Ant::pickResource(const string &type, const Uint32 &quantity)
{
    Uint32 pick;
    Uint32 newQuantity;
    const Uint32 totalResource = getResource();

    if (quantity > resourceCapacity - totalResource)
        newQuantity = resourceCapacity - totalResource;
    else
        newQuantity = quantity;
    pick = world.pickResource(position, type, newQuantity);
    resource[type] += pick;
    return pick;
}

void Ant::putPheromone(const string &type, const Uint32 &quantity)
{
    world.putPheromone(position, id, type, quantity);
}

void Ant::ai()
{
    if (world.getCell(position).type == "Anthill" && getResource("Food"))
        putResource("Food", 1);
    else if (world.getCell(position + angleWay[angle]).type == "Anthill" && getResource("Food"))
        moveForward();
    else if (world.getCell(position).type != "Anthill" && !world.getCell(position).resource.empty() && getResource() < resourceCapacity)
        pickResource("Food", 1);
    else if (getResource("Food")) {
        bool move = false;
        //Uint32 myQuantity = -1;
        array<Uint32, 6> aroundQuantity;
        Uint8 lessQuantity = 0;
        Int8 diff;

        /*for (const Pheromone &one : world.getCell(position).pheromone)
            if (one.ownerId == id && one.type == "Search")
                myQuantity = one.quantity;
                cerr << "myQuantity : " << myQuantity << endl;*/

        for (Uint8 i = 0; i < 6; i++) {
            aroundQuantity[i] = -1;
            for (const Pheromone &one : world.getCell(position + angleWay[i]).pheromone)
                if (one.ownerId == id && one.type == "Search")
                    aroundQuantity[i] = one.quantity;
        }

        for (Uint8 i = 1; i < 6; i++)
            if (aroundQuantity[i] < aroundQuantity[lessQuantity])
                lessQuantity = i;

        cerr << "other : " << endl;
        for (Int8 i = 0; i < 6; i++)
            cerr << "  - " << i << " : " << aroundQuantity[i] << endl;
        cerr << endl;
        cerr << "angle : " << (int)angle << endl;
        cerr << "lessQuantity : " << (int)lessQuantity << endl;
        diff = lessQuantity - angle;
        if (abs(diff) > 3)
            diff = (6 - abs(diff)) * (-diff / abs(diff));
        cerr << "diff : " << (int)diff << endl << endl << endl;

        if (diff == 0)
            move = true;
        else {
            if (diff < 0)
                rotateLeft();
            else
                rotateRight();
            if (abs(diff) == 1)
                move = true;
        }

        if (move) {
            moveForward();
            putPheromone("Food", 1200);
        }
    }
    else {
        vector<Int8> way = {-1, 0, 1};

        for (auto it = way.begin(); it != way.end();) {
            const Cell &cell = world.getCell(position + angleWay[(angle + *it + 6) % 6]);
            bool rm = false;

            if (cell.type == "Wall")
                rm = true;
            for (const Pheromone &one : cell.pheromone)
                if (one.ownerId == id && one.type == "Search")
                    rm = true;
            if (rm)
                it = way.erase(it);
            else
                it++;
        }

        if (way.empty()) {
            bool cantMove = false;

            for (Uint8 i = 0; i < 6; i++) {
                const Cell &cell = world.getCell(position + angleWay[i]);
                bool rm = false;

                if (cell.type == "Wall")
                    rm = true;
                for (const Pheromone &one : cell.pheromone)
                    if (one.ownerId == id && one.type == "Search")
                        rm = true;
                if (!rm) {
                    cantMove = true;
                    break;
                }
            }
            if (cantMove)
                rotateRight();
            else {
                vector<Int8> way = {-1, 0, 1};

                for (auto it = way.begin(); it != way.end();) {
                    const Cell &cell = world.getCell(position + angleWay[(angle + *it + 6) % 6]);
                    bool rm = false;

                    if (cell.type == "Wall")
                        rm = true;
                    if (rm)
                        it = way.erase(it);
                    else
                        it++;
                }
                if (way.empty())
                    rotateRight();
                else {
                    if (rand() % 3) {
                        if (rand() % 2)
                            rotateLeft();
                        else
                            rotateRight();
                    }
                    moveForward();
                    //putPheromone("Search", 1200); // ?
                }
            }
        }
        else {
            const Int8 randWay = way[rand() % way.size()];

            if (randWay == -1)
                rotateLeft();
            else if (randWay == 1)
                rotateRight();
            moveForward();

            /*bool pheroFind = false;

              for (const Pheromone &one : world.getCell(position).pheromone)
              if (one.ownerId == id && one.type == "Search")
              pheroFind = true;
              if (!pheroFind)*/
            putPheromone("Search", 1200);
        }
    }
}

void Ant::aff(RenderWindow &window) const
{
    const Uint32 hexagonRadius = world.getHexagonRadius();
    const float tmp = sqrt(pow(-0.5 * hexagonRadius, 2) + pow(sin(M_PI / 3) * hexagonRadius, 2)) / 2;
    const float size = sqrt(pow(hexagonRadius, 2) - pow(tmp, 2)) * 2;
    RectangleShape rectange(Vector2f(size, size)); // !!!

    rectange.setOrigin(rectange.getSize() / (float)2);
    rectange.setPosition(world.getHexagonPosition(position));
    rectange.setRotation(angle * 60);
    rectange.setTexture(&texture);
    if (getResource("Food")) // tmp
        rectange.setFillColor(Color::Green);
    window.draw(rectange);
}
