/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** Ant.cpp
*/

#include <iostream> // tmp
#include <cmath>
#include "Ant.hpp"

size_t Ant::antNum = 1;

Ant::Ant(World &_world, const Vector2i &_pos)
    : world(_world), id(antNum++), hp(10), hpMax(10), quantityMax(10), pos(_pos), angle(0)
{
}

Ant::~Ant()
{
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

const Vector2i &Ant::getPos() const
{
    return pos;
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
    Vector2i newPos(pos);

    if (angle == 0)
        newPos.y--;
    else if (angle == 1)
        newPos.x++;
    else if (angle == 2) {
        newPos.x++;
        newPos.y++;
    }
    else if (angle == 3)
        newPos.y++;
    else if (angle == 4)
        newPos.x--;
    else if (angle == 5) {
        newPos.x--;
        newPos.y--;
    }
    if (world.getCell(newPos).type == "Wall")
        return false;
    pos = newPos;
    return true;
}

Uint32 Ant::putResource(const string &type, const Uint32 &quantity)
{
    Uint32 put;
    Uint32 newQuantity;

    if (resource.find(type) == resource.end())
        return 0;
    if (quantity > resource[type])
        newQuantity = resource[type];
    else
        newQuantity = quantity;
    put = world.putResource(pos, type, newQuantity);
    resource[type] -= put;
    return put;
}

Uint32 Ant::pickResource(const string &type, const Uint32 &quantity)
{
    Uint32 pick;
    Uint32 newQuantity;
    Uint32 totalResource = 0;

    for (auto one : resource)
        totalResource += one.second;
    if (quantity > quantityMax - totalResource)
        newQuantity = quantityMax - totalResource;
    else
        newQuantity = quantity;
    pick = world.pickResource(pos, type, newQuantity);
    resource[type] += pick;
    return pick;
}

void Ant::ai()
{
    if (rand() % 3) {
        if (rand() % 2)
            rotateRight();
        else
            rotateLeft();
    }
    moveForward();
    pickResource("Food", 10);
    if (world.getCell(pos).type == "Anthill")
        putResource("Food", 20);
}

void Ant::aff(RenderWindow &window) const
{
    const Uint32 hexagonRadius = world.getHexagonRadius();
    const float tmp = sqrt(pow(-0.5 * hexagonRadius, 2) + pow(sin(M_PI / 3) * hexagonRadius, 2)) / 2;
    const float size = sqrt(pow(hexagonRadius, 2) - pow(tmp, 2)) * 2;
    Texture texture; // !!!
    RectangleShape rectange(Vector2f(size, size)); // !!!

    texture.loadFromFile("Resources/Texture/Ant.png"); // tmp
    rectange.setOrigin(rectange.getSize() / (float)2);
    rectange.setPosition(world.getHexagonPos(pos));
    rectange.setRotation(angle * 60);
    rectange.setTexture(&texture);
    if (resource.find("Food") != resource.end() && resource.at("Food") > 0) // tmp
        rectange.setFillColor(Color::Green);
    window.draw(rectange);
}
