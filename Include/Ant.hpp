/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** Ant.hpp
*/

#ifndef ANT_HPP
#define ANT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include "World.hpp" // ?

using namespace std;
using namespace sf;

class Ant
{
public:
    Ant(const World &_world, const Vector2i &_pos);
    ~Ant();
    const size_t &getId() const;
    const size_t &getHp() const;
    const size_t &getHpMax() const;
    const size_t &getStock() const;
    const size_t &getStockMax() const;
    const Vector2i &getPos() const;
    const Int8 &getAngle() const;
    void rotateLeft();
    void rotateRight();
    bool moveForward();
    void aff(RenderWindow &window) const;

private:
    const World &world; // ?
    const size_t id; // ?
    size_t hp;
    size_t hpMax;
    size_t stock;
    size_t stockMax;
    Vector2i pos;
    Int8 angle;

private:
    static size_t antNum; // ?
};

#endif
