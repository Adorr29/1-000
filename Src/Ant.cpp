/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** Ant.cpp
*/

#include <cmath>
#include "Ant.hpp"

size_t Ant::antNum = 1;

Ant::Ant(const World &_world, const Vector2i &_pos)
    : world(_world), id(antNum++), hp(10), hpMax(10), stock(0), stockMax(10), pos(_pos), angle(0)
{
}

Ant::~Ant()
{
}

const size_t &Ant::getId() const
{
    return id;
}

const size_t &Ant::getHp() const
{
    return hp;
}

const size_t &Ant::getHpMax() const
{
    return hpMax;
}

const size_t &Ant::getStock() const
{
    return stock;
}

const size_t &Ant::getStockMax() const
{
    return stockMax;
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
    if (angle == 0)
        pos.y--;
    else if (angle == 1)
        pos.x++;
    else if (angle == 2) {
        pos.x++;
        pos.y++;
    }
    else if (angle == 3)
        pos.y++;
    else if (angle == 4)
        pos.x--;
    else if (angle == 5) {
        pos.x--;
        pos.y--;
    }
    return true;
}

void Ant::aff(RenderWindow &window) const
{
    Uint32 hexagonRadius = world.getHexagonRadius();
    Vector2f hexagonPos = world.getHexagonPos(pos);
    float tmp = sqrt(pow(-0.5 * hexagonRadius, 2) + pow(sin(M_PI / 3) * hexagonRadius, 2)) / 2;
    float size = sqrt(pow(hexagonRadius, 2) - pow(tmp, 2)) * 2;
    Texture texture; // !!!
    RectangleShape rectange(Vector2f(size, size)); // !!!

    texture.loadFromFile("Resources/Texture/Ant.png"); // tmp
    rectange.setOrigin(rectange.getSize() / (float)2);
    rectange.setPosition(hexagonPos);
    rectange.setRotation(angle * 60);
    rectange.setTexture(&texture);
    window.draw(rectange);
}
