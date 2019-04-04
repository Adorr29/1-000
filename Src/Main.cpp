/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** Main.cpp
*/

#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    VideoMode videoMode = VideoMode::getDesktopMode();
    RenderWindow window(videoMode, "Window Title", Style::Fullscreen);

    window.setFramerateLimit(30);
    while (window.isOpen()) {
        for (Event event; window.pollEvent(event);) {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear();
        // aff
        window.display();
    }
    return 0;
}
