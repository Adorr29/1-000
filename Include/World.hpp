/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.hpp
*/

#ifndef WORLD_HPP
#define WORLD_HPP

#include <list>
#include <map>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Pheromone
{
    string type;
    size_t ownerId; // or const ref on Ant ?
    Uint32 quantity;
};

struct Cell
{
    string type;
    map<string, Uint32> resource;
    list<Pheromone> pheromone;
};

class World
{
public:
    World(const size_t &size);
    ~World();
    const Cell &getCell(const Vector2i &position) const;
    ConvexShape getHexagon() const;
    Vector2f getHexagonPosition(const Vector2i &position) const;
    const Uint32 &getHexagonRadius() const;
    Uint32 putResource(const Vector2i &position, const string &type, const Uint32 &quantity);
    Uint32 pickResource(const Vector2i &position, const string &type, const Uint32 &quantity);
    void putPheromone(const Vector2i &position, const size_t &id, const string &type, const Uint32 &quantity);
    void update();
    void aff(RenderWindow &window) const;

private:
    bool cellExist(const Vector2i &position) const;
    void sizeUp(); // ?
    Vector2i randCellPosition(const string &type = "") const;
    void scatterCell(const string &type, const Vector2i &position, const size_t &size);

private:
    const Uint32 hexagonRadius;
    map<Int32, map<Int32, Cell>> cell;
};

#endif
