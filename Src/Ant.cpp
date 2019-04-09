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
    : world(_world), id(antNum++), hp(10), hpMax(10), stock(0), stockMax(10), pos(_pos)
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

void Ant::aff(RenderWindow &window) const
{
    (void)window;
    Uint32 hexagonRadius = world.getHexagonRadius();
    Vector2f hexagonPos = world.getHexagonPos(pos);
    double var = sqrt(pow(-0.5 * hexagonRadius, 2) + pow(sin(M_PI / 3) * hexagonRadius, 2)) / 2;

    printf("len : %f\n", sqrt(pow(hexagonRadius, 2) - pow(var, 2)));
    // TODO
}
