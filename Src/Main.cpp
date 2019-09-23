/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** Main.cpp
*/

#include <iostream> // tmp
#include <list>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "Ant.hpp"

using namespace sf;

int main()
{
    srand(time(NULL));
    ContextSettings settings;
    settings.antialiasingLevel = 8; // ?
    VideoMode videoMode = VideoMode::getDesktopMode();
    RenderWindow window(videoMode, "1-000", Style::Fullscreen, settings);
    World world(40);
    list<Ant> antList;
    bool grapBool = false;
    Vector2f grap;
    float zoom = 1.0;

    for (size_t i = 0; i < 1; i++)
        antList.push_back(Ant(world, Vector2i(0, 0)));
    window.setFramerateLimit(30);
    while (window.isOpen()) {
        for (Event event; window.pollEvent(event);) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right) {
                    grapBool = true;
                    grap = Vector2f(event.mouseButton.x, event.mouseButton.y);
                }
            }
            else if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Right)
                    grapBool = false;
            }
            else if (event.type == Event::MouseMoved) {
                if (grapBool) {
                    View view = window.getView();
                    const Vector2f factor(view.getSize().x / window.getSize().x, view.getSize().y / window.getSize().y);

                    view.move(Vector2f((grap.x - event.mouseMove.x) * factor.x, (grap.y - event.mouseMove.y) * factor.y));
                    window.setView(view);
                    grap = Vector2f(event.mouseMove.x, event.mouseMove.y);
                }
            }
            else if (event.type == Event::MouseWheelMoved)
                zoom -= event.mouseWheel.delta * 0.1;
        }
        if (zoom != 1.0) {
            View view = window.getView();

            view.zoom(zoom);
            window.setView(view);
            zoom = (zoom + 1) / 2.0;
        }
        world.update();
        for (Ant &ant : antList) // tmp for fun
            ant.ai();
        window.clear(Color(50, 50, 50));
        world.aff(window); // ?
        for (const Ant &ant : antList)
            ant.aff(window); // ?
        window.display();
    }
    return 0;
}
