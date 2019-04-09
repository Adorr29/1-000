/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.hpp
*/

#ifndef WORLD_HPP
#define WORLD_HPP

#include <map>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Pheromone
{
    string type;
    size_t ownerId;
};

struct Cell
{
    string type;
    //vector<Pheromone> pheromone;
    //Tab *next[6]; // 6 for hexagon
};

class World
{
public:
    World();
    ~World();
    ConvexShape getHexagon() const;
    Vector2f getHexagonPos(const Vector2i &pos) const;
    const Uint32 &getHexagonRadius() const;
    void aff(RenderWindow &window) const;

private:
    void sizeUp(); // ?

private:
    const Uint32 hexagonRadius;
    map<Int32, map<Int32, Cell>> cell;
};

#endif
