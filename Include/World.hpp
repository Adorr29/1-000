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
    map<string, Uint32> resource;
    //vector<Pheromone> pheromone;
};

class World
{
public:
    World();
    ~World();
    const Cell &getCell(const Vector2i &pos) const;
    ConvexShape getHexagon() const;
    Vector2f getHexagonPos(const Vector2i &pos) const;
    const Uint32 &getHexagonRadius() const;
    Uint32 putResource(const Vector2i &pos, const string &type, const Uint32 &quantity);
    Uint32 pickResource(const Vector2i &pos, const string &type, const Uint32 &quantity);
    void aff(RenderWindow &window) const;

private:
    bool cellExist(const Vector2i &pos) const;
    void sizeUp(); // ?
    Vector2i randCellPos(const string &type = "") const;
    void scatterCell(const string &type, const Vector2i &pos, const size_t &size);

private:
    const Uint32 hexagonRadius;
    map<Int32, map<Int32, Cell>> cell;
};

#endif
