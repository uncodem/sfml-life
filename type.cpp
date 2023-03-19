#include "type.hpp"

Cell::Cell(int x, int y, int w, int h) {
	shape.setPosition(sf::Vector2f(x, y));
	shape.setSize(sf::Vector2f(w, h));
	live = false;

	shape.setOutlineColor(sf::Color(16,16,16, 255));
	shape.setOutlineThickness(1);

}

Cell::Cell() : Cell(0, 0, 10, 10) {}