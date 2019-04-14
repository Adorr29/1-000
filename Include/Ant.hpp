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
    Ant(World &_world, const Vector2i &_pos);
    ~Ant();
    const size_t &getId() const;
    const Uint32 &getHp() const;
    const Uint32 &getHpMax() const;
    const Vector2i &getPos() const;
    const Int8 &getAngle() const;
    void rotateLeft(); // private ?
    void rotateRight(); // private ?
    bool moveForward(); // private ?
    Uint32 putResource(const string &type, const Uint32 &quantity); // private ?
    Uint32 pickResource(const string &type, const Uint32 &quantity); // private ?
    void ai();
    void aff(RenderWindow &window) const;

private:
    World &world; // ?
    const size_t id; // ?
    Uint32 hp;
    Uint32 hpMax;
    map<string, Uint32> resource; // add getter TODO
    Uint32 quantityMax; // add getter TODO
    Vector2i pos;
    Int8 angle;

private:
    static size_t antNum; // ?
};

#endif
