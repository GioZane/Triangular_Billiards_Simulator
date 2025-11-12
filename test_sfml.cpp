#include <iostream>
#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Biliardo triangolare");
    sf::CircleShape circle(50.0);
    circle.setFillColor(sf::Color::Yellow);

    while (window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type== sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(circle);
        window.display();
    }
}